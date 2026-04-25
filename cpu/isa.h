#ifndef ISA_H
#define ISA_H

#include <vector>
#include <cstdint>

//  Instruction types
enum class InstrType { R, I, J, UNKNOWN };

// R-type funct codes
enum class Funct : uint8_t {
    SLL  = 0b000000,
    SRL  = 0b000010,
    SRA  = 0b000011,
    JR   = 0b001000,
    JALR = 0b001001,
    ADD  = 0b100000,
    ADDU = 0b100001,
    SUB  = 0b100010,
    AND  = 0b100100,
    OR   = 0b100101,
    XOR  = 0b100110,
    NOR  = 0b100111,
    SLT  = 0b101010,
    MULT = 0b011000,
    MULTU= 0b011001,
    MFHI = 0b010000,
    MFLO = 0b010010,
};

//  Opcodes (I-type / J-type; R-type all use opcode 0)
enum class Opcode : uint8_t {
    RTYPE = 0b000000,
    J     = 0b000010,
    JAL   = 0b000011,
    BEQ   = 0b000100,
    BNE   = 0b000101,
    ADDI  = 0b001000,
    SLTI  = 0b001010,
    ANDI  = 0b001100,
    ORI   = 0b001101,
    XORI  = 0b001110,
    LUI   = 0b001111,
    LW    = 0b100011,
    SW    = 0b101011,
    HALT  = 0b111111,  // custom: stops the CPU
};

//  Decoded instruction fields
struct Instruction {
    InstrType type    = InstrType::UNKNOWN;
    uint8_t   opcode  = 0;
    uint8_t   rs      = 0;   // source register 1 (bits 25-21)
    uint8_t   rt      = 0;   // source register 2 / dest for I-type (bits 20-16)
    uint8_t   rd      = 0;   // destination register(bits 15-11, R-type)
    uint8_t   shamt   = 0;   // shift amount          (bits 10-6,  R-type)
    uint8_t   funct   = 0;   // function code         (bits  5-0,  R-type)
    int16_t   imm16   = 0;   // signed 16-bit immediate (I-type)
    uint32_t  target  = 0;   // 26-bit jump target (J-type)
};

// Extract an unsigned integer from bits [start..end] (inclusive, MSB-first indexing).
unsigned int extractBits(const std::vector<bool>& word, int start, int end);

// Extract bits [start..end] as a vector<bool>.
std::vector<bool> extractBitsVec(const std::vector<bool>& word, int start, int end);

// Decode a 32-bit word (index 0 = MSB) into an Instruction.
Instruction decode(const std::vector<bool>& word);

#endif // ISA_H
