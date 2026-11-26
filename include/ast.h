#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum{
    EXPR_IDENTIFIER,
    EXPR_INTEGER,
    EXPR_BINARY
} ExpressionType;

typedef struct Expression Expression;

struct Expression {
    ExpressionType type;
    
    union {
        int integer_val;
        char *identifier;

        struct {
            TokenType op;
            Expression *left;
            Expression *right;
        } BinaryOp;
    };
};

#endif