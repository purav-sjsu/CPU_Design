#include "assembler.h"
#include <iostream>
#include <string>
#include <iomanip>

static void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " <source.asm> [-o output.bin] [--hex]\n"
              << "  --hex   print hex listing to stdout instead of writing binary\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { printUsage(argv[0]); return 1; }

    std::string inputPath;
    std::string outputPath;
    bool hexMode = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) outputPath = argv[++i];
        else if (arg == "--hex")         hexMode = true;
        else if (arg[0] != '-')          inputPath = arg;
    }

    if (inputPath.empty()) { printUsage(argv[0]); return 1; }
    if (outputPath.empty() && !hexMode) {
        // Default output: replace .asm with .bin
        outputPath = inputPath;
        size_t dot = outputPath.rfind('.');
        if (dot != std::string::npos) outputPath = outputPath.substr(0, dot);
        outputPath += ".bin";
    }

    try {
        auto words = assembleFile(inputPath);

        if (hexMode) {
            std::cout << "Addr     Word\n";
            for (size_t i = 0; i < words.size(); ++i) {
                if (words[i] != 0) {
                    std::cout << std::hex << std::setw(8) << std::setfill('0') << i
                              << " " << std::setw(8) << words[i] << "\n";
                }
            }
        } else {
            writeBinary(outputPath, words);
            std::cout << "Assembled " << words.size() << " words → " << outputPath << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
