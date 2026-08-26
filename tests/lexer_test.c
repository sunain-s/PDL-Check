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