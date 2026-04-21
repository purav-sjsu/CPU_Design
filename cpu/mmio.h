#ifndef MMIO_H
#define MMIO_H

#include <vector>
#include "../cpu/config.h"

// Returns true if address is a memory-mapped I/O address.
bool isMMIO(unsigned int address);

// Write a word to an MMIO address (MMIO_OUT → print char to stdout).
void mmioWrite(unsigned int address, const std::vector<bool>& value);

// Read a word from an MMIO address (MMIO_IN → read char from stdin).
std::vector<bool> mmioRead(unsigned int address);

#endif // MMIO_H
