#ifndef ALU_H
#define ALU_H

#include <vector>
#include <cstdint>

// Operations the ALU can perform (passed in by ControlUnit)
enum class ALUOp : uint8_t {
    ADD  = 0,   // A + B
    SUB  = 1,   // A - B
    AND  = 2,   // A & B
    OR   = 3,   // A | B
    XOR  = 4,   // A ^ B
    NOR  = 5,   // ~(A | B)
    SLT  = 6,   // 1 if A < B (signed), else 0
    SLL  = 7,   // A << lower5(B)
    SRL  = 8,   // A >> lower5(B)  logical
    SRA  = 9,   // A >> lower5(B)  arithmetic (sign-extended)
    // MULT = 10,  // HI/LO = A * B (signed)
};

struct ALUResult {
    std::vector<bool> result;
    bool zero;      // result == 0
    bool negative;  // MSB of result (sign bit)
    bool carry;     // unsigned carry-out / borrow
    bool overflow;  // signed overflow
};

class ALU {
public:
    // Execute op on A and B; for shifts B holds the shift amount (lower 5 bits)
    ALUResult execute(const std::vector<bool>& A,
                      const std::vector<bool>& B,
                      ALUOp op);
};

#endif // ALU_H
