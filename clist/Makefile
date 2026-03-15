CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = clist_test

SRC = clist_test.c clist.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean test

all: $(TARGET) docs

docs: clist.h
	doxygen Doxyfile
	touch docs

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c clist.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
	rm -rf docs

