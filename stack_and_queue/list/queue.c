/**
 * @file queue.c
 * @brief Реализация очереди целых чисел через односвязный список.
 */

#include "queue.h"
#include "node.h"

#include <stdlib.h>

/**
 * @brief Внутренняя структура очереди.
 */
struct queue
{
    node *head;  /**< Первый элемент очереди */
    node *tail;  /**< Последний элемент очереди */
};

/* =========================================================
   Создание и уничтожение
   ========================================================= */

queue *queue_create(void)
{
    queue *q = malloc(sizeof(queue));
    if (!q) return NULL;

    q->head = NULL;
    q->tail = NULL;

    return q;
}

queue *queue_copy(const queue *src)
{
    if (!src) return NULL;

    queue *q = queue_create();
    if (!q) return NULL;

    for (node *n = src->head; n != NULL; n = n->next) 
    {
        if (queue_push(q, n->data) != 0)
        {
            queue_free(q);
            return NULL;
        }
    }

    return q;
}

void queue_free(queue *q)
{
    if (!q) return;

    node *n = q->head;

    while (n)
    {
        node *next = n->next;
        free(n);
        n = next;
    }

    free(q);
}

/* =========================================================
   Проверка состояния
   ========================================================= */

int queue_is_empty(const queue *q)
{
    return q->head == NULL;
}

int queue_is_full(const queue *q)
{
    (void)q;
    return 0; /* динамическая очередь не имеет фиксированного ограничения */
}

/* =========================================================
   Работа с элементами
   ========================================================= */

int queue_push(queue *q, int val)
{
    node *n = malloc(sizeof(node));
    if (!n) return -1;

    n->data = val;
    n->next = NULL;

    if (q->tail)
        q->tail->next = n;
    else
        q->head = n;

    q->tail = n;

    return 0;
}

int queue_pop(queue *q, int *val)
{
    if (!q->head) return -1;

    node *n = q->head;

    if (val) *val = n->data;

    q->head = n->next;

    if (!q->head)
        q->tail = NULL;

    free(n);

    return 0;
}

int *queue_head(queue *q)
{
    return q->head ? &q->head->data : NULL;
}

int *queue_tail(queue *q)
{
    return q->tail ? &q->tail->data : NULL;
}

const int *queue_head_const(const queue *q)
{
    return q->head ? &q->head->data : NULL;
}

const int *queue_tail_const(const queue *q)
{
    return q->tail ? &q->tail->data : NULL;
}
