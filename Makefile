CC ?= gcc
CFLAGS ?= -Wall -Wextra
INCLUDES := -Iinclude

pdlcheck:
	@echo "Compiling PDL-Check"
	$(CC) $(CFLAGS) $(INCLUDES) src/main.c -o pdlcheck

run: pdlcheck
	@echo "Running PDL-Check"
	./pdlcheck

clean:
	rm -f pdlcheck