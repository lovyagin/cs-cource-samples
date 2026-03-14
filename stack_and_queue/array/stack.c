/**
 * @file stack.c
 * @brief Реализация стека целых чисел через статический массив.
 *
 * Максимальный размер стека задается константой STACK_MAX_SIZE.
 */

#include "stack.h"
#include <stdlib.h>
#include <string.h>

#ifndef STACK_MAX_SIZE
#define STACK_MAX_SIZE 16  /**< Максимальное количество элементов стека */
#endif

/**
 * @brief Внутренняя структура стека.
 */
struct stack {
    int data[STACK_MAX_SIZE];  /**< Массив для хранения элементов */
    size_t top;                /**< Индекс следующего свободного места (0 если стек пуст) */
};

/* =========================================================
   Создание и уничтожение
   ========================================================= */

stack *stack_create(void)
{
    stack *s = malloc(sizeof(stack));
    if (!s) return NULL;
    s->top = 0;
    return s;
}

stack *stack_copy(const stack *src)
{
    if (!src) return NULL;
    stack *s = stack_create();
    if (!s) return NULL;
    memcpy(s->data, src->data, sizeof(int) * src->top);
    s->top = src->top;
    return s;
}

void stack_free(stack *s)
{
    free(s);
}

/* =========================================================
   Проверка состояния
   ========================================================= */

int stack_is_empty(const stack *s)
{
    return s->top == 0;
}

int stack_is_full(const stack *s)
{
    return s->top >= STACK_MAX_SIZE;
}

/* =========================================================
   Работа с элементами
   ========================================================= */

int stack_push(stack *s, int val)
{
    if (s->top >= STACK_MAX_SIZE) return -1;

    s->data[s->top++] = val;

    return 0;
}

int stack_pop(stack *s, int *val)
{
    if (s->top == 0) return -1;

    --s->top;
    if (val) *val = s->data[s->top];
    
    return 0;
}

int *stack_top(stack *s)
{
    if (s->top == 0) return NULL;

    return s->data + s->top - 1;
}

const int *stack_top_const(const stack *s)
{
    if (s->top == 0) return NULL;

    return s->data + s->top - 1;
}
