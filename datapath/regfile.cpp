#include "regfile.h"

RegFile::RegFile(unsigned int numRegisters, unsigned int wordSize)
	: numRegisters(numRegisters), wordSize(wordSize) {
	registers.resize(numRegisters);
	for (unsigned int i = 0; i < numRegisters; i++) {
		registers[i] = Register(wordSize);
	}
}

void RegFile::write(unsigned int address, const std::vector<bool>& value) {
	registers[address].write(value);
}

std::vector<bool> RegFile::read(unsigned int address) const {
	return registers[address].read();
}