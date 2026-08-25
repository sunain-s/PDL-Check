#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

static const char *token_type_name(TokenType type)
{
    switch (type) {
        case TOKEN_EOF:        return "EOF";

        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER:    return "INTEGER";

        case TOKEN_PLUS:       return "PLUS";
        case TOKEN_MINUS:      return "MINUS";
        case TOKEN_STAR:       return "STAR";
        case TOKEN_FSLASH:     return "FSLASH";

        case TOKEN_EQ:         return "EQ";
        case TOKEN_NEQ:        return "NEQ";
        case TOKEN_LT:         return "LT";
        case TOKEN_GT:         return "GT";
        case TOKEN_LTE:        return "LTE";
        case TOKEN_GTE:        return "GTE";

        case TOKEN_ASSIGN:     return "ASSIGN";

        case TOKEN_SEMICOLON:  return "SEMICOLON";
        case TOKEN_LPAREN:     return "LPAREN";
        case TOKEN_RPAREN:     return "RPAREN";
        case TOKEN_LBRACE:     return "LBRACE";
        case TOKEN_RBRACE:     return "RBRACE";

        case TOKEN_IF:         return "IF";
        case TOKEN_ELSE:       return "ELSE";
        case TOKEN_WHILE:      return "WHILE";
    }

    return "UNKNOWN";
}

int main(void)
{
    const char *source =
        "if (x == 10) {\n"
        "    y = x + 1;\n"
        "} else {\n"
        "    y = 0;\n"
        "}";

    Lexer *lexer = lexer_create(source);

    if (lexer == NULL) {
        fprintf(stderr, "Failed to create lexer\n");
        return EXIT_FAILURE;
    }

    while (1) {
        Token token = lexer_next_token(lexer);

        printf(
            "%-12s lexeme=\"%s\" line=%d column=%d\n",
            token_type_name(token.type),
            token.lexeme,
            token.line,
            token.column
        );

        free(token.lexeme);

        if (token.type == TOKEN_EOF) {
            break;
        }
    }

    lexer_destroy(lexer);

    return EXIT_SUCCESS;
}
