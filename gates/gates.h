#ifndef GATES_H
#define GATES_H

#include <vector>

class OR {
	public:
		OR(bool inputA, bool inputB);
		bool eval();
	private:
		bool A;
		bool B;
		bool result;
};

class AND {
	public:
		AND(bool inputA, bool inputB);
		bool eval();
	private:
		bool A;
		bool B;
		bool result;
};

class NOT {
	public:
		NOT(bool input);
		bool eval();
	private:
		bool A;
		bool result;
};

class XOR {
	public:
		XOR(bool inputA, bool inputB);
		bool eval();
	private:
		bool A;
		bool B;
		bool result;
};

class NAND {
	public:
		NAND(bool inputA, bool inputB);
		bool eval();
	private:
		bool A;
		bool B;
		bool result;
};

#endif // GATES_H