/**
 * @file clist.c
 * @brief Реализация односвязного циклического списка с указателем на текущий элемент.
 */

#include "clist.h"
#include <stdlib.h>

/* =========================================================
   Внутренние структуры
   ========================================================= */

/**
 * @struct node
 * @brief Определение структуры узла.
 */
typedef struct node
{
    int data; /**< Значение, хранящееся в узле */
    struct node *
        next; /**< Указатель на следующий узел списка (не может быть NULL - список циклический) */
} node;

/**
 * @brief Список
 */
struct clist
{
    node *current; /**< Текущий элемент */
};

/* =========================================================
   Создание и уничтожение
   ========================================================= */

clist *clist_create(void)
{
    clist *lst = malloc(sizeof(clist));
    if (!lst) return NULL;
    lst->current = NULL;
    return lst;
}

void clist_free(clist *lst)
{
    if (lst->current)
    {
        node *start = lst->current;
        node *node  = start->next;
        while (node != start)
        {
            struct node *tmp = node;
            node             = node->next;
            free(tmp);
        }
        free(start);
    }
    free(lst);
}

clist *clist_copy(const clist *src)
{
    clist *dst = clist_create();
    if (!dst) return NULL;

    if (!src->current) return dst;

    node *node = src->current;
    do
    {
        if (clist_insert_after(dst, node->data) != 0)
        {
            clist_free(dst);
            return NULL;
        }
        dst->current = dst->current->next;
        node         = node->next;
    } while (node != src->current);

    dst->current = dst->current->next;
    return dst;
}

/* =========================================================
   Проверка состояния
   ========================================================= */

int clist_is_empty(const clist *lst)
{
    return lst->current == NULL;
}

/* =========================================================
   Работа с текущим элементом
   ========================================================= */

int *clist_current(clist *lst)
{
    return lst->current ? &lst->current->data : NULL;
}

const int *clist_current_const(const clist *lst)
{
    return lst->current ? &lst->current->data : NULL;
}

int *clist_next(clist *lst)
{
    if (!lst->current) return NULL;
    lst->current = lst->current->next;
    return &lst->current->data;
}

int clist_insert_after(clist *lst, int val)
{
    if (!lst) return -1;
    node *node = malloc(sizeof(*node));
    if (!node) return -1;
    node->data = val;
    if (!lst->current)
    {
        node->next   = node;
        lst->current = node;
    }
    else
    {
        node->next         = lst->current->next;
        lst->current->next = node;
    }
    return 0;
}

int clist_remove_after(clist *lst, int *val)
{
    if (!lst->current) return -1;
    node *to_remove = lst->current->next;
    if (to_remove == lst->current)
    {
        if (val) *val = to_remove->data;
        free(to_remove);
        lst->current = NULL;
        return 0;
    }
    lst->current->next = to_remove->next;
    if (val) *val = to_remove->data;
    free(to_remove);
    return 0;
}
