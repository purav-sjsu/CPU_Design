#include "flipflop.h"
#include "../gates/gates.h"

DFlipFlop::DFlipFlop(bool D, bool enable) {
	Q = 0;
	update(D, enable);
}

void DFlipFlop::update(bool D, bool enable) {
	bool R = NAND(D, D).eval();
	bool S = D;
	bool set = NAND(S, enable).eval();
	bool reset = NAND(R, enable).eval();

	bool newQ = NAND(set, NOT(Q).eval()).eval();
	bool newNotQ = NAND(reset, Q).eval();
	Q = newQ;
	notQ = newNotQ;
}