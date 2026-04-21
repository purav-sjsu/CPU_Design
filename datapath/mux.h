#ifndef MUX_H
#define MUX_H

#include <vector>

class Mux2To1 {
    public:
        Mux2To1(std::vector<bool> inputA, std::vector<bool> inputB, bool sel);
        std::vector<bool> getOutput();
    private:
        std::vector<bool> output;
};

class Mux4To1 {
    public:
        // sel is a 2-bit vector where sel[1] is MSB and sel[0] is LSB
        Mux4To1(std::vector<bool> inputA, std::vector<bool> inputB, 
                std::vector<bool> inputC, std::vector<bool> inputD, 
                std::vector<bool> sel);
        std::vector<bool> getOutput();
    private:
        std::vector<bool> output;
};

#endif // MUX_H