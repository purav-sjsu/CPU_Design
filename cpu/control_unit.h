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
    uint8_t memToReg  = 0;      // 0=ALU result, 1=memory read, 2=PC+1 (JAL), 3=HI/LO (MFHI/MFLO)
    bool    memRead   = false;  // read from memory
    bool    memWrite  = false;  // write to memory
    bool    branch    = false;  // true for BEQ/BNE (used with ALU zero flag)
    bool    jump      = false;  // true for J/JAL
    bool    jr        = false;  // true for JR
    bool    jal       = false;  // true for JAL
    bool    lui       = false;  // load upper immediate (handled without ALU)
    bool    mult      = false;  // true for MULT/MULTU (handled as separate block since result widths are different)
    bool    multRegWrite = false; // true for MULT/MULTU (writes to HI/LO instead of regfile)
    bool    mflo     = false;  // true for MFLO (read from LO)
    bool    mfhi     = false;  // true for MFHI (read from HI)
    bool    halt      = false;
    ALUOp   aluOp     = ALUOp::ADD;
};

class ControlUnit {
public:
    ControlUnit(Memory& mem, RegFile& rf, ALU& alu, Clock& clk, RegFile& multRf);

    // Execute one Fetch / Decode / Execute / Writeback cycle.
    void step();

    bool      isHalted()  const { return halted; }
    unsigned int getPC()  const { return pc; }
    CPUFlags  getFlags()  const { return flags; }

private:
    Memory&  memory;
    RegFile& regfile;
    RegFile& multRf;
    ALU&     alu;
    Clock&   clock;

    unsigned int pc = TEXT_START;   // program counter starts at TEXT_START
    bool         halted  = false;   // HALT instruction sets this to true
    CPUFlags     flags;             // Updated after every ALU operation; used for branches and JR

    ControlSignals generateControlSignals(const Instruction& instr) const;
    ALUOp          functToALUOp(uint8_t funct) const;
};

#endif // CONTROL_UNIT_H
