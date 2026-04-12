#ifndef CLOCK_H
#define CLOCK_H

#include <cstdint>

class Clock {
	public:
		Clock(int delayMs = 0);

		void tick();
		void reset();
		uint64_t getCycle() const;
		
	private:
		uint64_t cycle;
		int delayMs;
};

#endif // CLOCK_H