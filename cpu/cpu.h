#ifndef CPU_H
#define CPU_H

#include "../cpu/config.h"
#include "../cpu/control_unit.h"
#include "../datapath/alu.h"
#include "../datapath/regfile.h"
#include "../memory/memory.h"
#include "../clock/clock.h"

// Top-level CPU: owns all components and exposes a simple run interface.
class CPU {
public:
    CPU();

    // Load words from a flat array into instruction memory starting at TEXT_START.
    void load(const std::vector<std::vector<bool>>& program);

    // Run until HALT or maxCycles (0 = unlimited).
    void run(uint64_t maxCycles = 0);

    // Execute exactly one cycle.
    void step();

    // Print a hex dump of memory [from, to] to stdout.
    void dumpMemory(unsigned int from, unsigned int to) const;

    // Print all register values to stdout.
    void dumpRegisters() const;

    bool     isHalted()  const;
    uint64_t getCycles() const;
    CPUFlags getFlags()  const;

private:
    Memory  memory;
    RegFile regfile;
    RegFile multRegFile; // separate register file for HI/LO used by MULT/MULTU
    ALU     alu;
    Clock   clock;
    ControlUnit cu;
};

#endif // CPU_H
