#include "alu.h"
#include "adders.h"
#include "bitwise.h"
#include <cstdint>

// ── Safe bit-conversion helpers ───────────────────────────────────────────────
// (utils.h num2unsignedBinary uses 1<<size which overflows for size=32)

// vector<bool> (MSB at index 0) → uint32_t
static uint32_t toU32(const std::vector<bool>& v) {
    uint32_t r = 0;
    for (bool b : v) r = (r << 1) | (b ? 1u : 0u);
    return r;
}

// vector<bool> → int32_t (two's complement interpretation)
static int32_t toS32(const std::vector<bool>& v) {
    return static_cast<int32_t>(toU32(v));
}

// uint32_t → vector<bool> of length N, MSB at index 0
static std::vector<bool> fromU32(uint32_t val, unsigned int N) {
    std::vector<bool> result(N, false);
    for (unsigned int i = 0; i < N; ++i)
        result[i] = (val >> (N - 1 - i)) & 1u;
    return result;
}

// ── Two's complement negation: ~B + 1 ────────────────────────────────────────
static std::vector<bool> twosComplement(const std::vector<bool>& B) {
    auto notB = bitwiseNOT(B).eval();
    auto one  = fromU32(1u, static_cast<unsigned int>(B.size()));
    return Adder(notB, one).getSum();
}

// ── Shift helpers (index 0 = MSB convention) ──────────────────────────────────

// SLL: logical left shift by n — lower indices shift out, zeros fill high end
static std::vector<bool> sll(const std::vector<bool>& A, unsigned int n) {
    unsigned int N = static_cast<unsigned int>(A.size());
    std::vector<bool> out(N, false);
    // index 0=MSB: left-shifting means the MSB side loses bits, LSB side gains zeros.
    // result[i] = A[i+n]  for i = 0 .. N-n-1
    if (n < N)
        for (unsigned int i = 0; i + n < N; ++i)
            out[i] = A[i + n];
    return out;
}

// SRL: logical right shift by n — zeros fill from MSB side
static std::vector<bool> srl(const std::vector<bool>& A, unsigned int n) {
    unsigned int N = static_cast<unsigned int>(A.size());
    std::vector<bool> out(N, false);
    // result[i] = A[i-n]  for i = n .. N-1
    if (n < N)
        for (unsigned int i = n; i < N; ++i)
            out[i] = A[i - n];
    return out;
}

// SRA: arithmetic right shift by n — sign bit (A[0]) fills from MSB side
static std::vector<bool> sra(const std::vector<bool>& A, unsigned int n) {
    unsigned int N = static_cast<unsigned int>(A.size());
    bool sign = A[0]; // index 0 = MSB = sign bit
    std::vector<bool> out(N, sign);
    if (n < N)
        for (unsigned int i = n; i < N; ++i)
            out[i] = A[i - n];
    return out;
}

// Extract shift amount from lower 5 bits of B (standard MIPS shamt field)
static unsigned int shamtOf(const std::vector<bool>& B) {
    unsigned int N = static_cast<unsigned int>(B.size());
    unsigned int s = 0;
    unsigned int start = (N >= 5) ? N - 5 : 0;
    for (unsigned int i = start; i < N; ++i)
        s = (s << 1) | (B[i] ? 1u : 0u);
    return s & 0x1Fu;
}

// ── Build ALUResult from a result vector ──────────────────────────────────────
static ALUResult makeResult(std::vector<bool> result,
                             bool carry    = false,
                             bool overflow = false) {
    bool zero = true;
    for (bool b : result) if (b) { zero = false; break; }
    bool negative = result[0]; // MSB = sign bit
    return { result, zero, negative, carry, overflow };
}

// ── ALU::execute ──────────────────────────────────────────────────────────────
ALUResult ALU::execute(const std::vector<bool>& A,
                        const std::vector<bool>& B,
                        ALUOp op) {
    unsigned int N = static_cast<unsigned int>(A.size());

    switch (op) {

        // ── ADD ───────────────────────────────────────────────────────────────
        case ALUOp::ADD: {
            auto result   = Adder(A, B).getSum();
            uint64_t full = static_cast<uint64_t>(toU32(A)) + toU32(B);
            bool carry    = (full > 0xFFFFFFFFu);
            // Signed overflow: both same sign, result sign differs
            bool overflow = (toS32(A) > 0 && toS32(B) > 0 && toS32(result) < 0) ||
                            (toS32(A) < 0 && toS32(B) < 0 && toS32(result) > 0);
            return makeResult(result, carry, overflow);
        }

        // ── SUB ───────────────────────────────────────────────────────────────
        case ALUOp::SUB: {
            auto result   = Adder(A, twosComplement(B)).getSum();
            // Carry for SUB = no borrow (A >= B unsigned)
            bool carry    = (toU32(A) >= toU32(B));
            // Signed overflow: opposite signs, result sign ≠ A's sign
            bool overflow = (toS32(A) > 0 && toS32(B) < 0 && toS32(result) < 0) ||
                            (toS32(A) < 0 && toS32(B) > 0 && toS32(result) > 0);
            return makeResult(result, carry, overflow);
        }

        // ── Bitwise ───────────────────────────────────────────────────────────
        case ALUOp::AND:
            return makeResult(bitwiseAND(A, B).eval());

        case ALUOp::OR:
            return makeResult(bitwiseOR(A, B).eval());

        case ALUOp::XOR:
            return makeResult(bitwiseXOR(A, B).eval());

        // NOR = NOT(A OR B)
        case ALUOp::NOR:
            return makeResult(bitwiseNOT(bitwiseOR(A, B).eval()).eval());

        // ── SLT: set to 1 if A < B (signed), else 0 ──────────────────────────
        case ALUOp::SLT: {
            bool less = (toS32(A) < toS32(B));
            return makeResult(fromU32(less ? 1u : 0u, N));
        }

        // ── Shifts ────────────────────────────────────────────────────────────
        case ALUOp::SLL:
            return makeResult(sll(A, shamtOf(B)));

        case ALUOp::SRL:
            return makeResult(srl(A, shamtOf(B)));

        case ALUOp::SRA:
            return makeResult(sra(A, shamtOf(B)));

        default:
            return makeResult(std::vector<bool>(N, false));
    }
}
