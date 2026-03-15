/**
 * @file queue.c
 * @brief Реализация очереди целых чисел через статический кольцевой буфер.
 *
 * Максимальный размер очереди задается константой QUEUE_MAX_SIZE.
 */

#include "queue.h"
#include <stdlib.h>
#include <string.h>

#ifndef QUEUE_MAX_SIZE
#define QUEUE_MAX_SIZE 16 /**< Максимальное количество элементов очереди */
#endif

/**
 * @brief Внутренняя структура очереди.
 */
struct queue
{
    int    data[QUEUE_MAX_SIZE]; /**< Массив для хранения элементов */
    size_t head;                 /**< Индекс начала очереди */
    size_t
        tail; /**< Индекс конца очереди (следующий после последнего элемента) */
};

/* =========================================================
   Вспомогательные проверки состояния
   ========================================================= */

/* =========================================================
   Создание и уничтожение
   ========================================================= */

queue *queue_create(void)
{
    queue *q = (queue *) malloc(sizeof(queue));
    if (!q) return NULL;
    q->head = 0;
    q->tail = 0;
    return q;
}

queue *queue_copy(const queue *src)
{
    if (!src) return NULL;
    queue *q = queue_create();
    if (!q) return NULL;
    q->head = src->head;
    q->tail = src->tail;
    memcpy(q->data, src->data, sizeof(int) * QUEUE_MAX_SIZE);
    return q;
}

void queue_free(queue *q)
{
    free(q);
}

/* =========================================================
   Проверка состояния
   ========================================================= */

int queue_is_empty(const queue *q)
{
    return q->head == q->tail;
}

int queue_is_full(const queue *q)
{
    return ((q->tail + 1) % QUEUE_MAX_SIZE) == q->head;
}

/* =========================================================
   Работа с элементами
   ========================================================= */

int queue_push(queue *q, int val)
{
    if (queue_is_full(q)) return -1;

    q->data[q->tail] = val;
    q->tail          = (q->tail + 1) % QUEUE_MAX_SIZE;
    return 0;
}

int queue_pop(queue *q, int *val)
{
    if (queue_is_empty(q)) return -1;

    if (val) *val = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_MAX_SIZE;
    return 0;
}

int *queue_head(queue *q)
{
    return queue_is_empty(q) ? NULL : q->data + q->head;
}

int *queue_tail(queue *q)
{
    if (queue_is_empty(q)) return NULL;
    size_t idx = (q->tail == 0) ? QUEUE_MAX_SIZE - 1 : q->tail - 1;
    return q->data + idx;
}

const int *queue_head_const(const queue *q)
{
    return queue_is_empty(q) ? NULL : q->data + q->head;
}

const int *queue_tail_const(const queue *q)
{
    if (queue_is_empty(q)) return NULL;
    size_t idx = (q->tail == 0) ? QUEUE_MAX_SIZE - 1 : q->tail - 1;
    return q->data + idx;
}
