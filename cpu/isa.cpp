#include "isa.h"

// Extract bits [start, end] from a 32-bit instruction word and return as unsigned int.
unsigned int extractBits(const std::vector<bool>& word, int start, int end) {
    unsigned int result = 0;
    for (int i = start; i <= end; ++i)
        result = (result << 1) | (word[i] ? 1u : 0u);
    return result;
}

// Extract bits [start, end] from a 32-bit instruction word and return as vector<bool>.
std::vector<bool> extractBitsVec(const std::vector<bool>& word, int start, int end) {
    return std::vector<bool>(word.begin() + start, word.begin() + end + 1);
}

// Decode a 32-bit instruction word into its fields.
Instruction decode(const std::vector<bool>& word) {
    Instruction instr;
    instr.opcode = static_cast<uint8_t>(extractBits(word, 0, 5));

    // Decode R-type, J-type, and I-type instructions based on opcode
    if (instr.opcode == static_cast<uint8_t>(Opcode::RTYPE)) {
        instr.type  = InstrType::R;
        instr.rs    = static_cast<uint8_t>(extractBits(word, 6,  10));
        instr.rt    = static_cast<uint8_t>(extractBits(word, 11, 15));
        instr.rd    = static_cast<uint8_t>(extractBits(word, 16, 20));
        instr.shamt = static_cast<uint8_t>(extractBits(word, 21, 25));
        instr.funct = static_cast<uint8_t>(extractBits(word, 26, 31));
    } else if (instr.opcode == static_cast<uint8_t>(Opcode::J) ||
               instr.opcode == static_cast<uint8_t>(Opcode::JAL)) {
        instr.type   = InstrType::J;
        instr.target = extractBits(word, 6, 31);
    } else if (instr.opcode == static_cast<uint8_t>(Opcode::HALT)) {
        instr.type = InstrType::UNKNOWN; // signals HALT to control unit
    } else {
        instr.type = InstrType::I;
        instr.rs   = static_cast<uint8_t>(extractBits(word, 6,  10));
        instr.rt   = static_cast<uint8_t>(extractBits(word, 11, 15));
        unsigned int raw = extractBits(word, 16, 31);
        // Sign-extend 16-bit immediate
        instr.imm16 = (raw & 0x8000u) ? static_cast<int16_t>(raw | 0xFFFF0000u)
                                       : static_cast<int16_t>(raw);
    }
    return instr;
}
