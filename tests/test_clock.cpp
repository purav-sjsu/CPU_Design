#include <cassert>
#include <iostream>

#include "../clock/clock.h"

// Test clock starts at 0
void testClockStartsAtZero() {
	assert(Clock().getCycle() == 0);
}

// Test clock tick
void testClockTick() {
	Clock clock;
	clock.tick();
	assert(clock.getCycle() == 1);

	clock.tick();
	assert(clock.getCycle() == 2);
}

// Test clock reset
void testClockReset() {
	Clock clock;
	clock.tick();
	clock.tick();
	assert(clock.getCycle() == 2);
	clock.reset();
	assert(clock.getCycle() == 0);
	clock.tick();
	assert(clock.getCycle() == 1);
}

int main() {
	std::cout << "Running clock tests\n";
	std::cout << "(1/3) Testing initial cycle\n";
	testClockStartsAtZero();
	std::cout << "(2/3) Testing tick\n";
	testClockTick();
	std::cout << "(3/3) Testing reset\n";
	testClockReset();
	std::cout << "All clock tests passed!\n\n";
	return 0;
}