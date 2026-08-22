CC ?= gcc
CFLAGS ?= -Wall -Wextra
INCLUDES := -Iinclude

build:
	@echo "Compiling PDL-Check"
	$(CC) $(CFLAGS) $(INCLUDES) src/main.c -o pdlcheck

run:
	@echo "Running PDL-Check"
	./pdlcheck

clean:
	rm -f pdlcheck