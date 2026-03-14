/**
 * @file stack_test.c
 * @brief Общие тесты стека для любой реализации stack.
 */

#include <CUnit/Basic.h>
#include "stack.h"

/* =========================================================
   Вспомогательные функции
   ========================================================= */

/**
 * @brief Заполняет стек числами 0..n-1.
 *
 * Если стек имеет ограниченную вместимость,
 * заполнение прекращается при первой ошибке push.
 *
 * Если push завершился ошибкой, проверяется,
 * что стек сообщает о переполнении.
 *
 * @return Количество реально вставленных элементов.
 */
static int fill_stack(stack *s, int n)
{
    int inserted = 0;

    for (int i = 0; i < n; i++) {

        int r = stack_push(s, i);

        if (r != 0) {
            CU_ASSERT(stack_is_full(s));
            break;
        }

        inserted++;
    }

    return inserted;
}

/**
 * @brief Проверяет LIFO порядок.
 */
static void check_lifo(stack *s, int n)
{
    for (int i = n - 1; i >= 0; i--) {

        int val = -1;

        CU_ASSERT(stack_pop(s, &val) == 0);
        CU_ASSERT(val == i);
    }
}

/* =========================================================
   Тесты
   ========================================================= */

static void test_create_empty(void)
{
    stack *s = stack_create();

    CU_ASSERT_PTR_NOT_NULL(s);
    CU_ASSERT(stack_is_empty(s));
    CU_ASSERT(stack_top(s) == NULL);

    stack_free(s);
}

static void test_push_top(void)
{
    stack *s = stack_create();

    CU_ASSERT(stack_push(s, 10) == 0);

    int *t = stack_top(s);

    CU_ASSERT_PTR_NOT_NULL(t);
    CU_ASSERT(*t == 10);

    CU_ASSERT(stack_push(s, 20) == 0);

    t = stack_top(s);

    CU_ASSERT_PTR_NOT_NULL(t);
    CU_ASSERT(*t == 20);

    stack_free(s);
}

static void test_lifo_order(void)
{
    stack *s = stack_create();

    int n = fill_stack(s, 100);

    check_lifo(s, n);

    CU_ASSERT(stack_is_empty(s));

    stack_free(s);
}

static void test_pop_empty(void)
{
    stack *s = stack_create();

    int val;
    CU_ASSERT(stack_pop(s, &val) < 0);

    stack_free(s);
}

static void test_copy(void)
{
    stack *s1 = stack_create();

    int n = fill_stack(s1, 50);

    stack *s2 = stack_copy(s1);

    CU_ASSERT_PTR_NOT_NULL(s2);

    check_lifo(s1, n);
    check_lifo(s2, n);

    stack_free(s1);
    stack_free(s2);
}

static void test_interleaved(void)
{
    stack *s = stack_create();

    CU_ASSERT(stack_push(s, 1) == 0);
    CU_ASSERT(stack_push(s, 2) == 0);

    int val;

    CU_ASSERT(stack_pop(s, &val) == 0);
    CU_ASSERT(val == 2);

    CU_ASSERT(stack_push(s, 3) == 0);

    CU_ASSERT(stack_pop(s, &val) == 0);
    CU_ASSERT(val == 3);

    CU_ASSERT(stack_pop(s, &val) == 0);
    CU_ASSERT(val == 1);

    CU_ASSERT(stack_is_empty(s));

    stack_free(s);
}

/* =========================================================
   main
   ========================================================= */

int main(void)
{
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("stack_tests", NULL, NULL);

    CU_add_test(suite, "create_empty", test_create_empty);
    CU_add_test(suite, "push_top", test_push_top);
    CU_add_test(suite, "lifo_order", test_lifo_order);
    CU_add_test(suite, "pop_empty", test_pop_empty);
    CU_add_test(suite, "copy", test_copy);
    CU_add_test(suite, "interleaved", test_interleaved);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
