#include <cassert>
#include <iostream>
#include <vector>

#include "../../datapath/bitwise.h"
#include "../../utils/utils.h"

// Test bitwiseAND
void testBitwiseAND() {
	// 0xFF00 & 0x0F0F = 0x0F00
	auto result = bitwiseAND(num2unsignedBinary(0xFF00, 16), num2unsignedBinary(0x0F0F, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0x0F00);

	// 0xFFFF & 0x0000 = 0x0000
	result = bitwiseAND(num2unsignedBinary(0xFFFF, 16), num2unsignedBinary(0x0000, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0x0000);

	// 0xFFFF & 0xFFFF = 0xFFFF
	result = bitwiseAND(num2unsignedBinary(0xFFFF, 16), num2unsignedBinary(0xFFFF, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0xFFFF);
}

// Test bitwiseOR
void testBitwiseOR() {
	// 0xFF00 | 0x0F0F = 0xFF0F
	auto result = bitwiseOR(num2unsignedBinary(0xFF00, 16), num2unsignedBinary(0x0F0F, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0xFF0F);

	// 0x0000 | 0x0000 = 0x0000
	result = bitwiseOR(num2unsignedBinary(0x0000, 16), num2unsignedBinary(0x0000, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0x0000);

	// 0xAAAA | 0x5555 = 0xFFFF
	result = bitwiseOR(num2unsignedBinary(0xAAAA, 16), num2unsignedBinary(0x5555, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0xFFFF);
}

// Test bitwiseXOR
void testBitwiseXOR() {
	// 0xFF00 ^ 0x0F0F = 0xF00F
	auto result = bitwiseXOR(num2unsignedBinary(0xFF00, 16), num2unsignedBinary(0x0F0F, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0xF00F);

	// 0xFFFF ^ 0xFFFF = 0x0000
	result = bitwiseXOR(num2unsignedBinary(0xFFFF, 16), num2unsignedBinary(0xFFFF, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0x0000);

	// 0xAAAA ^ 0x5555 = 0xFFFF
	result = bitwiseXOR(num2unsignedBinary(0xAAAA, 16), num2unsignedBinary(0x5555, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0xFFFF);
}

// Test bitwiseNOT
void testBitwiseNOT() {
	// ~0x0000 = 0xFFFF
	auto result = bitwiseNOT(num2unsignedBinary(0x0000, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0xFFFF);

	// ~0xFFFF = 0x0000
	result = bitwiseNOT(num2unsignedBinary(0xFFFF, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0x0000);

	// ~0xAAAA = 0x5555
	result = bitwiseNOT(num2unsignedBinary(0xAAAA, 16)).eval();
	assert(unsignedBinaryToNum(result) == 0x5555);
}

int main() {
	std::cout << "Running bitwise tests\n";
	std::cout << "(1/4) Testing bitwiseAND\n";
	testBitwiseAND();
	std::cout << "(2/4) Testing bitwiseOR\n";
	testBitwiseOR();
	std::cout << "(3/4) Testing bitwiseXOR\n";
	testBitwiseXOR();
	std::cout << "(4/4) Testing bitwiseNOT\n";
	testBitwiseNOT();
	std::cout << "All bitwise tests passed!\n\n";
	return 0;
}