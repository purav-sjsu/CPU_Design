#include "register.h"

Register::Register(unsigned int size) 
	: size(size) {
	data.resize(size);
	for (unsigned int i = 0; i < size; i++) {
		data[i] = DFlipFlop();
	}
}

void Register::setData(std::vector<bool> value) {
	for (unsigned int i = 0; i < size; i++) {
		data[i].update(value[i], 1);
	}
}

std::vector<bool> Register::getData() const {
	std::vector<bool> result(size);
	for (unsigned int i = 0; i < size; i++) {
		result[i] = data[i].getQ();
	}
	return result;
}