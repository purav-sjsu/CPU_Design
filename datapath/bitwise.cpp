#include "bitwise.h"
#include "../gates/gates.h"

// bitwise OR gate
bitwiseOR::bitwiseOR(std::vector<bool> inputA, std::vector<bool> inputB)
	: A(inputA), B(inputB) {
	result.resize(A.size());
	for (int i = A.size() - 1; i >= 0; i--) {
		result[i] = OR(A[i], B[i]).eval();
	}
}

std::vector<bool> bitwiseOR::eval() {
	return result;
}

// bitwise AND gate
bitwiseAND::bitwiseAND(std::vector<bool> inputA, std::vector<bool> inputB)
	: A(inputA), B(inputB) {
	result.resize(A.size());
	for (int i = A.size() - 1; i >= 0; i--) {
		result[i] = AND(A[i], B[i]).eval();
	}
}

std::vector<bool> bitwiseAND::eval() {
	return result;
}

// bitwise NOT gate
bitwiseNOT::bitwiseNOT(std::vector<bool> input)
	: A(input) {
	result.resize(A.size());
	for (int i = A.size() - 1; i >= 0; i--) {
		result[i] = NOT(A[i]).eval();
	}
}

std::vector<bool> bitwiseNOT::eval() {
	return result;
}

// bitwise XOR gate
bitwiseXOR::bitwiseXOR(std::vector<bool> inputA, std::vector<bool> inputB)
	: A(inputA), B(inputB) {
	result.resize(A.size());
	for (int i = A.size() - 1; i >= 0; i--) {
		result[i] = XOR(A[i], B[i]).eval();
	}
}

std::vector<bool> bitwiseXOR::eval() {
	return result;
}