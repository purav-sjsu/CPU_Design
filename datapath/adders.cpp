#include "adders.h"
#include "../gates/gates.h"

// HalfAdder
HalfAdder::HalfAdder(bool inputA, bool inputB)
	: A(inputA), B(inputB) {
	sum = XOR(A, B);
	carryOut = AND(A, B);
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
	carryOut = OR(halfAdder.getCarryOut(), halfAdder2.getCarryOut());
}	

bool FullAdder::getSum() {
	return sum;
}

bool FullAdder::getCarryOut() {
	return carryOut;
}

// Adder (Modified)
Adder::Adder(std::vector<bool> inputA, std::vector<bool> inputB, bool initialCarryIn)
	: A(inputA), B(inputB), sum(inputA.size(), 0) {
	int lsb = inputA.size() - 1;
	bool currentCarry = initialCarryIn; // Start with our input carry
    
	for (int i = lsb; i >= 0; i--) {
        // We can just use a FullAdder for every bit now, bridging the initial carry-in
        FullAdder fa(A[i], B[i], currentCarry);
        sum[i] = fa.getSum();
        currentCarry = fa.getCarryOut();
	}
}

std::vector<bool> Adder::getSum() {
	return sum;
}

// Subtractor (New)
Subtractor::Subtractor(std::vector<bool> inputA, std::vector<bool> inputB) {
    std::vector<bool> notB(inputB.size());
    
    // Invert B with bitwise NOT
    for (size_t i = 0; i < inputB.size(); ++i) {
        notB[i] = NOT(inputB[i]); 
    }

    // Add via existing Adder with carry-in forced to 1 (A + ~B + 1)
    Adder add(inputA, notB, true); 
    difference = add.getSum();
}

std::vector<bool> Subtractor::getDifference() {
    return difference;
}