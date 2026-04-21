#include <cassert>
#include <iostream>

#include "../../datapath/flags.h"

void testFlagsRegister() {
    FlagsRegister flags;

    // Verify initial state is empty/false
    assert(flags.getZ() == false);
    assert(flags.getN() == false);
    assert(flags.getC() == false);
    assert(flags.getV() == false);

    // Push new flag states (Z=1, N=0, C=1, V=0)
    flags.update(true, false, true, false);
    assert(flags.getZ() == true);
    assert(flags.getN() == false);
    assert(flags.getC() == true);
    assert(flags.getV() == false);

    // Push new flag states (Z=0, N=1, C=0, V=1)
    flags.update(false, true, false, true);
    assert(flags.getZ() == false);
    assert(flags.getN() == true);
    assert(flags.getC() == false);
    assert(flags.getV() == true);
}

int main() {
    std::cout << "Running flags register tests\n";
    testFlagsRegister();
    std::cout << "All flags register tests passed!\n\n";
    return 0;
}