/**
 * @file queue_test.c
 * @brief Общие тесты очереди для любой реализации queue.
 */

#include "queue.h"
#include <CUnit/Basic.h>

/* =========================================================
   Вспомогательные функции
   ========================================================= */

/**
 * @brief Заполняет очередь числами 0..n-1.
 *
 * Если очередь имеет ограниченную вместимость,
 * заполнение прекращается при первой ошибке push.
 *
 * Если push завершился ошибкой, дополнительно проверяется,
 * что очередь сообщает о переполнении.
 *
 * @return Количество реально вставленных элементов.
 */
static int fill_queue(queue *q, int n)
{
    int inserted = 0;

    for (int i = 0; i < n; i++)
    {

        int r = queue_push(q, i);

        if (r != 0)
        {
            /* очередь не приняла элемент → должна быть полной */
            CU_ASSERT(queue_is_full(q));
            break;
        }

        inserted++;
    }

    return inserted;
}

/**
 * @brief Проверяет FIFO порядок.
 */
static void check_fifo(queue *q, int n)
{
    for (int i = 0; i < n; i++)
    {
        int val = -1;
        CU_ASSERT(queue_pop(q, &val) == 0);
        CU_ASSERT(val == i);
    }
}

/* =========================================================
   Тесты
   ========================================================= */

static void test_create_empty(void)
{
    queue *q = queue_create();

    CU_ASSERT_PTR_NOT_NULL(q);
    CU_ASSERT(queue_is_empty(q));
    CU_ASSERT(queue_head(q) == NULL);
    CU_ASSERT(queue_tail(q) == NULL);

    queue_free(q);
}

static void test_push_head_tail(void)
{
    queue *q = queue_create();

    CU_ASSERT(queue_push(q, 10) == 0);

    int *h = queue_head(q);
    int *t = queue_tail(q);

    CU_ASSERT_PTR_NOT_NULL(h);
    CU_ASSERT_PTR_NOT_NULL(t);
    CU_ASSERT(*h == 10);
    CU_ASSERT(*t == 10);

    CU_ASSERT(queue_push(q, 20) == 0);

    h = queue_head(q);
    t = queue_tail(q);

    CU_ASSERT(*h == 10);
    CU_ASSERT(*t == 20);

    queue_free(q);
}

static void test_fifo_order(void)
{
    queue *q = queue_create();

    int n = fill_queue(q, 100);

    check_fifo(q, n);

    CU_ASSERT(queue_is_empty(q));

    queue_free(q);
}

static void test_pop_empty(void)
{
    queue *q = queue_create();

    int val;
    CU_ASSERT(queue_pop(q, &val) < 0);

    queue_free(q);
}

static void test_copy(void)
{
    queue *q1 = queue_create();

    int n = fill_queue(q1, 50);

    queue *q2 = queue_copy(q1);

    CU_ASSERT_PTR_NOT_NULL(q2);

    check_fifo(q1, n);
    check_fifo(q2, n);

    queue_free(q1);
    queue_free(q2);
}

static void test_interleaved(void)
{
    queue *q = queue_create();

    CU_ASSERT(queue_push(q, 1) == 0);
    CU_ASSERT(queue_push(q, 2) == 0);

    int val;

    CU_ASSERT(queue_pop(q, &val) == 0);
    CU_ASSERT(val == 1);

    CU_ASSERT(queue_push(q, 3) == 0);

    CU_ASSERT(queue_pop(q, &val) == 0);
    CU_ASSERT(val == 2);

    CU_ASSERT(queue_pop(q, &val) == 0);
    CU_ASSERT(val == 3);

    CU_ASSERT(queue_is_empty(q));

    queue_free(q);
}

/* =========================================================
   main
   ========================================================= */

int main(void)
{
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("queue_tests", NULL, NULL);

    CU_add_test(suite, "create_empty", test_create_empty);
    CU_add_test(suite, "push_head_tail", test_push_head_tail);
    CU_add_test(suite, "fifo_order", test_fifo_order);
    CU_add_test(suite, "pop_empty", test_pop_empty);
    CU_add_test(suite, "copy", test_copy);
    CU_add_test(suite, "interleaved", test_interleaved);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}