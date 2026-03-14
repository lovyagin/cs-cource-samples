/**
 * @file stack.c
 * @brief Реализация стека целых чисел через односвязный список.
 */

#include "stack.h"
#include "node.h"

#include <stdlib.h>

/**
 * @brief Внутренняя структура стека.
 */
struct stack {

    node *top; /**< Указатель на вершину стека */

};

/* =========================================================
   Создание и уничтожение
   ========================================================= */

stack *stack_create(void)
{
    stack *s = malloc(sizeof(stack));
    if (!s) return NULL;

    s->top = NULL;
    return s;
}

stack *stack_copy(const stack *src)
{
    if (!src) return NULL;

    stack *s = stack_create();
    if (!s) return NULL;

    node **dst_link = &s->top;

    for (node *n = src->top; n; n = n->next) 
    {
        node *new_node = malloc(sizeof(node));
        if (!new_node)
        {
            stack_free(s);
            return NULL;
        }

        new_node->data = n->data;
        new_node->next = NULL;

        *dst_link = new_node;
        dst_link = &new_node->next;
    }

    return s;
}

void stack_free(stack *s)
{
    if (!s) return;

    node *n = s->top;

    while (n) 
    {
        node *next = n->next;
        free(n);
        n = next;
    }

    free(s);
}

/* =========================================================
   Проверка состояния
   ========================================================= */

int stack_is_empty(const stack *s)
{
    return s->top == NULL;
}

int stack_is_full(const stack *s)
{
    (void)s;
    return 0; /* динамический стек не имеет фиксированного ограничения */
}

/* =========================================================
   Работа с элементами
   ========================================================= */

int stack_push(stack *s, int val)
{
    node *n = malloc(sizeof(node));
    if (!n) return -1;

    n->data = val;
    n->next = s->top;
    s->top = n;

    return 0;
}

int stack_pop(stack *s, int *val)
{
    if (!s->top) return -1;

    node *n = s->top;

    if (val) *val = n->data;

    s->top = n->next;
    free(n);

    return 0;
}

int *stack_top(stack *s)
{
    return s->top ? &s->top->data : NULL;
}

const int *stack_top_const(const stack *s)
{
    return s->top ? &s->top->data : NULL;
}
