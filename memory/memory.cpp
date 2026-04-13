#include "memory.h"
#include <stdexcept>

Memory::Memory(unsigned int numWords, unsigned int wordSize)
	: numWords(numWords), wordSize(wordSize) {
	memory.resize(numWords);
	for (unsigned int i = 0; i < numWords; i++) {
		memory[i] = Register(wordSize);
	}
}

void Memory::write(unsigned int address, const std::vector<bool>& value) {
	if (!checkAddress(address)) {
		throw std::out_of_range("Segmentation fault: Address out of bounds");
	}
	memory[address].write(value);
}

std::vector<bool> Memory::read(unsigned int address) const {
	if (!checkAddress(address)) {
		throw std::out_of_range("Segmentation fault: Address out of bounds");
	}
	return memory[address].read();
}

bool Memory::checkAddress(unsigned int address) const {
	return address < numWords;
}

unsigned int Memory::getMemorySize() const {
	return numWords * wordSize / 8;
}

void Memory::reset() {
	for (unsigned int i = 0; i < numWords; i++) {
		memory[i].reset();
	}
}