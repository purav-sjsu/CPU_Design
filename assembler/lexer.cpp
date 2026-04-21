#include "lexer.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// Known mnemonics — anything else that starts a line is treated as a label ref
static const std::vector<std::string> MNEMONICS = {
    "add","addu","sub","subu","and","or","xor","nor","slt","sltu",
    "sll","srl","sra","jr","jalr",
    "addi","addiu","slti","sltiu","andi","ori","xori","lui",
    "lw","sw","beq","bne",
    "j","jal",
    "nop","halt","move","li","la"
};

static bool isMnemonic(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (auto& m : MNEMONICS) if (m == lower) return true;
    return false;
}

static bool isDirective(const std::string& s) {
    return !s.empty() && s[0] == '.';
}

static bool isRegister(const std::string& s) {
    return !s.empty() && s[0] == '$';
}

static bool isImmediate(const std::string& s) {
    if (s.empty()) return false;
    int start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if ((int)s.size() > start + 2 && s[start] == '0' &&
        (s[start+1] == 'x' || s[start+1] == 'X')) return true;
    for (int i = start; i < (int)s.size(); ++i)
        if (!std::isdigit(s[i])) return false;
    return s.size() > (size_t)start;
}

std::vector<Token> tokeniseLine(const std::string& rawLine, int lineNo) {
    std::vector<Token> tokens;

    // Strip comment (# or ;)
    std::string line = rawLine;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '#' || line[i] == ';') { line = line.substr(0, i); break; }
    }

    size_t i = 0;
    auto skipWS = [&]() { while (i < line.size() && std::isspace(line[i])) ++i; };

    skipWS();
    while (i < line.size()) {
        char c = line[i];

        // String literal
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

        // Punctuation
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

        // Classify
        if      (isDirective(word))  tokens.push_back({TokenType::DIRECTIVE,  word, lineNo});
        else if (isRegister(word))   tokens.push_back({TokenType::REGISTER,   word, lineNo});
        else if (isImmediate(word))  tokens.push_back({TokenType::IMMEDIATE,  word, lineNo});
        else if (isMnemonic(word))   tokens.push_back({TokenType::MNEMONIC,   word, lineNo});
        else                         tokens.push_back({TokenType::LABEL_REF,  word, lineNo});

        skipWS();
    }

    if (!tokens.empty())
        tokens.push_back({TokenType::END_OF_LINE, "", lineNo});

    return tokens;
}

std::vector<Token> tokenise(const std::vector<std::string>& lines) {
    std::vector<Token> all;
    for (int i = 0; i < (int)lines.size(); ++i) {
        auto toks = tokeniseLine(lines[i], i + 1);
        all.insert(all.end(), toks.begin(), toks.end());
    }
    return all;
}
