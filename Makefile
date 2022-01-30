BIN = signal-compiler
SRCS = $(wildcard *.c)

CFLAGS += -Wall -Wextra -Werror -pedantic -std=c99

$(BIN): $(SRCS)
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(BIN)
