#include "adders.h"
#include "../gates/gates.h"

// HalfAdder
HalfAdder::HalfAdder(bool inputA, bool inputB)
	: A(inputA), B(inputB) {
	sum = XOR(A, B).eval();
	carryOut = AND(A, B).eval();
}

bool HalfAdder::getSum() {
	return sum;
}

bool HalfAdder::getCarryOut() {
	return carryOut;
}

// FullAdder
FullAdder::FullAdder(bool inputA, bool inputB, bool carryIn)
	: A(inputA), B(inputB), carryIn(carryIn) {
	HalfAdder halfAdder = HalfAdder(A, B);
	HalfAdder halfAdder2 = HalfAdder(halfAdder.getSum(), carryIn);
	sum = halfAdder2.getSum();
	carryOut = OR(halfAdder.getCarryOut(), halfAdder2.getCarryOut()).eval();
}	

bool FullAdder::getSum() {
	return sum;
}

bool FullAdder::getCarryOut() {
	return carryOut;
}

// Adder
Adder::Adder(std::vector<bool> inputA, std::vector<bool> inputB)
	: A(inputA), B(inputB), sum(inputA.size(), 0) {
	int lsb = inputA.size() - 1;
	bool carryIn = 0;
	for (int i = lsb; i >= 0; i--) {
		if (i == lsb) {
			HalfAdder halfAdder = HalfAdder(A[i], B[i]);
			sum[i] = halfAdder.getSum();
			carryIn = halfAdder.getCarryOut();
			continue;
		}
		sum[i] = FullAdder(A[i], B[i], carryIn).getSum();
		carryIn = FullAdder(A[i], B[i], carryIn).getCarryOut();
	}
}

std::vector<bool> Adder::getSum() {
	return sum;
}