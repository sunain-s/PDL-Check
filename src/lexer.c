#include <stdlib.h>
#include "lexer.h"

struct Lexer {
    const char *source;
    size_t position;
    int line;
    int column;
};

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