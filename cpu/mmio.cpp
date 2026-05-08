#include "mmio.h"
#include "../utils/utils.h"
#include <iostream>
#include <cstdint>

static bool     s_hasInputWord = false;
static uint32_t s_inputWord    = 0;

void setMMIOInputWord(uint32_t val) {
    s_hasInputWord = true;
    s_inputWord    = val;
}

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
        if (s_hasInputWord) {
            s_hasInputWord = false;
            return num2unsignedBinary(static_cast<int>(s_inputWord), WORD_SIZE);
        }
        char c = '\0';
        std::cin.get(c);
        return num2unsignedBinary(static_cast<int>(static_cast<unsigned char>(c)), WORD_SIZE);
    }
    return std::vector<bool>(WORD_SIZE, false);
}
