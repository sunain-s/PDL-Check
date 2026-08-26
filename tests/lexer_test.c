#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

//-------------------------------------------------------------------------------------------------
// Test helpers

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(condition) \
    do { \
        tests_run++; \
        if (!(condition)) { \
            fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            tests_failed++; \
            return; \
        } \
    } while (0)

#define ASSERT_INT(expected, actual) \
    do { \
        tests_run++; \
        if ((expected) != (actual)) { \
            fprintf(stderr, "FAIL: %s:%d: Expected %d, got %d\n", __FILE__, __LINE__, (expected), (actual)); \
            tests_failed++; \
            return; \
        } \
    } while (0)

#define ASSERT_STRING(expected, actual) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) != 0) { \
            fprintf(stderr, "FAIL: %s:%d: Expected \"%s\", got \"%s\"\n", __FILE__, __LINE__, (expected), (actual)); \
            tests_failed++; \
            return; \
        } \
    } while (0)

static void assert_token(Token token, TokenType expected_type, const char *expected_lexeme, int expected_line, int expected_column) {
    ASSERT_INT(expected_type, token.type);
    ASSERT_STRING(expected_lexeme, token.lexeme);
    ASSERT_INT(expected_line, token.line);
    ASSERT_INT(expected_column, token.column);

    free(token.lexeme);
}

static Token next_token(Lexer *lexer) {
    return lexer_next_token(lexer);
}

//-------------------------------------------------------------------------------------------------
// Tests

static void test_identifiers(void) {
    Lexer *lexer = lexer_create("foo bar _temp hello_world x123");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "foo", 1, 1);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "bar", 1, 5);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "_temp", 1, 9);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "hello_world", 1, 15);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "x123", 1, 27);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 31);

    lexer_destroy(lexer);
}

//-------------------------------------------------------------------------------------------------
// Test runner

typedef void (*TestFunction)(void);

typedef struct {
    const char *name;
    TestFunction function;
} Test;

static Test tests[] = {
    {"identifiers", test_identifiers}
};

int main(void) {
    size_t test_count = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < test_count; i++) {
        int failures_before = tests_failed;
        tests[i].function();

        if (tests_failed == failures_before) {
            printf("PASS: %s\n", tests[i].name);
        }
    }
    printf("\n%d assertions, %d failed\n", tests_run, tests_failed);
    return tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}