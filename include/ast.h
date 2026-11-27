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
        } Binary;
    };
};

Expression *ast_create_integer(int value);

Expression *ast_create_identifier(const char *name);

Expression *ast_create_binary(TokenType operator, Expression *left, Expression *right);

void ast_destroy(Expression *expr);

void ast_print(const Expression *expr);

#endif