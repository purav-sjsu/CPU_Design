#include "gates.h"

// OR gate
OR::OR(bool inputA, bool inputB)
	: A(inputA), B(inputB) {
	result = A || B;
}

bool OR::eval() {
	return result;
}

// AND gate
AND::AND(bool inputA, bool inputB)
	: A(inputA), B(inputB) {
	result = A && B;
}

bool AND::eval() {
	return result;
}

// NOT gate
NOT::NOT(bool input)
	: A(input) {
	result = !A;
}

bool NOT::eval() {
	return result;
}

// XOR gate
XOR::XOR(bool inputA, bool inputB)
	: A(inputA), B(inputB) {
	result = A ^ B;
}

bool XOR::eval() {
	return result;
}

// NAND gate
NAND::NAND(bool inputA, bool inputB)
	: A(inputA), B(inputB) {
	result = NOT(AND(A, B).eval()).eval();
}

bool NAND::eval() {
	return result;
}