#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <stdexcept>

std::vector<bool> num2unsignedBinary(int value, int size);
std::vector<bool> num2signedBinary(int value, int size);
int unsignedBinaryToNum(std::vector<bool> value);
int signedBinaryToNum(std::vector<bool> value);

#endif