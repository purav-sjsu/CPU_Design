#include <cassert>
#include <iostream>
#include <vector>

#include "../utils/utils.h"

// Test out-of-range errors
void testEdgeCases() {
	// unsigned: negative value
	bool threw = false;
	try { num2unsignedBinary(-1, 16); } catch (std::out_of_range&) { threw = true; }
	assert(threw);

	// unsigned: too large
	threw = false;
	try { num2unsignedBinary(65536, 16); } catch (std::out_of_range&) { threw = true; }
	assert(threw);

	// signed: too large
	threw = false;
	try { num2signedBinary(32768, 16); } catch (std::out_of_range&) { threw = true; }
	assert(threw);

	// signed: too small
	threw = false;
	try { num2signedBinary(-32769, 16); } catch (std::out_of_range&) { threw = true; }
	assert(threw);
}

// Test round-trip: num -> binary -> num for every 16-bit value
void testRoundTrip() {
	// unsigned 16-bit round-trips (0 to 65535)
	for (int i = 0; i <= 65535; i++) {
		assert(unsignedBinaryToNum(num2unsignedBinary(i, 16)) == i);
	}

	// signed 16-bit round-trips (-32768 to 32767)
	for (int i = -32768; i <= 32767; i++) {
		assert(signedBinaryToNum(num2signedBinary(i, 16)) == i);
	}
}

int main() {
	std::cout << "Running utils tests\n";
	std::cout << "(1/2) Testing edge cases\n";
	testEdgeCases();
	std::cout << "(2/2) Testing 16-bit round-trips\n";
	testRoundTrip();
	std::cout << "All utils tests passed!\n\n";
	return 0;
}
