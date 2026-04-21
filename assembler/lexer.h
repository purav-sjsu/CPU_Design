#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    MNEMONIC,    // add, lw, beq …
    REGISTER,    // $t0, $0, $ra …
    IMMEDIATE,   // 42, -7, 0xFF
    LABEL_DEF,   // "loop:" — the definition site
    LABEL_REF,   // "loop"  — used as a branch/jump target
    DIRECTIVE,   // .text, .data, .word, .asciiz, .org
    STRING,      // "Hello, World!"
    LPAREN,      // (
    RPAREN,      // )
    COMMA,       // ,
    END_OF_LINE,
};

struct Token {
    TokenType   type;
    std::string value;  // raw text (without surrounding quotes for strings)
    int         line;
};

// Tokenise a single source line. Returns an empty vector for blank/comment lines.
std::vector<Token> tokeniseLine(const std::string& line, int lineNo);

// Tokenise an entire source file (vector of lines).
std::vector<Token> tokenise(const std::vector<std::string>& lines);

#endif // LEXER_H
