#include "loader.h"
#include "debug.h"
#include "../cpu/cpu.h"
#include "../cpu/mmio.h"
#include <iostream>
#include <string>

static void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog
              << " <program.bin> [--dump-regs] [--dump-mem] [--max-cycles N] [--debug] [--arg N]\n"
              << "  --arg N   set the runtime input value (pre-loaded into MMIO_IN)\n"
              << "            factorial: --arg N  (computes N!)\n"
              << "            fibonacci: --arg N  (prints N fibonacci terms)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { printUsage(argv[0]); return 1; }

    std::string binPath;
    bool     dumpRegs  = false;
    bool     dumpMem   = false;
    bool     debugMode = false;
    uint64_t maxCycles = 0;
    int inputArg = -1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if      (arg == "--dump-regs")                 dumpRegs  = true;
        else if (arg == "--dump-mem")                  dumpMem   = true;
        else if (arg == "--debug")                     debugMode = true;
        else if (arg == "--max-cycles" && i + 1 < argc) maxCycles = std::stoull(argv[++i]);
        else if (arg == "--arg" && i + 1 < argc)       inputArg = std::stoi(argv[++i]);
        else if (arg[0] != '-')                        binPath   = arg;
    }

    if (binPath.empty()) { printUsage(argv[0]); return 1; }

    try {
        std::cerr << "[emulator] Loading " << binPath << "\n";

        auto rawWords = loadBinary(binPath);
        std::vector<std::vector<bool>> program;
        program.reserve(rawWords.size());
        for (uint32_t w : rawWords) program.push_back(wordToBits(w));

        std::cerr << "[emulator] " << rawWords.size() << " words loaded into memory\n";

        uint32_t mmioVal = static_cast<uint32_t>(inputArg >= 0 ? inputArg : 4);
        setMMIOInputWord(mmioVal);
        std::cerr << "[emulator] MMIO_IN loaded with " << mmioVal << "\n";

        CPU cpu;
        cpu.load(program);
        std::cerr << "[emulator] CPU initialized\n\n";

        if (debugMode) {
            std::cerr << "[emulator] Running in debug mode\n";
            runDebug(cpu, maxCycles);
        } else {
            cpu.run(maxCycles);
        }

        std::cerr << "\n[emulator] Halted after " << cpu.getCycles() << " cycles\n";

        if (dumpRegs) cpu.dumpRegisters();
        if (dumpMem) cpu.dumpMemory();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
