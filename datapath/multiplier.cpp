#include "multiplier.h"
#include "adders.h"
#include "shifter.h"
#include "../utils/utils.h"

// Multiplier (Constructor)
// Multiplying two n-bit numbers can yield a product of up to 2n bits, so we need a 64-bit product for 32-bit inputs.
Multiplier::Multiplier(std::vector<bool> inputA, std::vector<bool> inputB)
    : A(inputA), B(inputB), product(64, 0) {}
    

std::vector<bool> Multiplier::getProduct() {
    std::vector<bool> product(64, false);

    // Zero-extend A (32-bit) into the lower half of a 64-bit vector.
    // index 0 = MSB convention: A[j] (place 2^(31-j)) maps to index 32+j
    // in the 64-bit vector (place 2^(63-(32+j)) = 2^(31-j)). Correct.
    std::vector<bool> A64(64, false);
    for (int j = 0; j < 32; ++j)
        A64[32 + j] = A[j];

    // Shift-and-add: iterate B from LSB (index 31) to MSB (index 0).
    // B[i] = 1 means this bit contributes A × 2^(31-i) to the product.
    // LeftShifter(A64, n) multiplies the 64-bit value by 2^n (shifts toward index 0).
    for (int i = 31; i >= 0; --i) {
        if (B[i]) {
            int shift = 31 - i;
            auto shiftedA = LeftShifter(A64, shift).getOutput();
            product = Adder(product, shiftedA).getSum();
        }
    }
    return product;
}