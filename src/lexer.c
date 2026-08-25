#include <stdlib.h>
#include "lexer.h"

//-------------------------------------------------------------------------------------------------
// Structs

struct Lexer {
    const char *source;
    size_t position;
    int line;
    int column;
};

//-------------------------------------------------------------------------------------------------
// Intenal helpers

static char lexer_current(Lexer *lexer) {
    return lexer->source[lexer->position];
}

static char lexer_peek_next(Lexer *lexer) {
    if (lexer->source[lexer->position + 1] == '\0') {
        return '\0';
    }
    return lexer->source[lexer->position + 1];
}

static void lexer_advance(Lexer *lexer) {
    if (lexer_current(lexer) == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    lexer->position++;
}

static Token make_token(TokenType type, const char *start, size_t length, int line, int column) {
    Token token;
    token.type = type;
    token.line = line;
    token.column = column;

    token.lexeme = malloc(length + 1);
    if (token.lexeme == NULL) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    memcpy(token.lexeme, start, length);
    token.lexeme[length] = '\0';
    return token;
}

static void skip_whitespace(Lexer *lexer) {
    while (isspace((unsigned char)lexer_current(lexer))) {
        lexer_advance(lexer);
    }
}

static TokenType keyword_type(const char *lexeme) {
    if (strcmp(lexeme, "if") == 0) {
        return TOKEN_IF;
    }
    if (strcmp(lexeme, "else") == 0) {
        return TOKEN_ELSE;
    }
    if (strcmp(lexeme, "while") == 0) {
        return TOKEN_WHILE;
    }

    return TOKEN_IDENTIFIER;
}

static Token lex_identifier(Lexer *lexer) {
    const char *start = &lexer->source[lexer->position];
    int line = lexer->line;
    int column = lexer->column;

    while (isalnum((unsigned char)lexer_current(lexer)) || lexer_current(lexer) == '_') {
        lexer_advance(lexer);
    }

    size_t length = &lexer->source[lexer->position] - start;
    Token token = make_token(TOKEN_IDENTIFIER, start, length, line, column);
    token.type = keyword_type(token.lexeme);
    return token;
}

//-------------------------------------------------------------------------------------------------
// Public functions

Lexer *lexer_create(const char *source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    if (lexer == NULL) {
        return NULL;
    }

    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

void lexer_destroy(Lexer *lexer) {
    free(lexer);
}
