#ifndef ADDERS_H
#define ADDERS_H

#include <vector>

class HalfAdder {
	public:
		HalfAdder(bool inputA, bool inputB);
		bool getSum();
		bool getCarryOut();
	private:
		bool A;
		bool B;
		bool sum;
		bool carryOut;
};

class FullAdder {
	public:
		FullAdder(bool inputA, bool inputB, bool carryIn);
		bool getSum();
		bool getCarryOut();
	private:
		bool A;
		bool B;
		bool carryIn;
		bool sum;
		bool carryOut;
};

// n-bit adder
//rightmost vector element is LSB
class Adder {
	public:
        // Modified to accept an optional carryIn (defaults to 0 so existing code doesn't break)
		Adder(std::vector<bool> inputA, std::vector<bool> inputB, bool carryIn = false);
		std::vector<bool> getSum();
	private:
		std::vector<bool> A;
		std::vector<bool> B;
		std::vector<bool> sum;
};

// New Subtractor Class
class Subtractor {
    public:
        Subtractor(std::vector<bool> inputA, std::vector<bool> inputB);
        std::vector<bool> getDifference();
    private:
        std::vector<bool> difference;
};
#endif // ADDERS_H