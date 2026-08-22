#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_EOF,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,

    // Arithmetic operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_FSLASH,

    // Comparison operators
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTE,
    TOKEN_GTE,

    // Assignment
    TOKEN_ASSIGN,
} TokenType;

#endif