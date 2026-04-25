#include "loader.h"
#include <fstream>
#include <stdexcept>


// Read a big-endian binary file produced by the assembler.
// Returns one uint32_t per 4-byte word.
std::vector<uint32_t> loadBinary(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open: " + path);

    std::vector<uint32_t> words;
    uint8_t buf[4];
    while (f.read(reinterpret_cast<char*>(buf), 4)) {
        uint32_t w = (static_cast<uint32_t>(buf[0]) << 24) |
                     (static_cast<uint32_t>(buf[1]) << 16) |
                     (static_cast<uint32_t>(buf[2]) <<  8) |
                      static_cast<uint32_t>(buf[3]);
        words.push_back(w);
    }
    return words;
}

// Convert a uint32_t word to a 32-element vector<bool> (MSB at index 0).
std::vector<bool> wordToBits(uint32_t word) {
    std::vector<bool> bits(32);
    for (int i = 0; i < 32; ++i)
        bits[i] = (word >> (31 - i)) & 1u; // index 0 = MSB
    return bits;
}
