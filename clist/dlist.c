/**
 * @file dlist.c
 * @brief Реализация двусвязного списка целых чисел.
 *
 * Список хранит указатель на текущий элемент.
 * Поддерживаются переход к следующему и предыдущему элементу,
 * вставка до/после текущего, удаление текущего и получение текущего элемента.
 */

#include "dlist.h"
#include <stdio.h>
#include <stdlib.h>

/* Внутренняя структура узла */
typedef struct dlist_node
{
    int                data;
    struct dlist_node *next;
    struct dlist_node *prev;
} dlist_node;

/* Внутренняя структура списка */
struct dlist
{
    dlist_node *current;
};

/* =========================================================
   Создание и уничтожение
   ========================================================= */

dlist *dlist_create(void)
{
    dlist *lst = malloc(sizeof(dlist));
    if (!lst) return NULL;
    lst->current = NULL;
    return lst;
}

void dlist_free(dlist *lst)
{
    if (lst->current)
    {
        dlist_node *start = lst->current;
        dlist_node *node  = start->next;
        while (node != start)
        {
            dlist_node *tmp = node;
            node            = node->next;
            free(tmp);
        }
        free(start);
    }
    free(lst);
}

dlist *dlist_copy(const dlist *src)
{
    if (!src) return NULL;

    dlist *dst = dlist_create();
    if (!dst) return NULL;

    if (!src->current) return dst; // пустой список

    const dlist_node *start = src->current;
    const dlist_node *node  = start;
    do
    {
        if (dlist_insert_after(dst, node->data) < 0)
        {
            dlist_free(dst);
            return NULL;
        }
        dlist_next(dst);
        node = node->next;
    } while (node != start);

    return dst;
}

/* =========================================================
   Проверка состояния
   ========================================================= */

int dlist_is_empty(const dlist *lst)
{
    return lst->current == NULL;
}

/* =========================================================
   Доступ к текущему элементу
   ========================================================= */

int *dlist_current(dlist *lst)
{
    return lst->current ? &lst->current->data : NULL;
}

const int *dlist_current_const(const dlist *lst)
{
    return lst->current ? &lst->current->data : NULL;
}

int *dlist_next(dlist *lst)
{
    if (!lst->current) return NULL;
    lst->current = lst->current->next;
    return &lst->current->data;
}

int *dlist_prev(dlist *lst)
{
    if (!lst->current) return NULL;
    lst->current = lst->current->prev;
    return &lst->current->data;
}

/* =========================================================
   Модификация списка
   ========================================================= */

int dlist_insert_after(dlist *lst, int val)
{
    dlist_node *node = malloc(sizeof(dlist_node));
    if (!node) return -1;
    node->data = val;

    if (!lst->current)
    {
        node->next = node->prev = node;
        lst->current            = node;
    }
    else
    {
        node->prev               = lst->current;
        node->next               = lst->current->next;
        lst->current->next->prev = node;
        lst->current->next       = node;
    }
    return 0;
}

int dlist_insert_before(dlist *lst, int val)
{
    if (!lst->current) return dlist_insert_after(lst, val);
    lst->current = lst->current->prev;
    return dlist_insert_after(lst, val);
}

int dlist_remove_current(dlist *lst, int *val)
{
    if (!lst->current) return -1;
    dlist_node *node = lst->current;
    if (val) *val = node->data;

    if (node->next == node)
    {
        free(node);
        lst->current = NULL;
        return 0;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
    lst->current     = node->next;
    free(node);
    return 0;
}
