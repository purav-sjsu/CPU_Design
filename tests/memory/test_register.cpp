#include <cassert>
#include <iostream>
#include <vector>

#include "../../memory/register.h"
#include "../../utils/utils.h"

// All bits should be 0 after construction
void testInitialState() {
	Register reg;
	std::vector<bool> expected(16, 0);
	assert(reg.getData() == expected);
}

// Store a value and read it back
void testStoreAndLoad() {
	Register reg;
	std::vector<bool> val = num2unsignedBinary(42, 16);
	reg.setData(val);
	assert(reg.getData() == val);
}

// Overwrite with a new value
void testOverwrite() {
	Register reg;
	reg.setData(num2unsignedBinary(100, 16));
	std::vector<bool> val2 = num2unsignedBinary(200, 16);
	reg.setData(val2);
	assert(reg.getData() == val2);
}

// Store all 1s and all 0s
void testAllOnesAndZeros() {
	Register reg;

	std::vector<bool> allOnes(16, 1);
	reg.setData(allOnes);
	assert(reg.getData() == allOnes);
	
	std::vector<bool> allZeros(16, 0);
	reg.setData(allZeros);
	assert(reg.getData() == allZeros);
}

int main() {
	std::cout << "Running register tests\n";
	std::cout << "(1/4) Testing initial state\n";
	testInitialState();
	std::cout << "(2/4) Testing store and load\n";
	testStoreAndLoad();
	std::cout << "(3/4) Testing overwrite\n";
	testOverwrite();
	std::cout << "(4/4) Testing all 1s and all 0s\n";
	testAllOnesAndZeros();
	std::cout << "All register tests passed!\n\n";
	return 0;
}
