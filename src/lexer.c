#include <stdlib.h>
#include "lexer.h"

struct Lexer {
    const char *source;
    size_t position;
    int line;
    int column;
};