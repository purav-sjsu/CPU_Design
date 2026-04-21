#include "mux.h"
#include "../gates/gates.h"

// 2-to-1 MUX
Mux2To1::Mux2To1(std::vector<bool> inputA, std::vector<bool> inputB, bool sel) 
    : output(inputA.size()) {
    
    bool notSel = NOT(sel);
    
    for (size_t i = 0; i < inputA.size(); ++i) {
        // Out = (A AND ~sel) OR (B AND sel)
        bool a_routed = AND(inputA[i], notSel);
        bool b_routed = AND(inputB[i], sel);
        output[i] = OR(a_routed, b_routed);
    }
}

std::vector<bool> Mux2To1::getOutput() { return output; }

// 4-to-1 MUX
Mux4To1::Mux4To1(std::vector<bool> A, std::vector<bool> B, 
                 std::vector<bool> C, std::vector<bool> D, 
                 std::vector<bool> sel) {
    
    // Stage 1: Select between A/B and C/D using the LSB of the selector
    Mux2To1 muxAB(A, B, sel[0]);
    Mux2To1 muxCD(C, D, sel[0]);
    
    // Stage 2: Select between the winners using the MSB of the selector
    Mux2To1 muxFinal(muxAB.getOutput(), muxCD.getOutput(), sel[1]);
    
    output = muxFinal.getOutput();
}

std::vector<bool> Mux4To1::getOutput() { return output; }