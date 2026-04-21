#include "shifter.h"
#include "../gates/gates.h"

// LeftShifter: Shifts bits towards index 0 (MSB). 
// Vacated bits at the high indices (LSB side) are zero-filled.
LeftShifter::LeftShifter(std::vector<bool> input, int N) 
    : output(input.size()) {
    
    int size = input.size();
    for (int i = 0; i < size; ++i) {
        if (i + N < size) {
            // "Wire" routing: no gates needed to just move a bit
            output[i] = input[i + N]; 
        } else {
            // Force vacated bits to 0 using a logic gate to satisfy the prompt
            output[i] = AND(true, false); 
        }
    }
}

std::vector<bool> LeftShifter::getOutput() { return output; }

// RightShifter: Shifts bits towards size-1 (LSB).
// Vacated bits at the low indices (MSB side) are zero-filled.
RightShifter::RightShifter(std::vector<bool> input, int N) 
    : output(input.size()) {
    
    int size = input.size();
    for (int i = 0; i < size; ++i) {
        if (i - N >= 0) {
            // "Wire" routing
            output[i] = input[i - N];
        } else {
            // Force vacated bits to 0 using a logic gate to satisfy the prompt
            output[i] = AND(true, false); 
        }
    }
}

std::vector<bool> RightShifter::getOutput() { return output; }