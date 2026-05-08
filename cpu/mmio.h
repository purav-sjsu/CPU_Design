#ifndef MMIO_H
#define MMIO_H

#include <vector>
#include <cstdint>
#include "../cpu/config.h"

// Returns true if address is a memory-mapped I/O address.
bool isMMIO(unsigned int address);

// Write a word to an MMIO address (MMIO_OUT → print char to stdout).
void mmioWrite(unsigned int address, const std::vector<bool>& value);

// Read a word from an MMIO address (MMIO_IN → read char from stdin).
std::vector<bool> mmioRead(unsigned int address);

// Pre-load a full 32-bit value to be returned by the next MMIO_IN read.
void setMMIOInputWord(uint32_t val);

#endif // MMIO_H
