#include "cpu.h"
#include "../utils/utils.h"
#include <iostream>
#include <iomanip>

// CPU constructor initializes components and sets SP to STACK_TOP.
CPU::CPU()
    : memory(MEM_WORDS, WORD_SIZE),
      regfile(NUM_REGS, WORD_SIZE),
      multRegFile(2, WORD_SIZE), // HI and LO registers
      alu(),
      clock(0),
      cu(memory, regfile, alu, clock, multRegFile) {
    // Initialise stack pointer to top of stack region
    regfile.write(REG_SP, num2unsignedBinary(static_cast<int>(STACK_TOP), WORD_SIZE));
}

// Load words from a flat array into instruction memory starting at TEXT_START.
void CPU::load(const std::vector<std::vector<bool>>& program) {
    for (unsigned int i = 0; i < program.size(); ++i) {
        memory.write(TEXT_START + i, program[i]);
    }
}

// Run the CPU until it halts or reaches maxCycles (if nonzero).
void CPU::run(uint64_t maxCycles) {
    uint64_t limit = (maxCycles == 0) ? UINT64_MAX : maxCycles;
    // Main loop: keep stepping until HALT or cycle limit reached
    while (!cu.isHalted() && clock.getCycle() < limit) {
        cu.step();
    }
}

// Execute exactly one CPU cycle.
void CPU::step() {
    cu.step();
}

// Print a memory dump of TEXT, DATA, and STACK sections to stdout.
void CPU::dumpMemory() const {
    constexpr unsigned int STACK_BOTTOM = DATA_END + 1;  // 0xF00

    auto lastNonZero = [&](unsigned int start, unsigned int end) -> int {
        for (int a = static_cast<int>(end); a >= static_cast<int>(start); --a)
            if (getMemoryWord(static_cast<unsigned int>(a)) != 0) return a;
        return static_cast<int>(start) - 1;
    };

    auto printRange = [&](unsigned int start, unsigned int end,
                          unsigned int forceAddr = ~0u, const char* forceLabel = nullptr) {
        unsigned int zeros = 0;
        auto flushZeros = [&]() {
            if (zeros > 0) {
                std::cout << "    ... (" << std::dec << zeros << " zero word"
                          << (zeros == 1 ? "" : "s") << ")\n";
                zeros = 0;
            }
        };
        for (unsigned int a = start; a <= end; ++a) {
            uint32_t v = getMemoryWord(a);
            if (v == 0 && a != forceAddr) { ++zeros; continue; }
            flushZeros();
            std::cout << "  [0x" << std::hex << std::setw(3) << std::setfill('0') << a
                      << "]  0x" << std::setw(8) << std::setfill('0') << v;
            if (a == forceAddr && forceLabel)
                std::cout << "  <- " << forceLabel;
            std::cout << "\n";
        }
        flushZeros();
        std::cout << std::dec;
    };

    std::cout << "\n=== Memory Dump ===\n";

    int tLast = lastNonZero(TEXT_START, TEXT_END);
    std::cout << "\n-- TEXT [0x000-0x7FF] (program code) --\n";
    if (tLast < static_cast<int>(TEXT_START)) {
        std::cout << "  (empty)\n";
    } else {
        printRange(TEXT_START, static_cast<unsigned int>(tLast));
    }

    int dLast = lastNonZero(DATA_START, DATA_END);
    if (dLast >= static_cast<int>(DATA_START)) {
        std::cout << "\n-- DATA [0x800-0xEFF] (static data) --\n";
        printRange(DATA_START, static_cast<unsigned int>(dLast));
    }

    unsigned int sp = getRegister(REG_SP);
    std::cout << "\n-- STACK [0xF00-0xFEF] (grows down";
    if (sp >= STACK_BOTTOM && sp <= STACK_TOP)
        std::cout << ", $sp=0x" << std::hex << std::setw(3) << std::setfill('0') << sp;
    std::cout << std::dec << ") --\n";

    if (sp > STACK_TOP || sp < STACK_BOTTOM) {
        std::cout << "  (SP=0x" << std::hex << sp << " out of stack region)\n" << std::dec;
    } else if (sp == STACK_TOP) {
        std::cout << "  $sp=0x" << std::hex << std::setw(3) << std::setfill('0') << sp << std::dec << "\n";
    } else {
        printRange(sp, STACK_TOP, sp, "$sp");
    }

    std::cout << "\n-- MMIO [0xFFE-0xFFF] --\n";
    std::cout << "  [0xFFE]  0x" << std::hex << std::setw(8) << std::setfill('0')
              << getMemoryWord(MMIO_IN)  << "  MMIO_IN  (stdin)\n";
    std::cout << "  [0xFFF]  0x" << std::hex << std::setw(8) << std::setfill('0')
              << getMemoryWord(MMIO_OUT) << "  MMIO_OUT (stdout)\n\n";
    std::cout << std::dec;
}

// Print all register values to stdout.
void CPU::dumpRegisters() const {
    std::cout << "Register dump:\n";
    for (unsigned int i = 0; i < NUM_REGS; ++i) {
        auto word = regfile.read(i);
        unsigned int val = static_cast<unsigned int>(unsignedBinaryToNum(word));
        std::cout << "  $" << std::setw(2) << std::setfill('0') << i
                  << " = 0x" << std::hex << std::setw(8) << std::setfill('0') << val
                  << "  (" << std::dec << val << ")\n";
    }
}

bool CPU::isHalted() const { return cu.isHalted(); }
uint64_t CPU::getCycles() const { return clock.getCycle(); }
CPUFlags CPU::getFlags() const { return cu.getFlags(); }
unsigned int CPU::getPC() const { return cu.getPC(); }

uint32_t CPU::getMemoryWord(unsigned int addr) const {
    auto bits = memory.read(addr);
    uint32_t result = 0;
    for (bool b : bits) result = (result << 1) | (b ? 1u : 0u);
    return result;
}

uint32_t CPU::getRegister(unsigned int idx) const {
    auto bits = regfile.read(idx);
    uint32_t result = 0;
    for (bool b : bits) result = (result << 1) | (b ? 1u : 0u);
    return result;
}
