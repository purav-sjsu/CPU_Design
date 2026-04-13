#include <cassert>
#include <iostream>

#include "../gates/gates.h"

// Test AND gate

void testAndGate() {
	assert(AND(0, 0).eval() == false);
	assert(AND(0, 1).eval() == false);
	assert(AND(1, 0).eval() == false);
	assert(AND(1, 1).eval() == true);
}

// Test OR gate
void testOrGate() {
	assert(OR(0, 0).eval() == 0);
	assert(OR(0, 1).eval() == 1);
	assert(OR(1, 0).eval() == 1);
	assert(OR(1, 1).eval() == 1);
}

// Test XOR gate
void testXorGate() {
	assert(XOR(0, 0).eval() == 0);
	assert(XOR(0, 1).eval() == 1);
	assert(XOR(1, 0).eval() == 1);
	assert(XOR(1, 1).eval() == 0);
}

// Test NOT gate
void testNotGate() {
	assert(NOT(0).eval() == 1);
	assert(NOT(1).eval() == 0);
}

// Test NAND gate
void testNandGate() {
	assert(NAND(0, 0).eval() == 1);
	assert(NAND(0, 1).eval() == 1);
	assert(NAND(1, 0).eval() == 1);
	assert(NAND(1, 1).eval() == 0);
}

int main() {
	std::cout << "Running gate tests\n";
	std::cout << "(1/4) Testing AND gate\n";
	testAndGate();
	std::cout << "(2/4) Testing OR gate\n";
	testOrGate();
	std::cout << "(3/4) Testing XOR gate\n";
	testXorGate();
	std::cout << "(4/4) Testing NOT gate\n";
	testNotGate();
	std::cout << "All gate tests passed!\n\n";
	std::cout << "(5/5) Testing NAND gate\n";
	testNandGate();
	std::cout << "All gate tests passed!\n\n";
	return 0;
}