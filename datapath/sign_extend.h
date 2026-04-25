#ifndef SIGN_EXTEND_H
#define SIGN_EXTEND_H

#include <vector>

// Replicate the MSB of val into the upper (targetSize - val.size()) positions.
std::vector<bool> signExtend(const std::vector<bool>& val, unsigned int targetSize);

// Fill the upper bits with 0 instead of the sign bit.
std::vector<bool> zeroExtend(const std::vector<bool>& val, unsigned int targetSize);

#endif // SIGN_EXTEND_H
