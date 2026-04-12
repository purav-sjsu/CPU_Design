#include "clock.h"
#include <thread>
#include <chrono>

Clock::Clock(int delayMs) : cycle(0), delayMs(delayMs) {}

void Clock::tick() {
	cycle++;

	if (delayMs > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
	}
}

void Clock::reset() {	
	cycle = 0;
}

uint64_t Clock::getCycle() const {
	return cycle;
}