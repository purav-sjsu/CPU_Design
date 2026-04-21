#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <vector>
#include "../cpu/config.h"
#include "../cpu/isa.h"
#include "../datapath/alu.h"
#include "../datapath/regfile.h"
#include "../memory/memory.h"
#include "../clock/clock.h"

// Flags register updated after every ALU operation
struct CPUFlags {
    bool zero     = false;
    bool negative = false;
    bool carry    = false;
    bool overflow = false;
};

// Internal control signals derived from the decoded instruction
struct ControlSignals {
    bool    regWrite  = false;  // write result to register file
    uint8_t regDst    = 0;      // 0=rt (I-type), 1=rd (R-type), 2=$ra (JAL)
    bool    aluSrc    = false;  // false=rt, true=sign-extended immediate
    bool    zeroImm   = false;  // use zero-extend instead of sign-extend (AND/OR/XOR I)
    uint8_t memToReg  = 0;      // 0=ALU result, 1=memory read, 2=PC+1 (JAL)
    bool    memRead   = false;
    bool    memWrite  = false;
    bool    branch    = false;
    bool    jump      = false;
    bool    jr        = false;
    bool    jal       = false;
    bool    lui       = false;  // load upper immediate (handled without ALU)
    bool    halt      = false;
    ALUOp   aluOp     = ALUOp::ADD;
};

class ControlUnit {
public:
    ControlUnit(Memory& mem, RegFile& rf, ALU& alu, Clock& clk);

    // Execute one Fetch / Decode / Execute / Writeback cycle.
    void step();

    bool      isHalted()  const { return halted; }
    unsigned int getPC()  const { return pc; }
    CPUFlags  getFlags()  const { return flags; }

private:
    Memory&  memory;
    RegFile& regfile;
    ALU&     alu;
    Clock&   clock;

    unsigned int pc      = TEXT_START;
    bool         halted  = false;
    CPUFlags     flags;

    ControlSignals generateControlSignals(const Instruction& instr) const;
    ALUOp          functToALUOp(uint8_t funct) const;
};

#endif // CONTROL_UNIT_H
