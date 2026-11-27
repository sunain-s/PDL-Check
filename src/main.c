#include <stdio.h>
#include <stdlib.h>

#include "ast.h"


int main() {
    // Test 1: Integer
    Expression *integer = ast_create_integer(42);

    printf("=== Test 1: Integer ===\n");
    ast_print(integer);
    ast_destroy(integer);


    // Test 2: Identifier
    Expression *identifier = ast_create_identifier("my_variable");

    printf("\n=== Test 2: Identifier ===\n");
    ast_print(identifier);
    ast_destroy(identifier);


    // Test 3: Binary expression (10 + 20)
    Expression *binary = ast_create_binary(
        TOKEN_PLUS,
        ast_create_integer(10),
        ast_create_integer(20)
    );

    printf("\n=== Test 3: Binary ===\n");
    ast_print(binary);
    ast_destroy(binary);


    // Test 4: Nested binary expression ((10 + 20) * 5)
    Expression *nested = ast_create_binary(
        TOKEN_STAR,
        ast_create_binary(
            TOKEN_PLUS,
            ast_create_integer(10),
            ast_create_integer(20)
        ),
        ast_create_integer(5)
    );

    printf("\n=== Test 4: Nested Binary ===\n");
    ast_print(nested);
    ast_destroy(nested);

    return 0;
}
