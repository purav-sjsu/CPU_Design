#include <cassert>
#include <iostream>
#include <vector>

#include "../../datapath/regfile.h"
#include "../../utils/utils.h"

// All registers should read 0 after construction
void testInitialState() {
	RegFile rf(8, 16);
	std::vector<bool> zero(16, 0);
	for (unsigned int i = 0; i < 8; i++) {
		assert(rf.read(i) == zero);
	}
}

// Write a value to a register and read it back
void testWriteAndRead() {
	RegFile rf(8, 16);
	std::vector<bool> val = num2unsignedBinary(42, 16);
	rf.write(0, val);
	assert(rf.read(0) == val);
}

// Write to multiple registers and verify each retains its value
void testMultipleRegisters() {
	RegFile rf(8, 16);
	std::vector<bool> a = num2unsignedBinary(100, 16);
	std::vector<bool> b = num2unsignedBinary(200, 16);
	std::vector<bool> c = num2unsignedBinary(65535, 16);

	rf.write(0, a);
	rf.write(4, b);
	rf.write(7, c);

	assert(rf.read(0) == a);
	assert(rf.read(4) == b);
	assert(rf.read(7) == c);
}

// Overwriting a register replaces the old value
void testOverwrite() {
	RegFile rf(8, 16);
	rf.write(3, num2unsignedBinary(1000, 16));
	std::vector<bool> val2 = num2unsignedBinary(2000, 16);
	rf.write(3, val2);
	assert(rf.read(3) == val2);
}

// Writing to one register should not affect others
void testIsolation() {
	RegFile rf(8, 16);
	std::vector<bool> zero(16, 0);
	std::vector<bool> val = num2unsignedBinary(0xBEEF, 16);

	rf.write(2, val);

	for (unsigned int i = 0; i < 8; i++) {
		if (i == 2) {
			assert(rf.read(i) == val);
		} else {
			assert(rf.read(i) == zero);
		}
	}
}

// Store all 1s and all 0s
void testAllOnesAndZeros() {
	RegFile rf(8, 16);

	std::vector<bool> allOnes(16, 1);
	rf.write(0, allOnes);
	assert(rf.read(0) == allOnes);

	std::vector<bool> allZeros(16, 0);
	rf.write(0, allZeros);
	assert(rf.read(0) == allZeros);
}

// Fill every register and verify all values
void testFillAllRegisters() {
	RegFile rf(8, 16);
	std::vector<std::vector<bool>> values(8);

	for (unsigned int i = 0; i < 8; i++) {
		values[i] = num2unsignedBinary((i + 1) * 1000, 16);
		rf.write(i, values[i]);
	}

	for (unsigned int i = 0; i < 8; i++) {
		assert(rf.read(i) == values[i]);
	}
}

int main() {
	std::cout << "Running RegFile tests\n";
	std::cout << "(1/7) Testing initial state\n";
	testInitialState();
	std::cout << "(2/7) Testing write and read\n";
	testWriteAndRead();
	std::cout << "(3/7) Testing multiple registers\n";
	testMultipleRegisters();
	std::cout << "(4/7) Testing overwrite\n";
	testOverwrite();
	std::cout << "(5/7) Testing register isolation\n";
	testIsolation();
	std::cout << "(6/7) Testing all 1s and all 0s\n";
	testAllOnesAndZeros();
	std::cout << "(7/7) Testing fill all registers\n";
	testFillAllRegisters();
	std::cout << "All RegFile tests passed!\n";
	return 0;
}