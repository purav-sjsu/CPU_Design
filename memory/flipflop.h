#ifndef FLIPFLOP_H
#define FLIPFLOP_H


// D Flip-Flop
class DFlipFlop {
	public:
		DFlipFlop();
		void update(bool D, bool enable);
		bool getQ() const;
	private:
		bool Q; // Output
		bool notQ; // Inverted Output
};

#endif // FLIPFLOP_H