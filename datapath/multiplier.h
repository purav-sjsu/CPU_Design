#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include <vector>


// n-bit multiplier: the results
//rightmost vector element is LSB
class Multiplier {
	public:
        // Modified to accept an optional carryIn (defaults to 0 so existing code doesn't break)
		Multiplier(std::vector<bool> inputA, std::vector<bool> inputB);
		std::vector<bool> getProduct();
	private:
		std::vector<bool> A;
		std::vector<bool> B;
        std::vector<bool> product; // 64-bit product of A and B
};


#endif // MULTIPLIER_H