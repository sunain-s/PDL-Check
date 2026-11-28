#ifndef AST_H
#define AST_H

#include "lexer.h"

//-------------------------------------------------------------------------------------------------
// Expressions

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

//-------------------------------------------------------------------------------------------------
// Statements

typedef enum {
    STMT_ASSIGN,
    STMT_IF,
    STMT_WHILE,
    STMT_SEQUENCE
} StatementType;

typedef struct Statement Statement;

struct Statement {
    StatementType type;

    union {
        struct {
            Expression *target;
            Expression *value;
        } Assign;

        struct {
            Expression *condition;
            Statement *then_branch;
            Statement *else_branch;
        } If;

        struct {
            Expression *condition;
            Statement *body;
        } While;

        struct {
            Statement **statements;
            size_t count;
            size_t capacity;
        } Sequence;
    };
};

Statement *ast_create_assignment(Expression *target, Expression *value);

Statement *ast_create_if(Expression *condition, Statement *then_branch, Statement *else_branch);

Statement *ast_create_while(Expression *condition, Statement *body);

Statement *ast_create_sequence(void);

#endif