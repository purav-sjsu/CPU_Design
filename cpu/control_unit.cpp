#include "control_unit.h"
#include "mmio.h"
#include "../datapath/mux.h"
#include "../datapath/sign_extend.h"
#include "../datapath/multiplier.h"
#include "../utils/utils.h"
#include <stdexcept>


ControlUnit::ControlUnit(Memory& mem, RegFile& rf, ALU& alu, Clock& clk, RegFile& multRf)
    : memory(mem), regfile(rf), multRf(multRf), alu(alu), clock(clk) {}

//  Main Control Unit cycle: Fetch, Decode, Execute, Memory Access, Writeback, PC Update
void ControlUnit::step() {
    if (halted) return;

    // Fetch instruction word from memory at PC
    auto instrWord = memory.read(pc);
    // Default next PC is PC+1 (word-addressed)
    unsigned int nextPC = pc + 1;

    // Decode instruction and generate control signals
    Instruction instr = decode(instrWord);
    ControlSignals sig = generateControlSignals(instr);

    if (sig.halt) { halted = true; return; }

    //  READ REGISTERS 
    auto rsVal = regfile.read(instr.rs);
    auto rtVal = regfile.read(instr.rt);

    //  LUI: handled without ALU or memory access; writes imm16 to upper half of rt
    if (sig.lui) {
        // Place imm16 in the upper 16 bits, lower 16 zeroed
        std::vector<bool> luiVal(WORD_SIZE, false);
        auto imm16bits = extractBitsVec(instrWord, 16, 31);
        for (unsigned int i = 0; i < 16; ++i) luiVal[i] = imm16bits[i];
        if (instr.rt != REG_ZERO) regfile.write(instr.rt, luiVal);
        pc = nextPC;
        clock.tick();
        return;
    }

    //  ALU INPUT SELECT
    std::vector<bool> imm16bits = extractBitsVec(instrWord, 16, 31);
    std::vector<bool> immExt = sig.zeroImm
        ? zeroExtend(imm16bits, WORD_SIZE)
        : signExtend(imm16bits, WORD_SIZE);

    // For register shifts (SLL/SRL/SRA with shamt field) B = shamt value
    std::vector<bool> aluB;
    bool isRegShift = (instr.type == InstrType::R) &&
                      (instr.funct == static_cast<uint8_t>(Funct::SLL) ||
                       instr.funct == static_cast<uint8_t>(Funct::SRL) ||
                       instr.funct == static_cast<uint8_t>(Funct::SRA));
    if (isRegShift) {
        aluB = num2unsignedBinary(instr.shamt, WORD_SIZE);
    } else {
        aluB = mux2(rtVal, immExt, sig.aluSrc);
    }

    // EXECUTE
    ALUResult aluRes;
    aluRes = alu.execute(rsVal, aluB, sig.aluOp);
    unsigned int aluAddr = static_cast<unsigned int>(unsignedBinaryToNum(aluRes.result));
    
    // Multiplication handling: if it's a MULT/MULTU instruction, perform multiplication and write to HI/LO registers in multRf
    if (sig.mult && sig.multRegWrite) {
        std::vector<bool> multResult(WORD_SIZE * 2, false);
        multResult = Multiplier(rsVal, aluB).getProduct();
        multRf.write(0, std::vector<bool>(multResult.begin(), multResult.begin() + WORD_SIZE)); // HI
        multRf.write(1, std::vector<bool>(multResult.begin() + WORD_SIZE, multResult.end()));   // LO
    }

    // If instruction is MFHI/MFLO, read from HI/LO registers in multRf instead of memory or ALU result
    std::vector<bool> mfhiloData(WORD_SIZE, false); // default value for memory write (SW) when memWrite is false
    if (sig.mfhi) {
        mfhiloData = multRf.read(0); // HI register is at index 0
    } else if (sig.mflo) {
        mfhiloData = multRf.read(1); // LO register is at index 1
    }

    //  MEMORY ACCESS (for LW/SW)
    std::vector<bool> memData(WORD_SIZE, false);
    if (sig.memRead) {
        memData = isMMIO(aluAddr) ? mmioRead(aluAddr) : memory.read(aluAddr);
    }
    if (sig.memWrite) {
        if (isMMIO(aluAddr)) mmioWrite(aluAddr, rtVal);
        else                 memory.write(aluAddr, rtVal);
    }

    //  WRITEBACK
    auto pc1Vec   = num2unsignedBinary(static_cast<int>(nextPC), WORD_SIZE);
    auto writeData = mux4(aluRes.result, memData, pc1Vec,
                          mfhiloData,
                          sig.memToReg);

    unsigned int writeReg = (sig.regDst == 1) ? instr.rd
                          : (sig.regDst == 2) ? REG_RA
                          :                     instr.rt;

    // $zero (R0) is hardwired to 0; writes are silently discarded
    if (sig.regWrite && writeReg != REG_ZERO) {
        regfile.write(writeReg, writeData);
    }

    //  PC UPDATE
    // Branch offset is word-relative to PC+1 (no shift needed: word-addressed)
    unsigned int branchTarget = static_cast<unsigned int>(
        static_cast<int>(nextPC) + instr.imm16);
    unsigned int jumpTarget = instr.target;
    unsigned int jrTarget   = static_cast<unsigned int>(unsignedBinaryToNum(rsVal));

    bool beqTaken = (instr.opcode == static_cast<uint8_t>(Opcode::BEQ)) && aluRes.zero;
    bool bneTaken = (instr.opcode == static_cast<uint8_t>(Opcode::BNE)) && !aluRes.zero;
    bool branchTaken = sig.branch && (beqTaken || bneTaken);

    if      (sig.jr)                  pc = jrTarget;
    else if (sig.jump || sig.jal)     pc = jumpTarget;
    else if (branchTaken)             pc = branchTarget;
    else                              pc = nextPC;

    //  UPDATE FLAGS & CLOCK CYCLE
    flags = { aluRes.zero, aluRes.negative, aluRes.carry, aluRes.overflow };
    clock.tick();
}

// Generate Control signals based on the decoded instruction
ControlSignals ControlUnit::generateControlSignals(const Instruction& instr) const {
    ControlSignals sig;

    // Handle HALT separately since it doesn't fit cleanly into R/I/J types
    if (instr.opcode == static_cast<uint8_t>(Opcode::HALT)) {
        sig.halt = true;
        return sig;
    }

    switch (static_cast<Opcode>(instr.opcode)) {
        case Opcode::RTYPE:
            sig.regWrite = true;
            sig.regDst   = 1;       // rd
            sig.aluSrc   = false;   // B = rt
            sig.memToReg = 0;
            if (instr.funct == static_cast<uint8_t>(Funct::JR)) {
                sig.regWrite = false;
                sig.jr = true;
            } else if (instr.funct == static_cast<uint8_t>(Funct::JALR)) {
                sig.regWrite = true;
                sig.regDst   = 1;   // rd receives return address
                sig.memToReg = 2;
                sig.jr = true;
            } else if (instr.funct == static_cast<uint8_t>(Funct::MULT) ||
                instr.funct == static_cast<uint8_t>(Funct::MULTU)) {
                sig.regWrite = false;
                sig.multRegWrite = true;
                sig.mult = true;
                // sig.aluOp        = ALUOp::MULT; // Removed MULT from ALU and added as separate block
            } else if (instr.funct == static_cast<uint8_t>(Funct::MFHI)) {
                sig.mfhi     = true;
                sig.memToReg = 3; // read from HI register
            } else if (instr.funct == static_cast<uint8_t>(Funct::MFLO)) {
                sig.mflo     = true;
                sig.memToReg = 3; // read from LO register
            } else  {
                sig.aluOp = functToALUOp(instr.funct);
            }
            break;

        case Opcode::ADDI:
            sig.regWrite = true; sig.regDst = 0; sig.aluSrc = true;
            sig.memToReg = 0;    sig.aluOp  = ALUOp::ADD;
            break;

        case Opcode::SLTI:
            sig.regWrite = true; sig.regDst = 0; sig.aluSrc = true;
            sig.memToReg = 0;    sig.aluOp  = ALUOp::SLT;
            break;

        case Opcode::ANDI:
            sig.regWrite = true; sig.regDst = 0; sig.aluSrc = true;
            sig.zeroImm  = true; sig.memToReg = 0; sig.aluOp = ALUOp::AND;
            break;

        case Opcode::ORI:
            sig.regWrite = true; sig.regDst = 0; sig.aluSrc = true;
            sig.zeroImm  = true; sig.memToReg = 0; sig.aluOp = ALUOp::OR;
            break;

        case Opcode::XORI:
            sig.regWrite = true; sig.regDst = 0; sig.aluSrc = true;
            sig.zeroImm  = true; sig.memToReg = 0; sig.aluOp = ALUOp::XOR;
            break;

        case Opcode::LUI:
            sig.lui = true;  // handled specially in step()
            break;

        case Opcode::LW:
            sig.regWrite = true; sig.regDst = 0; sig.aluSrc = true;
            sig.memToReg = 1;    sig.memRead = true; sig.aluOp = ALUOp::ADD;
            break;

        case Opcode::SW:
            sig.regWrite = false; sig.aluSrc  = true;
            sig.memWrite = true;  sig.aluOp   = ALUOp::ADD;
            break;

        case Opcode::BEQ:
            sig.branch = true; sig.aluSrc = false; sig.aluOp = ALUOp::SUB;
            break;

        case Opcode::BNE:
            sig.branch = true; sig.aluSrc = false; sig.aluOp = ALUOp::SUB;
            break;

        case Opcode::J:
            sig.jump = true;
            break;

        case Opcode::JAL:
            sig.jump     = true;
            sig.jal      = true;
            sig.regWrite = true;
            sig.regDst   = 2;   // $ra
            sig.memToReg = 2;   // write PC+1 into $ra
            break;

        default:
            break;
    }
    return sig;
}

ALUOp ControlUnit::functToALUOp(uint8_t funct) const {
    switch (static_cast<Funct>(funct)) {
        case Funct::ADD:  return ALUOp::ADD;
        case Funct::ADDU: return ALUOp::ADD;
        case Funct::SUB:  return ALUOp::SUB;
        case Funct::AND:  return ALUOp::AND;
        case Funct::OR:   return ALUOp::OR;
        case Funct::XOR:  return ALUOp::XOR;
        case Funct::NOR:  return ALUOp::NOR;
        case Funct::SLT:  return ALUOp::SLT;
        case Funct::SLL:  return ALUOp::SLL;
        case Funct::SRL:  return ALUOp::SRL;
        case Funct::SRA:  return ALUOp::SRA;
        default:          return ALUOp::ADD;
    }
}
