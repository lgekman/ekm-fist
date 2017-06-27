
O ?= /tmp/$(USER)-fist
SRC := fist.c fist-filters.c fist-test.c
OBJ := $(SRC:%.c=$(O)/%.o)

.PHONY: all clean test

CC := gcc

$(O)/%.o: %.c
	$(CC) -c -g -Wall -o $@ $(CFLAGS) $<

all: $(O) $(O)/fist-test

test: all
	$(O)/fist-test

$(O)/fist-test: $(OBJ)
	$(CC) -g -o $@ $(OBJ)

$(OBJ): fist.h Makefile

$(O):
	mkdir -p $(O)

clean:
	rm -f $(OBJ) $(O)/fist-test
