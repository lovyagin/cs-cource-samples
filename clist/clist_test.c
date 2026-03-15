#include "clist.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Выводит элементы циклического списка начиная с текущего.
 *
 * Печатает текущий элемент в квадратных скобках.
 *
 * @param lst Список.
 */
void clist_print(clist *lst)
{
    if (!lst || clist_is_empty(lst))
    {
        printf("(empty)\n");
        return;
    }

    int *start = clist_current(lst);
    if (!start) return;

    int *val = start;
    do
    {
        printf("%d ", *val);
        val = clist_next(lst);
    } while (val != start);
    printf("\n");
}

/**
 * @brief Тест базовых операций: вставка, переход, удаление.
 */
void test_basic_operations()
{
    printf("=== Test: basic operations ===\n");

    clist *lst = clist_create();

    clist_insert_after(lst, 10);
    clist_insert_after(lst, 20);
    clist_insert_after(lst, 30);

    printf("After inserts:\n");
    clist_print(lst);

    clist_next(lst);
    printf("After next():\n");
    clist_print(lst);

    clist_remove_after(lst, NULL);
    printf("After remove_after():\n");
    clist_print(lst);

    clist_free(lst);
}

/**
 * @brief Тест копирования и независимости копии.
 */
void test_copy_independence()
{
    printf("=== Test: copy independence ===\n");

    clist *lst = clist_create();
    for (int i = 1; i <= 5; ++i) clist_insert_after(lst, i);

    printf("Original list:\n");
    clist_print(lst);

    clist *copy = clist_copy(lst);
    printf("Copied list:\n");
    clist_print(copy);

    // Изменяем оригинал
    clist_insert_after(lst, 99);
    printf("Original list after insert 99:\n");
    clist_print(lst);
    printf("Copied list remains unchanged:\n");
    clist_print(copy);

    clist_free(lst);
    clist_free(copy);
}

int main(void)
{
    test_basic_operations();
    test_copy_independence();
    return 0;
}
