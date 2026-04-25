#ifndef REGFILE_H
#define REGFILE_H

#include <vector>
#include "../memory/register.h"

class RegFile {
	public:
		RegFile(unsigned int numRegisters = 8, unsigned int wordSize = 32);
		void write(unsigned int address, const std::vector<bool>& value);
		std::vector<bool> read(unsigned int address) const;

	private:
		std::vector<Register> registers;
		unsigned int numRegisters;
		unsigned int wordSize;
};

#endif // REGFILE_H