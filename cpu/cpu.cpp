#include "cpu.h"
#include "../utils/utils.h"
#include <iostream>
#include <iomanip>

CPU::CPU()
    : memory(MEM_WORDS, WORD_SIZE),
      regfile(NUM_REGS, WORD_SIZE),
      alu(),
      clock(0),
      cu(memory, regfile, alu, clock) {
    // Initialise stack pointer to top of stack region
    regfile.write(REG_SP, num2unsignedBinary(static_cast<int>(STACK_TOP), WORD_SIZE));
}

void CPU::load(const std::vector<std::vector<bool>>& program) {
    for (unsigned int i = 0; i < program.size(); ++i) {
        memory.write(TEXT_START + i, program[i]);
    }
}

void CPU::run(uint64_t maxCycles) {
    uint64_t limit = (maxCycles == 0) ? UINT64_MAX : maxCycles;
    while (!cu.isHalted() && clock.getCycle() < limit) {
        cu.step();
    }
}

void CPU::step() {
    cu.step();
}

void CPU::dumpMemory(unsigned int from, unsigned int to) const {
    std::cout << "Memory dump [0x" << std::hex << from
              << " – 0x" << to << "]:\n";
    for (unsigned int addr = from; addr <= to; ++addr) {
        auto word = memory.read(addr);
        unsigned int val = static_cast<unsigned int>(unsignedBinaryToNum(word));
        std::cout << "  [0x" << std::setw(4) << std::setfill('0') << addr << "] = 0x"
                  << std::setw(8) << val << "\n";
    }
    std::cout << std::dec;
}

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

bool     CPU::isHalted()  const { return cu.isHalted(); }
uint64_t CPU::getCycles() const { return clock.getCycle(); }
CPUFlags CPU::getFlags()  const { return cu.getFlags(); }
