#include "flags.h"

FlagsRegister::FlagsRegister() {
    // Automatically initialized by your DFlipFlop constructor
}

void FlagsRegister::update(bool z, bool n, bool c, bool v) {
    // Pass 'true' to the enable pin so the flip-flop accepts the new data
    z_ff.update(z, true);
    n_ff.update(n, true);
    c_ff.update(c, true);
    v_ff.update(v, true);
}

// Fetching the stored states using your getQ() method
bool FlagsRegister::getZ() const { return z_ff.getQ(); }
bool FlagsRegister::getN() const { return n_ff.getQ(); }
bool FlagsRegister::getC() const { return c_ff.getQ(); }
bool FlagsRegister::getV() const { return v_ff.getQ(); }