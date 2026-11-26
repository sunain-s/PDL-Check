#include <stdlib.h>
#include <string.h>

#include "ast.h"

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
