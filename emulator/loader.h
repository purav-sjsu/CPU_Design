#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>
#include <cstdint>

// Read a big-endian binary file produced by the assembler.
// Returns one uint32_t per 4-byte word.
std::vector<uint32_t> loadBinary(const std::string& path);

// Convert a uint32_t word to a 32-element vector<bool> (MSB at index 0).
std::vector<bool> wordToBits(uint32_t word);

#endif // LOADER_H
