#include <cassert>
#include <iostream>
#include <vector>

#include "../../datapath/mux.h"
#include "../../utils/utils.h"

void testMux2To1() {
    std::vector<bool> A = num2unsignedBinary(10, 16);
    std::vector<bool> B = num2unsignedBinary(20, 16);

    // Select A
    Mux2To1 mux0(A, B, false);
    assert(unsignedBinaryToNum(mux0.getOutput()) == 10);

    // Select B
    Mux2To1 mux1(A, B, true);
    assert(unsignedBinaryToNum(mux1.getOutput()) == 20);
}

void testMux4To1() {
    std::vector<bool> A = num2unsignedBinary(10, 16);
    std::vector<bool> B = num2unsignedBinary(20, 16);
    std::vector<bool> C = num2unsignedBinary(30, 16);
    std::vector<bool> D = num2unsignedBinary(40, 16);

    // sel = 00 -> A
    Mux4To1 mux0(A, B, C, D, {false, false});
    assert(unsignedBinaryToNum(mux0.getOutput()) == 10);

    // sel = 01 -> B (assuming sel[0] is LSB)
    Mux4To1 mux1(A, B, C, D, {true, false});
    assert(unsignedBinaryToNum(mux1.getOutput()) == 20);

    // sel = 10 -> C
    Mux4To1 mux2(A, B, C, D, {false, true});
    assert(unsignedBinaryToNum(mux2.getOutput()) == 30);

    // sel = 11 -> D
    Mux4To1 mux3(A, B, C, D, {true, true});
    assert(unsignedBinaryToNum(mux3.getOutput()) == 40);
}

int main() {
    std::cout << "Running multiplexer tests\n";
    std::cout << "(1/2) Testing 2-to-1 MUX\n";
    testMux2To1();
    std::cout << "(2/2) Testing 4-to-1 MUX\n";
    testMux4To1();
    std::cout << "All multiplexer tests passed!\n\n";
    return 0;
}