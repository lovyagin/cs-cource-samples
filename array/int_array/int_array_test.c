/**4
 * @file int_array_test.c
 * @brief Unit-тесты для int_array с демонстрацией работы политик.
 *
 * Проверяется:
 * - create, create_copy, assign, resize
 * - push_back, pop_back, remove_swap
 * - корректность размера и вместимости
 * - иллюстративный вывод состояния массива
 */

#include "int_array.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>

/* ===========================
   Вспомогательные функции
   =========================== */

/**
 * @brief Выводит содержимое массива, его размер и вместимость.
 */
static void print_array(const int_array *arr, const char *msg)
{
    printf("%s:\tsize=%zu, capacity=%zu,\telems=[",
           msg,
           int_array_get_size(arr),
           int_array_get_capacity(arr));
    for (size_t i = 0; i < int_array_get_size(arr); ++i)
        printf("%d%s",
               *int_array_get_const(arr, i),
               (i < int_array_get_size(arr) - 1) ? ", " : "");
    printf("]\n");
}

/**
 * @brief Политика минимальной вместимости 3.
 */
static size_t policy_min3(size_t size)
{
    return size < 3 ? 3 : int_array_policy_default_grow(size);
}

/* ===========================
   Общие тесты
   =========================== */

void test_create_free(void)
{
    int_array *arr = int_array_create();
    CU_ASSERT_PTR_NOT_NULL(arr);
    CU_ASSERT(int_array_get_size(arr) == 0);
    CU_ASSERT(int_array_get_capacity(arr) >= 1);

    int_array_free(arr);
}

void test_create_copy_resize(void)
{
    int_array *arr = int_array_create();
    CU_ASSERT_PTR_NOT_NULL(arr);

    // resize
    arr = int_array_resize(arr, 5);
    CU_ASSERT_PTR_NOT_NULL(arr);
    for (size_t i = 0; i < 5; ++i) int_array_set(arr, i, (int) (i * 10));

    // create_copy
    int_array *copy = int_array_create_copy(arr);
    CU_ASSERT_PTR_NOT_NULL(copy);
    CU_ASSERT(int_array_get_size(copy) == int_array_get_size(arr));
    for (size_t i = 0; i < int_array_get_size(copy); ++i)
        CU_ASSERT(int_array_get(copy, i) == int_array_get(arr, i));

    int_array_free(arr);
    int_array_free(copy);
}

void test_assign(void)
{
    int_array *a = int_array_create();
    int_array *b = int_array_create();
    CU_ASSERT_PTR_NOT_NULL(a);
    CU_ASSERT_PTR_NOT_NULL(b);

    CU_ASSERT_PTR_NOT_NULL(int_array_push_back(a, 42));
    CU_ASSERT_PTR_NOT_NULL(int_array_push_back(a, 24));

    CU_ASSERT_PTR_NOT_NULL(int_array_push_back(b, 1));
    CU_ASSERT_PTR_NOT_NULL(int_array_push_back(b, 2));
    CU_ASSERT_PTR_NOT_NULL(int_array_push_back(b, 3));

    b = int_array_assign(b, a);
    CU_ASSERT_PTR_NOT_NULL(b);
    CU_ASSERT(int_array_get_size(b) == int_array_get_size(a));
    for (size_t i = 0; i < int_array_get_size(a); ++i)
        CU_ASSERT(int_array_get(a, i) == int_array_get(b, i));

    int_array_free(a);
    int_array_free(b);
}

/* ===========================
   Иллюстративные тесты
   =========================== */

static void demo_array_operations(int_array_policy grow,
                                  int_array_policy shrink,
                                  const char      *label)
{
    int_array *arr = int_array_create();
    CU_ASSERT_PTR_NOT_NULL(arr);

    // Устанавливаем политики
    int_array_set_grow_policy(arr, grow);
    int_array_set_shrink_policy(arr, shrink);

    printf("\n--- Demo: %s ---\n", label);

    // push_back
    for (int i = 1; i <= 15; ++i)
    {
        arr = int_array_push_back(arr, i * 15);
        CU_ASSERT_PTR_NOT_NULL(arr);
        print_array(arr, "After push_back");
    }

    // remove_swap элементы
    arr = int_array_remove_swap(arr, 1);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(1)");

    arr = int_array_remove_swap(arr, 0);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(0)");

    arr = int_array_remove_swap(arr, 6);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(6)");

    arr = int_array_remove_swap(arr, 4);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(4)");

    arr = int_array_remove_swap(arr, 5);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(5)");

    arr = int_array_remove_swap(arr, 1);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(1)");

    arr = int_array_remove_swap(arr, int_array_get_size(arr) - 1);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(lst)");

    arr = int_array_remove_swap(arr, 2);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(2)");

    arr = int_array_remove_swap(arr, 0);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(1)");

    arr = int_array_remove_swap(arr, int_array_get_size(arr) - 1);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(lst)");

    arr = int_array_remove_swap(arr, 2);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(1)");

    arr = int_array_remove_swap(arr, 0);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(1)");

    arr = int_array_remove_swap(arr, int_array_get_size(arr) - 1);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(lst)");

    arr = int_array_remove_swap(arr, 1);
    CU_ASSERT_PTR_NOT_NULL(arr);
    print_array(arr, "After remove_swap(1)");

    int_array_free(arr);
}

void test_demo_default_policy(void)
{
    demo_array_operations(int_array_policy_default_grow,
                          int_array_policy_default_shrink,
                          "default policy");
}

void test_demo_min3_policy(void)
{
    demo_array_operations(policy_min3,
                          int_array_policy_default_shrink,
                          "min_capacity=3 policy");
}

/* ===========================
   main
   =========================== */
int main(void)
{
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    CU_pSuite suite = CU_add_suite("int_array_suite", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_ADD_TEST(suite, test_create_free);
    CU_ADD_TEST(suite, test_create_copy_resize);
    CU_ADD_TEST(suite, test_assign);
    CU_ADD_TEST(suite, test_demo_default_policy);
    CU_ADD_TEST(suite, test_demo_min3_policy);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
