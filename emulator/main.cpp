#include "loader.h"
#include "../cpu/cpu.h"
#include "../utils/utils.h"
#include <iostream>
#include <string>

static void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog
              << " <program.bin> [--dump-regs] [--dump-mem] [--max-cycles N] [--arg N]\n"
              << "  --arg N   set the factorial input (replaces hardcoded value, 0-15)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { printUsage(argv[0]); return 1; }

    std::string binPath;
    bool dumpRegs = false;
    bool dumpMem  = false;
    uint64_t maxCycles = 0;
    int factorialArg = -1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if      (arg == "--dump-regs")   dumpRegs = true;
        else if (arg == "--dump-mem")    dumpMem  = true;
        else if (arg == "--max-cycles" && i + 1 < argc)
            maxCycles = std::stoull(argv[++i]);
        else if (arg == "--arg" && i + 1 < argc)
            factorialArg = std::stoi(argv[++i]);
        else if (arg[0] != '-')          binPath = arg;
    }

    if (binPath.empty()) { printUsage(argv[0]); return 1; }

    try {
        // Load binary
        auto rawWords = loadBinary(binPath);

        // Convert uint32_t vector to  vector<bool>
        std::vector<std::vector<bool>> program;
        program.reserve(rawWords.size());
        for (uint32_t w : rawWords) program.push_back(wordToBits(w));

        // Patch first instruction's 16-bit immediate with --arg value.
        // Instruction 0 is always "addi $a0, $zero, <N>" (bits 15-0 = immediate).
        if (factorialArg >= 0 && !program.empty()) {
            auto imm = num2unsignedBinary(factorialArg, 16);
            for (int b = 0; b < 16; ++b)
                program[0][16 + b] = imm[b];
        }

        // Boot CPU
        CPU cpu;
        cpu.load(program);
        cpu.run(maxCycles);

        std::cerr << "\n[CPU halted after " << cpu.getCycles() << " cycles]\n";

        if (dumpRegs) cpu.dumpRegisters();
        if (dumpMem)  cpu.dumpMemory(TEXT_START, DATA_END);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
