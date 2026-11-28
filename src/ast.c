#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast.h"

#define AST_INDENT_SIZE 4

//-------------------------------------------------------------------------------------------------
// Intenal helpers

static char *ast_strdup(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t length = strlen(str);
    char *copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, str, length + 1);
    return copy;
}

static void ast_print_recursive(const Expression *expr, int depth) {
    if (expr == NULL) {
        printf("%*sNULL\n", depth * AST_INDENT_SIZE, "");
        return;
    }

    printf("%*s", depth * AST_INDENT_SIZE, "");

    switch (expr->type) {
        case EXPR_INTEGER:
            printf("Int(%d)\n", expr->integer_val);
            break;

        case EXPR_IDENTIFIER:
            printf("Var(%s)\n", expr->identifier);
            break;

        case EXPR_BINARY:
            const char *op = "UNKNOWN";

            switch (expr->Binary.op) {
                case TOKEN_PLUS:   op = "+"; break;
                case TOKEN_MINUS:  op = "-"; break;
                case TOKEN_STAR:   op = "*"; break;
                case TOKEN_FSLASH: op = "/"; break;
                case TOKEN_EQ:     op = "=="; break;
                case TOKEN_NEQ:    op = "!="; break;
                case TOKEN_LT:     op = "<"; break;
                case TOKEN_GT:     op = ">"; break;
                case TOKEN_LTE:    op = "<="; break;
                case TOKEN_GTE:    op = ">="; break;
                default: break;
            }

            printf("Binary(%s)\n", op);
            ast_print_recursive(expr->Binary.left, depth + 1);
            ast_print_recursive(expr->Binary.right, depth + 1);
            break;
    }
}

//-------------------------------------------------------------------------------------------------
// Public functions

Expression *ast_create_integer(int value) {
    Expression *expr = malloc(sizeof(Expression));
    if (expr == NULL) {
        return NULL;
    }

    expr->type = EXPR_INTEGER;
    expr->integer_val = value;
    return expr;
}

Expression *ast_create_identifier(const char *name) {
    if (name == NULL) {
        return NULL;
    }

    Expression *expr = malloc(sizeof(Expression));
    if (expr == NULL) {
        return NULL;
    }

    expr->identifier = ast_strdup(name);
    if (expr->identifier == NULL) {
        free(expr);
        return NULL;
    }

    expr->type = EXPR_IDENTIFIER;
    return expr;
}

Expression *ast_create_binary(TokenType op, Expression *left, Expression *right) {
    Expression *expr = malloc(sizeof(Expression));
    if (expr == NULL) {
        return NULL;
    }

    expr->type = EXPR_BINARY;
    expr->Binary.op = op;
    expr->Binary.left = left;
    expr->Binary.right = right;
    return expr;
}

void ast_destroy(Expression *expr) {
    if (expr == NULL) {
        return;
    }

    switch (expr->type) {
        case EXPR_INTEGER:
            break;

        case EXPR_IDENTIFIER:
            free(expr->identifier);
            break;
        
        case EXPR_BINARY:
            ast_destroy(expr->Binary.left);
            ast_destroy(expr->Binary.right);
            break;
    }

    free(expr);
}

void ast_print(const Expression *expr) {
    ast_print_recursive(expr, 0);
}