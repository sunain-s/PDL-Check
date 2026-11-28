#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

int main(void) {
    Statement *program = ast_create_sequence();

    // x = 10;
    Statement *assign = ast_create_assignment(ast_create_identifier("x"), ast_create_integer(10));
    ast_sequence_append(program, assign);

    // while (x < 20) { x = x + 1; }
    Statement *body = ast_create_sequence();
    Statement *increment = ast_create_assignment(
        ast_create_identifier("x"),
        ast_create_binary(TOKEN_PLUS, ast_create_identifier("x"), ast_create_integer(1))
    );

    ast_sequence_append(body, increment);
    Statement *loop = ast_create_while(ast_create_binary(TOKEN_LT, ast_create_identifier("x"), ast_create_integer(20)), body);
    ast_sequence_append(program, loop);

    // if (x == 20) { x = 0; } else { x = 10; }
    Statement *then_branch = ast_create_sequence();
    ast_sequence_append(then_branch, ast_create_assignment(ast_create_identifier("x"), ast_create_integer(0)));
    Statement *else_branch = ast_create_sequence();
    ast_sequence_append(else_branch, ast_create_assignment(ast_create_identifier("x"), ast_create_integer(10)));

    Statement *if_stmt = ast_create_if(
        ast_create_binary(TOKEN_EQ, ast_create_identifier("x"), ast_create_integer(20)),
        then_branch,
        else_branch
    );

    ast_sequence_append(program, if_stmt);
    ast_print_statement(program);
    ast_destroy_stmt(program);

    return 0;
}