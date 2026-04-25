#include "encoder.h"
#include "../cpu/isa.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>

// Function to encode an R-type instruction
uint32_t encodeR(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct) {
    return (0u          << 26) |
           ((rs & 0x1F) << 21) |
           ((rt & 0x1F) << 16) |
           ((rd & 0x1F) << 11) |
           ((shamt& 0x1F)<< 6) |
           (funct & 0x3F);
}

// Function to encode an I-type instruction
uint32_t encodeI(uint8_t opcode, uint8_t rs, uint8_t rt, uint16_t imm16) {
    return ((opcode& 0x3F) << 26) |
           ((rs    & 0x1F) << 21) |
           ((rt    & 0x1F) << 16) |
           (imm16  & 0xFFFF);
}

// Function to encode a J-type instruction
uint32_t encodeJ(uint8_t opcode, uint32_t target26) {
    return ((opcode & 0x3F) << 26) | (target26 & 0x03FFFFFF);
}

// ── Symbol / immediate resolver ───────────────────────────────────────────────
static int resolveOperand(const std::string& s,
                           const std::map<std::string, unsigned int>& symtab) {
    bool isLabel;
    int v = parseImmediate(s, &isLabel);
    if (isLabel) {
        auto it = symtab.find(s);
        if (it == symtab.end())
            throw std::runtime_error("Undefined label: " + s);
        return static_cast<int>(it->second);
    }
    return v;
}

// Parse "offset($base)" into offset and base register
static void parseMemOperand(const std::string& op, int& offset, int& base) {
    size_t lp = op.find('(');
    if (lp == std::string::npos)
        throw std::runtime_error("Expected offset(base) syntax, got: " + op);
    size_t rp = op.find(')', lp);
    std::string offStr  = op.substr(0, lp);
    std::string baseStr = op.substr(lp + 1, rp - lp - 1);
    offset = offStr.empty() ? 0 : parseImmediate(offStr);
    base   = resolveRegister(baseStr);
}

// Main encoding function: takes a parsed instruction line, symbol table, and current address,
// and returns the 32-bit machine code word.
uint32_t encode(const ParsedLine& pl,
                const std::map<std::string, unsigned int>& symtab,
                unsigned int currentAddr) {
    const std::string& mn  = pl.mnemonic;
    const auto& ops = pl.operands;

    // R-type instructions: add, sub, and, or, xor, nor, slt, sll, srl, sra, jr, jalr
    if (mn=="add"  || mn=="addu") return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, mn=="add" ? 0x20 : 0x21);
    if (mn=="sub"  || mn=="subu") return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, mn=="sub" ? 0x22 : 0x23);
    if (mn=="and")  return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, 0x24);
    if (mn=="or")   return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, 0x25);
    if (mn=="xor")  return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, 0x26);
    if (mn=="nor")  return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, 0x27);
    if (mn=="slt")  return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, 0x2A);
    if (mn=="sltu") return encodeR(resolveRegister(ops[1]), resolveRegister(ops[2]), resolveRegister(ops[0]), 0, 0x2B);

    // Shifts: sll $rd, $rt, shamt
    if (mn=="sll") return encodeR(0, resolveRegister(ops[1]), resolveRegister(ops[0]), parseImmediate(ops[2]) & 0x1F, 0x00);
    if (mn=="srl") return encodeR(0, resolveRegister(ops[1]), resolveRegister(ops[0]), parseImmediate(ops[2]) & 0x1F, 0x02);
    if (mn=="sra") return encodeR(0, resolveRegister(ops[1]), resolveRegister(ops[0]), parseImmediate(ops[2]) & 0x1F, 0x03);

    // jr $rs  /  jalr $rd, $rs
    if (mn=="jr")   return encodeR(resolveRegister(ops[0]), 0, 0,  0, 0x08);
    if (mn=="jalr") return encodeR(resolveRegister(ops[1]), 0, resolveRegister(ops[0]), 0, 0x09);

    // Multiplication instructions: mult, multu, mfhi, mflo
    if (mn=="mult")  return encodeR(resolveRegister(ops[0]), resolveRegister(ops[1]), 0, 0, 0x18);
    if (mn=="multu") return encodeR(resolveRegister(ops[0]), resolveRegister(ops[1]), 0, 0, 0x19);
    if (mn=="mfhi")  return encodeR(0, 0, resolveRegister(ops[0]), 0, 0x10);
    if (mn=="mflo")  return encodeR(0, 0, resolveRegister(ops[0]), 0, 0x12);

    //  I-type arithmetic
    if (mn=="addi")  return encodeI(0x08, resolveRegister(ops[1]), resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[2]));
    if (mn=="addiu") return encodeI(0x09, resolveRegister(ops[1]), resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[2]));
    if (mn=="slti")  return encodeI(0x0A, resolveRegister(ops[1]), resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[2]));
    if (mn=="andi")  return encodeI(0x0C, resolveRegister(ops[1]), resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[2]));
    if (mn=="ori")   return encodeI(0x0D, resolveRegister(ops[1]), resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[2]));
    if (mn=="xori")  return encodeI(0x0E, resolveRegister(ops[1]), resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[2]));
    if (mn=="lui")   return encodeI(0x0F, 0, resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[1]));

    // I-type memory: lw/sw $rt, offset($rs)
    if (mn=="lw") {
        int offset, base;
        parseMemOperand(ops[1], offset, base);
        return encodeI(0x23, base, resolveRegister(ops[0]), (uint16_t)offset);
    }
    if (mn=="sw") {
        int offset, base;
        parseMemOperand(ops[1], offset, base);
        return encodeI(0x2B, base, resolveRegister(ops[0]), (uint16_t)offset);
    }

    // Branches: beq/bne $rs, $rt, label
    if (mn=="beq" || mn=="bne") {
        int target = resolveOperand(ops[2], symtab);
        // Branch offset = target - (currentAddr + 1)  [word-addressed, no <<2]
        int offset  = target - static_cast<int>(currentAddr + 1);
        uint8_t op  = (mn=="beq") ? 0x04 : 0x05;
        return encodeI(op, resolveRegister(ops[0]), resolveRegister(ops[1]), (uint16_t)offset);
    }

    // J-type j/jal label
    if (mn=="j")   return encodeJ(0x02, (uint32_t)resolveOperand(ops[0], symtab));
    if (mn=="jal") return encodeJ(0x03, (uint32_t)resolveOperand(ops[0], symtab));

    //  Pseudo-instructions 
    // nop → sll $0, $0, 0
    if (mn=="nop")  return 0x00000000;
    // halt → opcode 0x3F
    if (mn=="halt") return (0x3F << 26);
    // move $rd, $rs → add $rd, $zero, $rs
    if (mn=="move") return encodeR(0, resolveRegister(ops[1]), resolveRegister(ops[0]), 0, 0x20);
    // li $rt, imm → addi $rt, $zero, imm
    if (mn=="li")   return encodeI(0x08, 0, resolveRegister(ops[0]), (uint16_t)parseImmediate(ops[1]));

    throw std::runtime_error("Unknown mnemonic: " + mn + " (line " +
                              std::to_string(pl.lineNumber) + ")");
}
