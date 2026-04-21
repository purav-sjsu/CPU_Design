#include "mmio.h"
#include "../utils/utils.h"
#include <iostream>

bool isMMIO(unsigned int address) {
    return address == MMIO_OUT || address == MMIO_IN;
}

void mmioWrite(unsigned int address, const std::vector<bool>& value) {
    if (address == MMIO_OUT) {
        char c = static_cast<char>(unsignedBinaryToNum(value));
        std::cout << c << std::flush;
    }
}

std::vector<bool> mmioRead(unsigned int address) {
    if (address == MMIO_IN) {
        char c = '\0';
        std::cin.get(c);
        return num2unsignedBinary(static_cast<int>(static_cast<unsigned char>(c)), WORD_SIZE);
    }
    return std::vector<bool>(WORD_SIZE, false);
}
