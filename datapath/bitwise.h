#ifndef BITWISE_H
#define BITWISE_H

#include <vector>

class bitwiseOR {
	public:
		bitwiseOR(std::vector<bool> inputA, std::vector<bool> inputB);
		std::vector<bool> eval();
	private:
		std::vector<bool> A;
		std::vector<bool> B;
		std::vector<bool> result;
};

class bitwiseAND {
	public:
		bitwiseAND(std::vector<bool> inputA, std::vector<bool> inputB);
		std::vector<bool> eval();
	private:
		std::vector<bool> A;
		std::vector<bool> B;
		std::vector<bool> result;
};

class bitwiseNOT {
	public:
		bitwiseNOT(std::vector<bool> input);
		std::vector<bool> eval();
	private:
		std::vector<bool> A;
		std::vector<bool> result;
};

class bitwiseXOR {
	public:
		bitwiseXOR(std::vector<bool> inputA, std::vector<bool> inputB);
		std::vector<bool> eval();
	private:
		std::vector<bool> A;
		std::vector<bool> B;
		std::vector<bool> result;
};

#endif // BITWISE_H