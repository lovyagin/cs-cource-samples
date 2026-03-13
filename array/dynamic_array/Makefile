CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
LDFLAGS = -lcunit

SRC = int_array.c int_array_test.c
OBJ = $(SRC:.c=.o)
TARGET = int_array_test

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)