#include "bus.h"

Bus::Bus() : data(WORD_SIZE, false) {}

void Bus::put(const std::vector<bool>& d) {
    data = d;
}

std::vector<bool> Bus::get() const {
    return data;
}

void Bus::reset() {
    data.assign(WORD_SIZE, false);
}
