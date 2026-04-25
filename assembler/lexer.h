#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Token types for the assembler lexer.
enum class TokenType {
    MNEMONIC,    // add, lw, beq …
    REGISTER,    // Register names like $t0, $0, $ra …
    IMMEDIATE,   // Immediate values like 42, -7, 0xFF
    LABEL_DEF,   // "loop:" the definition site
    LABEL_REF,   // "loop" used as a branch/jump target
    DIRECTIVE,   // .text, .data etc.
    STRING,      // "Hello, World!"
    LPAREN,      // Left parenthesis "("
    RPAREN,      // Right parenthesis ")"
    COMMA,       // Comma ","
    END_OF_LINE,
};

struct Token {
    TokenType   type;   // Type of the token
    std::string value;  // raw text of the token
    int         line;   // Line number in source file
};

// Tokenise a single source line. Returns an empty vector for blank/comment lines.
std::vector<Token> tokeniseLine(const std::string& line, int lineNo);

// Tokenise an entire source file (vector of lines).
std::vector<Token> tokenise(const std::vector<std::string>& lines);

#endif // LEXER_H
