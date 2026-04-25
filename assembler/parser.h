#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>
#include <vector>

enum class LineKind { INSTRUCTION, DIRECTIVE, EMPTY };

// One parsed source line, after tokenisation and classification.
// Contains the line's label (if any), instruction/directive mnemonic,
// operands, and source line number for error reporting.
struct ParsedLine {
    LineKind            kind = LineKind::EMPTY; // instruction, directive, or empty (label-only or blank)
    std::string         label;                  // label defined on this line (may be empty)
    std::string         mnemonic;               // instruction or directive name
    std::vector<std::string> operands;          // raw operand strings
    int                 lineNumber = 0;         // Source line number for error reporting
    unsigned int        address    = 0;         // filled during pass 1
};

// Parse a flat token stream (from tokenise()) into a list of ParsedLines.
std::vector<ParsedLine> parse(const std::vector<Token>& tokens);

// Resolve a register name ($t0, $ra, $0 …) to its integer index 0-31.
// Throws std::invalid_argument for invalid or unknown names.
int resolveRegister(const std::string& name);

// Parse an integer literal (decimal, 0xHEX, or a label looked up in symtab).
// If the string is a label, *isLabel is set true.
int parseImmediate(const std::string& s, bool* isLabel = nullptr);

#endif // PARSER_H
