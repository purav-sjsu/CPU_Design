#include <cassert>
#include <iostream>
#include <vector>

#include "../../datapath/mux.h"
#include "../../utils/utils.h"

void testMux2To1() {
    std::vector<bool> A = num2unsignedBinary(10, 16);
    std::vector<bool> B = num2unsignedBinary(20, 16);

    assert(unsignedBinaryToNum(mux2(A, B, false)) == 10);
    assert(unsignedBinaryToNum(mux2(A, B, true))  == 20);
}

void testMux4To1() {
    std::vector<bool> A = num2unsignedBinary(10, 16);
    std::vector<bool> B = num2unsignedBinary(20, 16);
    std::vector<bool> C = num2unsignedBinary(30, 16);
    std::vector<bool> D = num2unsignedBinary(40, 16);

    assert(unsignedBinaryToNum(mux4(A, B, C, D, 0)) == 10);
    assert(unsignedBinaryToNum(mux4(A, B, C, D, 1)) == 20);
    assert(unsignedBinaryToNum(mux4(A, B, C, D, 2)) == 30);
    assert(unsignedBinaryToNum(mux4(A, B, C, D, 3)) == 40);
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
