#include <cassert>
#include <iostream>
#include <vector>

#include "../../datapath/shifter.h"
#include "../../utils/utils.h"

void testLeftShifter() {
    // Shift 1 left by 1 -> 2
    LeftShifter ls1(num2unsignedBinary(1, 16), 1);
    assert(unsignedBinaryToNum(ls1.getOutput()) == 2);

    // Shift 5 left by 2 -> 20
    LeftShifter ls2(num2unsignedBinary(5, 16), 2);
    assert(unsignedBinaryToNum(ls2.getOutput()) == 20);
}

void testRightShifter() {
    // Shift 2 right by 1 -> 1
    RightShifter rs1(num2unsignedBinary(2, 16), 1);
    assert(unsignedBinaryToNum(rs1.getOutput()) == 1);

    // Shift 20 right by 2 -> 5
    RightShifter rs2(num2unsignedBinary(20, 16), 2);
    assert(unsignedBinaryToNum(rs2.getOutput()) == 5);
}

int main() {
    std::cout << "Running shifter tests\n";
    std::cout << "(1/2) Testing left shifter\n";
    testLeftShifter();
    std::cout << "(2/2) Testing right shifter\n";
    testRightShifter();
    std::cout << "All shifter tests passed!\n\n";
    return 0;
}