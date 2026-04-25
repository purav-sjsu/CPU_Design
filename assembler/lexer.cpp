#include "lexer.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// List of valid mnemonics (case-insensitive)
static const std::vector<std::string> MNEMONICS = {
    "add","addu","sub","subu","and","or","xor","nor","slt","sltu",
    "sll","srl","sra","jr","jalr",
    "addi","addiu","slti","sltiu","andi","ori","xori","lui",
    "lw","sw","beq","bne",
    "j","jal",
    "nop","halt","move","li","la",
    "mult","multu","mfhi","mflo" // added for multiplication instructions
};


// Function to check if a string is a valid mnemonic
// Strings are first converted to lowercase for case-insensitive
// comparison against the list of mnemonics.
static bool isMnemonic(const std::string& s) {
    std::string lower = s;
    // Convert to lowercase for case-insensitive comparison
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    // Match the string against the list of mnemonics
    for (auto& m : MNEMONICS) if (m == lower) return true;
    return false;
}

// Check if a string is an assembler directive (starts with '.'), e.g. .text, .data, .word, .asciiz, .org
static bool isDirective(const std::string& s) {
    return !s.empty() && s[0] == '.';
}

// Check if a string looks like a register name, e.g. $t0, $0, $ra
static bool isRegister(const std::string& s) {
    return !s.empty() && s[0] == '$';
}

// Check if a string looks like an immediate value, e.g. 42, -7, 0xFF
static bool isImmediate(const std::string& s) {
    if (s.empty()) return false;
    int start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if ((int)s.size() > start + 2 && s[start] == '0' &&
        (s[start+1] == 'x' || s[start+1] == 'X')) return true;
    for (int i = start; i < (int)s.size(); ++i)
        if (!std::isdigit(s[i])) return false;
    return s.size() > (size_t)start;
}

// Tokenise a single line of assembly code into a vector of Tokens.
std::vector<Token> tokeniseLine(const std::string& rawLine, int lineNo) {
    std::vector<Token> tokens;

    // Strip comment lines and in-line comments (starting with # or ;)
    std::string line = rawLine;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '#' || line[i] == ';') { line = line.substr(0, i); break; }
    }

    // Skip Whitespace at the start and end of the line
    // Tokenise the line by splitting on whitespace and punctuation
    size_t i = 0;
    auto skipWS = [&]() { while (i < line.size() && std::isspace(line[i])) ++i; };
    skipWS();
    while (i < line.size()) {
        char c = line[i];

        // String literal, for example "Hello World!\n"
        if (c == '"') {
            ++i;
            std::string str;
            while (i < line.size() && line[i] != '"') {
                if (line[i] == '\\' && i + 1 < line.size()) {
                    char esc = line[++i];
                    switch (esc) {
                        case 'n':  str += '\n'; break;
                        case 't':  str += '\t'; break;
                        case '\\': str += '\\'; break;
                        case '"':  str += '"';  break;
                        default:   str += esc;  break;
                    }
                } else {
                    str += line[i];
                }
                ++i;
            }
            if (i < line.size()) ++i; // closing "
            tokens.push_back({TokenType::STRING, str, lineNo});
            skipWS(); continue;
        }

        // Punctuation tokens: parentheses 4($sp) and commas in between operands
        if (c == '(') { tokens.push_back({TokenType::LPAREN,  "(", lineNo}); ++i; skipWS(); continue; }
        if (c == ')') { tokens.push_back({TokenType::RPAREN,  ")", lineNo}); ++i; skipWS(); continue; }
        if (c == ',') { tokens.push_back({TokenType::COMMA,   ",", lineNo}); ++i; skipWS(); continue; }

        // Read next word (runs of non-space, non-punct characters)
        size_t start = i;
        while (i < line.size() && !std::isspace(line[i]) &&
               line[i] != ',' && line[i] != '(' && line[i] != ')') ++i;
        std::string word = line.substr(start, i - start);

        if (word.empty()) { skipWS(); continue; }

        // Label definition: ends with ':'
        if (word.back() == ':') {
            tokens.push_back({TokenType::LABEL_DEF, word.substr(0, word.size()-1), lineNo});
            skipWS(); continue;
        }

        // Classify the word into token types: directive, register, immediate, mnemonic, or label reference
        if (isDirective(word))       tokens.push_back({TokenType::DIRECTIVE,  word, lineNo}); // Words starting with '.' are assembler directives
        else if (isMnemonic(word))   tokens.push_back({TokenType::MNEMONIC,   word, lineNo}); // Word is a valid instruction mnemonic
        else if (isRegister(word))   tokens.push_back({TokenType::REGISTER,   word, lineNo}); // Words starting with '$' are registers
        else if (isImmediate(word))  tokens.push_back({TokenType::IMMEDIATE,  word, lineNo}); // Words that look like numbers are immediates
        else                         tokens.push_back({TokenType::LABEL_REF,  word, lineNo}); // Otherwise, it's a label reference (used as branch/jump target or data label)

        skipWS();
    }

    // Add an END_OF_LINE token at the end of the line to simplify parsing logic later.
    if (!tokens.empty())
        tokens.push_back({TokenType::END_OF_LINE, "", lineNo});

    return tokens;
}

// Tokenise an entire source file (vector of lines).
std::vector<Token> tokenise(const std::vector<std::string>& lines) {
    std::vector<Token> all;
    for (int i = 0; i < (int)lines.size(); ++i) {
        // Tokenise each line and append to the overall token list
        auto toks = tokeniseLine(lines[i], i + 1);
        all.insert(all.end(), toks.begin(), toks.end());
    }
    return all;
}
