#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include "flipflop.h"

class Register {
	public:
		Register(unsigned int size = 16);
		void setData(std::vector<bool> value);
		std::vector<bool> getData() const;
	private:
		std::vector<DFlipFlop> data;
	 	unsigned int size;
};

#endif // REGISTER_H