#include "assembler.h"
#include "lexer.h"
#include "parser.h"
#include "encoder.h"
#include "../cpu/config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Assemble a source file (given as a list of lines) into a list of 32-bit words.
// The output vector is indexed from 0; word[0] goes to TEXT_START in memory.
std::vector<uint32_t> assemble(const std::vector<std::string>& sourceLines) {
    // Tokenise the source lines into a flat list of tokens
    // Calling from lexer.cpp: tokenise() function
    auto tokens = tokenise(sourceLines);

    // Parse the token stream into structured lines (instructions and directives)
    // Calling from parser.cpp: parse() function
    auto parsed = parse(tokens);

    // Build symbol table and assign addresses to instructions and data
    std::map<std::string, unsigned int> symtab;
    unsigned int textAddr = TEXT_START; // Text section starts at TEXT_START
    unsigned int dataAddr = DATA_START; // Data section starts at DATA_START
    bool inData = false;

    // Data section words (strings / .word directives)
    std::vector<std::pair<unsigned int, uint32_t>> dataWords;

    // assign addresses to instructions and data, and build symbol table for labels
    for (auto& pl : parsed) {
        if (pl.kind == LineKind::DIRECTIVE) {
            std::string dir = pl.mnemonic;
            if      (dir == ".text") { inData = false; }
            else if (dir == ".data") { inData = true;  }
            else if (dir == ".org") {
                unsigned int addr = static_cast<unsigned int>(parseImmediate(pl.operands[0]));
                if (inData) dataAddr = addr; else textAddr = addr;
            } else if (dir == ".word" && inData) {
                if (!pl.label.empty()) symtab[pl.label] = dataAddr;
                for (auto& op : pl.operands) {
                    dataWords.push_back({dataAddr, static_cast<uint32_t>(parseImmediate(op))});
                    ++dataAddr;
                }
            } else if ((dir == ".asciiz" || dir == ".ascii") && inData) {
                if (!pl.label.empty()) symtab[pl.label] = dataAddr;
                // Emit one word per character; null-terminate for .asciiz
                std::string str = pl.operands.empty() ? "" : pl.operands[0];
                for (char c : str) {
                    dataWords.push_back({dataAddr, static_cast<uint32_t>(c)});
                    ++dataAddr;
                }
                if (dir == ".asciiz") {
                    dataWords.push_back({dataAddr, 0u});
                    ++dataAddr;
                }
            }
            // Record any label attached to a directive
            if (!pl.label.empty() && dir != ".word" && dir != ".asciiz" && dir != ".ascii")
                symtab[pl.label] = inData ? dataAddr : textAddr;
            pl.address = inData ? dataAddr : textAddr;
            continue;
        }

        if (pl.kind == LineKind::EMPTY) {
            if (!pl.label.empty()) symtab[pl.label] = inData ? dataAddr : textAddr;
            continue;
        }

        // INSTRUCTION
        if (!pl.label.empty()) symtab[pl.label] = textAddr;
        pl.address = textAddr;
        ++textAddr;
    }

    // encode all instructions 
    // Pre-allocate output covering text + data
    unsigned int maxAddr = textAddr > dataAddr ? textAddr : dataAddr;
    std::vector<uint32_t> words(maxAddr, 0u);
    for (const auto& pl : parsed) {
        if (pl.kind != LineKind::INSTRUCTION) continue;
        uint32_t word = encode(pl, symtab, pl.address);
        words[pl.address] = word;
    }

    // Splice in data section words
    for (auto& [addr, val] : dataWords) {
        if (addr < words.size()) words[addr] = val;
    }

    return words;
}

// Function to read a .asm file from disk, assemble it, and return the machine words.
std::vector<uint32_t> assembleFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Cannot open: " + path);
    std::vector<std::string> lines;
    std::string line;
    // Read the file line by line into a vector of strings
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return assemble(lines);
}

// Function to write assembled words as raw binary to a file.
void writeBinary(const std::string& path, const std::vector<uint32_t>& words) {
    std::ofstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot write: " + path);
    for (uint32_t w : words) {
        // Big-endian byte order (MSB first, matches vector<bool> index-0=MSB)
        uint8_t bytes[4] = {
            static_cast<uint8_t>((w >> 24) & 0xFF),
            static_cast<uint8_t>((w >> 16) & 0xFF),
            static_cast<uint8_t>((w >>  8) & 0xFF),
            static_cast<uint8_t>( w        & 0xFF),
        };
        f.write(reinterpret_cast<char*>(bytes), 4);
    }
}
