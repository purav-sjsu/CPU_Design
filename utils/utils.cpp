#include "utils.h"
#include <climits>
#include <string>
#include <stdexcept>
// converts a non-negative integer to an unsigned binary vector
// valid range: 0 to (2^size - 1)
std::vector<bool> num2unsignedBinary(int value, int size) {
	// When size == 32, (1 << 32) is UB and wraps to 0; cap at INT_MAX instead
	int maxVal = (size >= 32) ? INT_MAX : (1 << size) - 1;
	if (value < 0 || value > maxVal)
		throw std::out_of_range("value " + std::to_string(value) + " cannot be represented in " + std::to_string(size) + " unsigned bits");

	std::vector<bool> result(size, 0);
	for (int i = 0; i < size; i++) {
		int bitPosition = size - 1 - i;
		result[i] = (value >> bitPosition) & 1;
	}
	return result;
}

// converts an integer to a signed binary vector (two's complement)
// valid range: -(2^(size-1)) to (2^(size-1) - 1)
std::vector<bool> num2signedBinary(int value, int size) {
	int minVal = -(1 << (size - 1));
	int maxVal =  (1 << (size - 1)) - 1;
	if (value < minVal || value > maxVal)
		throw std::out_of_range("value " + std::to_string(value) + " cannot be represented in " + std::to_string(size) + " signed bits");

	std::vector<bool> result(size, 0);
	unsigned int bits = (unsigned int)value;
	for (int i = 0; i < size; i++) {
		int bitPosition = size - 1 - i;
		result[i] = (bits >> bitPosition) & 1;
	}
	return result;
}

// converts an unsigned binary vector to a non-negative integer
// each bit contributes its positional value (2^i)
int unsignedBinaryToNum(std::vector<bool> value) {
	int size = value.size();
	int result = 0;
	for (int i = 0; i < size; i++) {
		result += value[i] * (1 << (size - 1 - i));
	}
	return result;
}

// converts a signed binary vector to an integer (two's complement)
// if negative: flip all bits, add 1 to get magnitude, then negate
int signedBinaryToNum(std::vector<bool> value) {
	int size = value.size();
	bool isNegative = value[0];

	if (!isNegative) return unsignedBinaryToNum(value);

	// flip all the bits
	std::vector<bool> flipped(size);
	for (int i = 0; i < size; i++)
		flipped[i] = !value[i];

	// flip the bits, add 1 to get the magnitude then negate
	return -(unsignedBinaryToNum(flipped) + 1);
}
