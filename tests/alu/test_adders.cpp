#include <cassert>
#include <iostream>
#include <vector>

#include "../../datapath/adders.h"
#include "../../utils/utils.h"

// Test HalfAdder
void testHalfAdder() {
	HalfAdder h00(0, 0);
	assert(h00.getSum() == 0);
	assert(h00.getCarryOut() == 0);

	HalfAdder h01(0, 1);
	assert(h01.getSum() == 1);
	assert(h01.getCarryOut() == 0);

	HalfAdder h10(1, 0);
	assert(h10.getSum() == 1);
	assert(h10.getCarryOut() == 0);

	HalfAdder h11(1, 1);
	assert(h11.getSum() == 0);
	assert(h11.getCarryOut() == 1);
}

// Test FullAdder
void testFullAdder() {
	FullAdder f000(0, 0, 0);
	assert(f000.getSum() == 0);
	assert(f000.getCarryOut() == 0);

	FullAdder f010(0, 1, 0);
	assert(f010.getSum() == 1);
	assert(f010.getCarryOut() == 0);

	FullAdder f100(1, 0, 0);
	assert(f100.getSum() == 1);
	assert(f100.getCarryOut() == 0);

	FullAdder f110(1, 1, 0);
	assert(f110.getSum() == 0);
	assert(f110.getCarryOut() == 1);

	FullAdder f111(1, 1, 1);
	assert(f111.getSum() == 1);
	assert(f111.getCarryOut() == 1);
}

// Test 16-bit adder
void testAdder() {
	// 0 + 0 = 0
	Adder a0(num2unsignedBinary(0, 16), num2unsignedBinary(0, 16));
	assert(unsignedBinaryToNum(a0.getSum()) == 0);

	// 1 + 1 = 2
	Adder a1(num2unsignedBinary(1, 16), num2unsignedBinary(1, 16));
	assert(unsignedBinaryToNum(a1.getSum()) == 2);

	// 3 + 5 = 8
	Adder a2(num2unsignedBinary(3, 16), num2unsignedBinary(5, 16));
	assert(unsignedBinaryToNum(a2.getSum()) == 8);

	// 15 + 1 = 16
	Adder a3(num2unsignedBinary(15, 16), num2unsignedBinary(1, 16));
	assert(unsignedBinaryToNum(a3.getSum()) == 16);

	// 65535 + 1 => overflow
	Adder a4(num2unsignedBinary(65535, 16), num2unsignedBinary(1, 16));
	assert(unsignedBinaryToNum(a4.getSum()) == 0);

	// 100 + 200 = 300
	Adder a5(num2unsignedBinary(100, 16), num2unsignedBinary(200, 16));
	assert(unsignedBinaryToNum(a5.getSum()) == 300);

	// 32768 + 32768 => overflow
	Adder a6(num2unsignedBinary(32768, 16), num2unsignedBinary(32768, 16));
	assert(unsignedBinaryToNum(a6.getSum()) == 0);
}

// Test 16-bit subtractor
void testSubtractor() {
	// 5 - 3 = 2
	Subtractor s0(num2unsignedBinary(5, 16), num2unsignedBinary(3, 16));
	assert(unsignedBinaryToNum(s0.getDifference()) == 2);

	// 10 - 10 = 0
	Subtractor s1(num2unsignedBinary(10, 16), num2unsignedBinary(10, 16));
	assert(unsignedBinaryToNum(s1.getDifference()) == 0);

	// 3 - 5 = -2 (In 16-bit two's complement, -2 represents as 65534 unsigned)
	Subtractor s2(num2unsignedBinary(3, 16), num2unsignedBinary(5, 16));
	assert(unsignedBinaryToNum(s2.getDifference()) == 65534);
}

int main() {
	std::cout << "Running adder tests\n";
	std::cout << "(1/3) Testing half adder\n";
	testHalfAdder();
	std::cout << "(2/3) Testing full adder\n";
	testFullAdder();
	std::cout << "(3/3) Testing adder\n";
	testAdder();
	std::cout << "(4/4) Testing subtractor\n";
	testSubtractor();
	std::cout << "All adder tests passed!\n\n";
	return 0;
}
