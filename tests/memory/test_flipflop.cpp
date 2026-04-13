#include <cassert>
#include <iostream>

#include "../../memory/flipflop.h"

// Q should default to 0 on construction
void testInitialState() {
	DFlipFlop ff;
	assert(ff.getQ() == 0);
}

// D=1, enable=1 then Q should latch to 1
void testStoreOneWhenEnabled() {
	DFlipFlop ff;
	ff.update(1, 1);
	assert(ff.getQ() == 1);
}

// Q should follow D through 0->1 and 1->0 transitions
void testTransitions() {
	DFlipFlop ff;

	ff.update(1, 1);
	assert(ff.getQ() == 1);

	ff.update(0, 1);
	assert(ff.getQ() == 0);

	ff.update(1, 1);
	assert(ff.getQ() == 1);
}

// When enable=0, Q must not change regardless of D
void testHoldStateWhenDisabled() {
	DFlipFlop ff;

	ff.update(1, 1);
	assert(ff.getQ() == 1);

	ff.update(0, 0);
	assert(ff.getQ() == 1);

	ff.update(1, 0);
	assert(ff.getQ() == 1);
}

// Writing the same D repeatedly should keep Q stable
void testRepeatedSameValue() {
	DFlipFlop ff;

	ff.update(1, 1);
	ff.update(1, 1);
	ff.update(1, 1);
	assert(ff.getQ() == 1);

	ff.update(0, 1);
	ff.update(0, 1);
	ff.update(0, 1);
	assert(ff.getQ() == 0);
}

int main() {
	std::cout << "Running D flip-flop tests\n";
	std::cout << "(1/5) Testing initial state\n";
	testInitialState();
	std::cout << "(2/5) Testing store 1 when enabled\n";
	testStoreOneWhenEnabled();
	std::cout << "(3/5) Testing 0->1 and 1->0 transitions\n";
	testTransitions();
	std::cout << "(4/5) Testing hold state when disabled\n";
	testHoldStateWhenDisabled();
	std::cout << "(5/5) Testing repeated same value stability\n";
	testRepeatedSameValue();
	std::cout << "All D flip-flop tests passed!\n\n";
	return 0;
}