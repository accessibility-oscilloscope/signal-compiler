BIN = signal-compiler
SRCS = $(wildcard *.c)

CFLAGS += -Wall -Wextra -Werror -pedantic -std=c99
LDFLAGS += -lm

$(BIN): $(SRCS)
	$(CC) $^ $(CFLAGS) -lm $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(BIN)
