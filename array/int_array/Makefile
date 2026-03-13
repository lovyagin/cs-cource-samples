CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g $(shell pkg-config --cflags cunit)
LDFLAGS = $(shell pkg-config --libs cunit)

SRC = int_array.c int_array_test.c
OBJ = $(SRC:.c=.o)
TARGET = int_array_test

.PHONY: all clean test

all: $(TARGET) docs

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

int_array.o: int_array.c int_array.h
	$(CC) $(CFLAGS) -c $< -o $@

int_array_test.o: int_array_test.c int_array.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

docs: Doxyfile int_array.h int_array.c
	doxygen Doxyfile
	touch docs