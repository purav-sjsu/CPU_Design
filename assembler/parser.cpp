#include "parser.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <map>

// Register name table for resolveRegister()
// Name and index are based on MIPS register conventions, with aliases for
// numeric names ($0, $1, …) and common names ($t0, $ra, etc.)
static const std::map<std::string, int> REG_NAMES = {
    {"$zero",0},{"$0",0},  {"$at",1},  {"$1",1},
    {"$v0",2},  {"$v1",3},  {"$2",2},  {"$3",3},
    {"$a0",4},  {"$a1",5},  {"$a2",6},  {"$a3",7},
    {"$4",4},   {"$5",5},   {"$6",6},   {"$7",7},
    {"$t0",8},  {"$t1",9},  {"$t2",10}, {"$t3",11},
    {"$t4",12}, {"$t5",13}, {"$t6",14}, {"$t7",15},
    {"$8",8},   {"$9",9},   {"$10",10}, {"$11",11},
    {"$12",12}, {"$13",13}, {"$14",14}, {"$15",15},
    {"$s0",16}, {"$s1",17}, {"$s2",18}, {"$s3",19},
    {"$s4",20}, {"$s5",21}, {"$s6",22}, {"$s7",23},
    {"$16",16}, {"$17",17}, {"$18",18}, {"$19",19},
    {"$20",20}, {"$21",21}, {"$22",22}, {"$23",23},
    {"$t8",24}, {"$t9",25}, {"$24",24}, {"$25",25},
    {"$k0",26}, {"$k1",27}, {"$26",26}, {"$27",27},
    {"$gp",28}, {"$28",28},
    {"$sp",29}, {"$29",29},
    {"$fp",30}, {"$30",30},
    {"$ra",31}, {"$31",31},
};

// Resolve a register name ($t0, $ra, $0 …) to its integer index 0-31.
int resolveRegister(const std::string& name) {
    auto it = REG_NAMES.find(name);
    if (it == REG_NAMES.end())
        throw std::invalid_argument("Unknown register: " + name);
    return it->second;
}

// Parse an integer literal (decimal, 0xHEX, or a label looked up in symtab).
int parseImmediate(const std::string& s, bool* isLabel) {
    if (isLabel) *isLabel = false;
    if (s.empty()) return 0;
    try {
        size_t pos;
        int v = std::stoi(s, &pos, 0); // base 0: auto-detect 0x prefix
        if (pos == s.size()) return v;
    } catch (...) {}
    if (isLabel) *isLabel = true;
    return 0; // caller must resolve using symbol table
}

// Parse tokens (from tokenise()) into a list of ParsedLines.
std::vector<ParsedLine> parse(const std::vector<Token>& tokens) {
    std::vector<ParsedLine> lines;
    ParsedLine current;
    bool inInstruction = false;

    // Flush the current line to the output list if it's not empty, and reset it.
    auto flush = [&]() {
        if (current.kind != LineKind::EMPTY || !current.label.empty()) {
            lines.push_back(current);
        }
        current = ParsedLine{};
        inInstruction = false;
    };

    
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& tok = tokens[i];

        // If END_OF_LINE, flush the current line and start a new one
        if (tok.type == TokenType::END_OF_LINE) {
            flush();
            continue;
        }

        // Save the line number for error reporting in the ParsedLine struct
        current.lineNumber = tok.line;

        switch (tok.type) {
            case TokenType::LABEL_DEF:
                // Save the label
                current.label = tok.value;
                break;

            case TokenType::MNEMONIC:
                // Start of an instruction line
                // Save the mnemonic and set the line kind to INSTRUCTION
                current.kind     = LineKind::INSTRUCTION;
                current.mnemonic = tok.value;
                // Lowercase for uniform comparison
                std::transform(current.mnemonic.begin(), current.mnemonic.end(),
                               current.mnemonic.begin(), ::tolower);
                inInstruction = true;
                break;

            case TokenType::DIRECTIVE:
                // Start of a directive line
                // Save the directive name and set the line kind to DIRECTIVE
                current.kind     = LineKind::DIRECTIVE;
                current.mnemonic = tok.value;
                inInstruction    = true;
                break;

            case TokenType::REGISTER:
            case TokenType::IMMEDIATE:
            case TokenType::LABEL_REF:
            case TokenType::STRING:
                // Part of the operands for the current instruction/directive
                if (inInstruction)
                    current.operands.push_back(tok.value);
                break;

            // Parentheses modify the previous operand (offset(base) syntax)
            // e.g. "lw $t0, 4($sp)" tokenises to MNEMONIC=lw, REGISTER=$t0, IMMEDIATE=4, LPAREN, REGISTER=$sp, RPAREN
            case TokenType::LPAREN:
                // The next token is the base register; merge it with last operand
                if (!current.operands.empty() && i + 1 < tokens.size()) {
                    std::string base = tokens[++i].value; // register
                    current.operands.back() += "(" + base + ")";
                    // skip the closing RPAREN
                    if (i + 1 < tokens.size() &&
                        tokens[i+1].type == TokenType::RPAREN) ++i;
                }
                break;

            case TokenType::COMMA:
            case TokenType::RPAREN:
                break; // already consumed above or ignored

            default:
                break;
        }
    }
    flush();
    return lines;
}
