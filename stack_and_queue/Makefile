CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude $(shell pkg-config --cflags cunit)
LDFLAGS = $(shell pkg-config --libs cunit)

# =========================================================
# Тестовые программы
# =========================================================

ARRAY_QUEUE_TEST = queue_test_array
ARRAY_STACK_TEST = stack_test_array

LIST_QUEUE_TEST = queue_test_list
LIST_STACK_TEST = stack_test_list

all: test docs

# =========================================================
# Сборка array реализации
# =========================================================

queue_test_array: tests/queue_test.c array/queue.c include/queue.h
	$(CC) $(CFLAGS) tests/queue_test.c array/queue.c -o $@ $(LDFLAGS)

stack_test_array: tests/stack_test.c array/stack.c include/stack.h
	$(CC) $(CFLAGS) tests/stack_test.c array/stack.c -o $@ $(LDFLAGS)

# =========================================================
# Сборка list реализации
# =========================================================

queue_test_list: tests/queue_test.c list/queue.c list/node.h include/queue.h
	$(CC) $(CFLAGS) tests/queue_test.c list/queue.c -o $@ $(LDFLAGS)

stack_test_list: tests/stack_test.c list/stack.c list/node.h include/stack.h
	$(CC) $(CFLAGS) tests/stack_test.c list/stack.c -o $@ $(LDFLAGS)

# =========================================================
# Запуск тестов
# =========================================================

test-array: $(ARRAY_QUEUE_TEST) $(ARRAY_STACK_TEST)
	./$(ARRAY_QUEUE_TEST)
	./$(ARRAY_STACK_TEST)

test-list: $(LIST_QUEUE_TEST) $(LIST_STACK_TEST)
	./$(LIST_QUEUE_TEST)
	./$(LIST_STACK_TEST)

test: test-array test-list

# =========================================================
# Документация
# =========================================================

docs: include/stack.h include/queue.h
	doxygen Doxyfile
	touch docs

# =========================================================
# Очистка
# =========================================================

clean:
	rm -f queue_test_array stack_test_array
	rm -f queue_test_list stack_test_list
	rm -rf docs

