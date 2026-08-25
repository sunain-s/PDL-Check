# PDL-Check Makefile

# Compiler and flags
CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -O2
INCLUDES := -Iinclude

# Paths and targets
SRCDIR := src
BINDIR := bin
BUILDDIR := $(BINDIR)/build
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/dep

TARGET := $(BINDIR)/pdlcheck

# Source files
SRCS := \
	$(SRCDIR)/main.c \
	$(SRCDIR)/lexer.c

# Convert src/*.c -> bin/build/obj/*.o
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Convert src/*.c -> bin/build/dep/*.d
DEPS := $(SRCS:$(SRCDIR)/%.c=$(DEPDIR)/%.d)


.PHONY: all build run clean distclean rebuild


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
