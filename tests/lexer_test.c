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

static void test_identifiers() {
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

static void test_integers() {
    Lexer *lexer = lexer_create("123 456 7890 0 -12");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_INTEGER, "123", 1, 1);
    assert_token(next_token(lexer), TOKEN_INTEGER, "456", 1, 5);
    assert_token(next_token(lexer), TOKEN_INTEGER, "7890", 1, 9);
    assert_token(next_token(lexer), TOKEN_INTEGER, "0", 1, 14);
    assert_token(next_token(lexer), TOKEN_MINUS, "-", 1, 16);
    assert_token(next_token(lexer), TOKEN_INTEGER, "12", 1, 17);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 19);

    lexer_destroy(lexer);
}

static void test_arithmetic_operators() {
    Lexer *lexer = lexer_create("+ - * /");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_PLUS, "+", 1, 1);
    assert_token(next_token(lexer), TOKEN_MINUS, "-", 1, 3);
    assert_token(next_token(lexer), TOKEN_STAR, "*", 1, 5);
    assert_token(next_token(lexer), TOKEN_FSLASH, "/", 1, 7);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 8);

    lexer_destroy(lexer);
}

static void test_comparison_operators() {
    Lexer *lexer = lexer_create("== != < > <= >=");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_EQ, "==", 1, 1);
    assert_token(next_token(lexer), TOKEN_NEQ, "!=", 1, 4);
    assert_token(next_token(lexer), TOKEN_LT, "<", 1, 7);
    assert_token(next_token(lexer), TOKEN_GT, ">", 1, 9);
    assert_token(next_token(lexer), TOKEN_LTE, "<=", 1, 11);
    assert_token(next_token(lexer), TOKEN_GTE, ">=", 1, 14);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 16);

    lexer_destroy(lexer);
}

static void test_assignment() {
    Lexer *lexer = lexer_create("= ==");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_ASSIGN, "=", 1, 1);
    assert_token(next_token(lexer), TOKEN_EQ, "==", 1, 3);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 5);

    lexer_destroy(lexer);
}

static void test_punctuation() {
    Lexer *lexer = lexer_create("; ( ) { }");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_SEMICOLON, ";", 1, 1);
    assert_token(next_token(lexer), TOKEN_LPAREN, "(", 1, 3);
    assert_token(next_token(lexer), TOKEN_RPAREN, ")", 1, 5);
    assert_token(next_token(lexer), TOKEN_LBRACE, "{", 1, 7);
    assert_token(next_token(lexer), TOKEN_RBRACE, "}", 1, 9);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 10);

    lexer_destroy(lexer);
}

static void test_keywords() {
    Lexer *lexer = lexer_create("if else while");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_IF, "if", 1, 1);
    assert_token(next_token(lexer), TOKEN_ELSE, "else", 1, 4);
    assert_token(next_token(lexer), TOKEN_WHILE, "while", 1, 9);
    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 14);

    lexer_destroy(lexer);
}

static void test_whitespace() {
    Lexer *lexer = lexer_create("  \t\n  x\r\n\n foo 123");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "x", 2, 3);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "foo", 4, 2);
    assert_token(next_token(lexer), TOKEN_INTEGER, "123", 4, 6);
    assert_token(next_token(lexer), TOKEN_EOF, "", 4, 9);

    lexer_destroy(lexer);
}

static void line_and_column_tracking() {
    Lexer *lexer = lexer_create(
        "x = 10;\n"
        "y = x + 1;\n"
        "while (y > 0) {}"
    );
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "x", 1, 1);
    assert_token(next_token(lexer), TOKEN_ASSIGN, "=", 1, 3);
    assert_token(next_token(lexer), TOKEN_INTEGER, "10", 1, 5);
    assert_token(next_token(lexer), TOKEN_SEMICOLON, ";", 1, 7);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "y", 2, 1);
    assert_token(next_token(lexer), TOKEN_ASSIGN, "=", 2, 3);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "x", 2, 5);
    assert_token(next_token(lexer), TOKEN_PLUS, "+", 2, 7);
    assert_token(next_token(lexer), TOKEN_INTEGER, "1", 2, 9);
    assert_token(next_token(lexer), TOKEN_SEMICOLON, ";", 2, 10);
    assert_token(next_token(lexer), TOKEN_WHILE, "while", 3, 1);
    assert_token(next_token(lexer), TOKEN_LPAREN, "(", 3, 7);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "y", 3, 8);
    assert_token(next_token(lexer), TOKEN_GT, ">", 3, 10);
    assert_token(next_token(lexer), TOKEN_INTEGER, "0", 3, 12);
    assert_token(next_token(lexer), TOKEN_RPAREN, ")", 3, 13);
    assert_token(next_token(lexer), TOKEN_LBRACE, "{", 3, 15);
    assert_token(next_token(lexer), TOKEN_RBRACE, "}", 3, 16);
    assert_token(next_token(lexer), TOKEN_EOF, "", 3, 17);

    lexer_destroy(lexer);
}

static void test_complete_program() {
    const char *source = 
        "if (x == 10) {\n"
        "    y = x + 1;\n"
        "} else {\n"
        "    y = 0;\n"
        "}";

    Lexer *lexer = lexer_create(source);
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_IF, "if", 1, 1);
    assert_token(next_token(lexer), TOKEN_LPAREN, "(", 1, 4);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "x", 1, 5);
    assert_token(next_token(lexer), TOKEN_EQ, "==", 1, 7);
    assert_token(next_token(lexer), TOKEN_INTEGER, "10", 1, 10);
    assert_token(next_token(lexer), TOKEN_RPAREN, ")", 1, 12);
    assert_token(next_token(lexer), TOKEN_LBRACE, "{", 1, 14);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "y", 2, 5);
    assert_token(next_token(lexer), TOKEN_ASSIGN, "=", 2, 7);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "x", 2, 9);
    assert_token(next_token(lexer), TOKEN_PLUS, "+", 2, 11);
    assert_token(next_token(lexer), TOKEN_INTEGER, "1", 2, 13);
    assert_token(next_token(lexer), TOKEN_SEMICOLON, ";", 2, 14);
    assert_token(next_token(lexer), TOKEN_RBRACE, "}", 3, 1);
    assert_token(next_token(lexer), TOKEN_ELSE, "else", 3, 3);
    assert_token(next_token(lexer), TOKEN_LBRACE, "{", 3, 8);
    assert_token(next_token(lexer), TOKEN_IDENTIFIER, "y", 4, 5);
    assert_token(next_token(lexer), TOKEN_ASSIGN, "=", 4, 7);
    assert_token(next_token(lexer), TOKEN_INTEGER, "0", 4, 9);
    assert_token(next_token(lexer), TOKEN_SEMICOLON, ";", 4, 10);
    assert_token(next_token(lexer), TOKEN_RBRACE, "}", 5, 1);
    assert_token(next_token(lexer), TOKEN_EOF, "", 5, 2);
}

static void test_empty_input() {
    Lexer *lexer = lexer_create("");
    ASSERT_TRUE(lexer != NULL);

    assert_token(next_token(lexer), TOKEN_EOF, "", 1, 1);

    lexer_destroy(lexer);
}

//-------------------------------------------------------------------------------------------------
// Test runner

typedef void (*TestFunction)();

typedef struct {
    const char *name;
    TestFunction function;
} Test;

static Test tests[] = {
    {"identifiers", test_identifiers},
    {"integers", test_integers},
    {"arithmetic operators", test_arithmetic_operators},
    {"comparison operators", test_comparison_operators},
    {"assignment", test_assignment},
    {"punctuation", test_punctuation},
    {"keywords", test_keywords},
    {"whitespace", test_whitespace},
    {"line and column tracking", line_and_column_tracking},
    {"complete program", test_complete_program},
    {"empty input", test_empty_input}
};

int main() {
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