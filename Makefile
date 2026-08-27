# PDL-Check Makefile

# Compiler and flags
CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O2
INCLUDES := -Iinclude

# Paths and targets
SRCDIR := src
BINDIR := bin
BUILDDIR := $(BINDIR)/build
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/dep

TARGET := $(BINDIR)/pdlcheck
TESTDIR := $(BINDIR)/tests
TEST_SRCS := $(wildcard tests/*_test.c)
TEST_TARGETS := $(TEST_SRCS:tests/%.c=$(TESTDIR)/%)

# Source files
SRCS := \
	$(SRCDIR)/main.c \
	$(SRCDIR)/lexer.c

# Convert src/*.c -> bin/build/obj/*.o
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Convert src/*.c -> bin/build/dep/*.d
DEPS := $(SRCS:$(SRCDIR)/%.c=$(DEPDIR)/%.d)

.PHONY: all build run test clean distclean rebuild

# Default target
all: $(TARGET)

# Explicit build target
build: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	@echo "Linking PDL-Check"
	@$(CC) $(OBJS) -o $@
	@echo "Build complete: $(TARGET)"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/$*.d)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INCLUDES) \
		-MMD -MP \
		-MF $(DEPDIR)/$*.d \
		-c $< -o $@

# Include generated dependency files
-include $(DEPS)

# Run
run: $(TARGET)
	@echo "Running PDL-Check"
	@./$(TARGET)

# Build and run test programs
test: $(TEST_TARGETS)
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test"; \
		./$$test || exit 1; \
	done

# Link test programs with the lexer, without the application's main function
$(TESTDIR)/%: tests/%.c $(OBJDIR)/lexer.o
	@mkdir -p $(TESTDIR)
	@echo "Building test $@"
	@$(CC) $(CFLAGS) $(INCLUDES) $< $(OBJDIR)/lexer.o -o $@

# Remove intermediate build files
clean:
	@echo "Cleaning intermediate build artifacts"
	@rm -rf $(BUILDDIR)

# Remove everything built
distclean:
	@echo "Cleaning all build artifacts"
	@rm -rf $(BINDIR)

# Full rebuild
rebuild: distclean build
	@echo "Rebuild complete"
