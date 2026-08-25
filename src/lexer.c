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

static Token lex_integer(Lexer *lexer) {
    const char *start = &lexer->source[lexer->position];
    int line = lexer->line;
    int column = lexer->column;

    while (isdigit((unsigned char)lexer_current(lexer))) {
        lexer_advance(lexer);
    }

    size_t length = &lexer->source[lexer->position] - start;
    return make_token(TOKEN_INTEGER, start, length, line, column);
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

Token lexer_next_token(Lexer *lexer) {
    skip_whitespace(lexer);
    const char *start = &lexer->source[lexer->position];
    int line = lexer->line;
    int column = lexer->column;

    char c = lexer_current(lexer);
    if (c == '\0') {
        return make_token(TOKEN_EOF, start, 0, line, column);
    }
    if (isalpha((unsigned char)c) || c == '_') {
        return lex_identifier(lexer);
    }
    if (isdigit((unsigned char)c)) {
        return lex_integer(lexer);
    }

    // One character tokens
    switch (c) {
        case '+':
            lexer_advance(lexer);
            return make_token(TOKEN_PLUS, start, 1, line, column);
        case '-':
            lexer_advance(lexer);
            return make_token(TOKEN_MINUS, start, 1, line, column);
        case '*':
            lexer_advance(lexer);
            return make_token(TOKEN_STAR, start, 1, line, column);
        case '/':
            lexer_advance(lexer);
            return make_token(TOKEN_FSLASH, start, 1, line, column);
        case ';':
            lexer_advance(lexer);
            return make_token(TOKEN_SEMICOLON, start, 1, line, column);
        case '(':
            lexer_advance(lexer);
            return make_token(TOKEN_LPAREN, start, 1, line, column);
        case ')':
            lexer_advance(lexer);
            return make_token(TOKEN_RPAREN, start, 1, line, column);
        case '{':
            lexer_advance(lexer);
            return make_token(TOKEN_LBRACE, start, 1, line, column);
        case '}':
            lexer_advance(lexer);
            return make_token(TOKEN_RBRACE, start, 1, line, column);
    }

    // 2 character tokens
    if (c == '=') {
        if (lexer_peek_next(lexer) == '=') {
            lexer_advance(lexer);
            lexer_advance(lexer);
            return make_token(TOKEN_EQ, start, 2, line, column);
        }
        
        lexer_advance(lexer);
        return make_token(TOKEN_ASSIGN, start, 1, line, column);
    }

    if (c == '!') {
        if (lexer_peek_next(lexer) == '=') {
            lexer_advance(lexer);
            lexer_advance(lexer);
            return make_token(TOKEN_NEQ, start, 2, line, column);
        }
    }

    if (c == '<') {
        if (lexer_peek_next(lexer) == '=') {
            lexer_advance(lexer);
            lexer_advance(lexer);
            return make_token(TOKEN_LTE, start, 2, line, column);
        }
        
        lexer_advance(lexer);
        return make_token(TOKEN_LT, start, 1, line, column);
    }

    if (c == '>') {
        if (lexer_peek_next(lexer) == '=') {
            lexer_advance(lexer);
            lexer_advance(lexer);
            return make_token(TOKEN_GTE, start, 2, line, column);
        }
        
        lexer_advance(lexer);
        return make_token(TOKEN_GT, start, 1, line, column);
    }

    // Unknown character
    lexer_advance(lexer);
    return make_token(TOKEN_EOF, start, 0, line, column);
}