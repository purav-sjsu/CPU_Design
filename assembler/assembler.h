#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>

// Assemble a source file (given as a list of lines) into a list of 32-bit words.
// The output vector is indexed from 0; word[0] goes to TEXT_START in memory.
std::vector<uint32_t> assemble(const std::vector<std::string>& sourceLines);

// Convenience: read a .asm file from disk, assemble it, return machine words.
std::vector<uint32_t> assembleFile(const std::string& path);

// Write assembled words as binary to a file.
void writeBinary(const std::string& path, const std::vector<uint32_t>& words);

#endif // ASSEMBLER_H
