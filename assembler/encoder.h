#ifndef ENCODER_H
#define ENCODER_H

#include "parser.h"
#include <cstdint>
#include <map>
#include <string>

// Encode one parsed instruction into a 32-bit word.
// symtab maps label names to their word addresses.
// currentAddr is the address of this instruction (needed for branch offsets).
uint32_t encode(const ParsedLine& pl,
                const std::map<std::string, unsigned int>& symtab,
                unsigned int currentAddr);

// Pack the five fields of a 32-bit MIPS R-type word.
uint32_t encodeR(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);

// Pack the fields of a 32-bit MIPS I-type word.
uint32_t encodeI(uint8_t opcode, uint8_t rs, uint8_t rt, uint16_t imm16);

// Pack the fields of a 32-bit MIPS J-type word.
uint32_t encodeJ(uint8_t opcode, uint32_t target26);

#endif // ENCODER_H
