#ifndef SHIFTER_H
#define SHIFTER_H

#include <vector>

class LeftShifter {
    public:
        LeftShifter(std::vector<bool> input, int N);
        std::vector<bool> getOutput();
    private:
        std::vector<bool> output;
};

class RightShifter {
    public:
        RightShifter(std::vector<bool> input, int N);
        std::vector<bool> getOutput();
    private:
        std::vector<bool> output;
};

#endif // SHIFTER_H