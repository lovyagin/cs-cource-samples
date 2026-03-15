/**
 * @file test_array.c
 * @brief Unit-тесты динамического массива
 */

#include "array.h"
#include "obj_type.h"

#include <stdio.h>
#include <string.h>

#define ASSERT(cond, msg)                                                 \
    do                                                                    \
    {                                                                     \
        if (!(cond))                                                      \
        {                                                                 \
            fprintf(stderr, "ASSERT FAILED: %s\n", msg);                  \
            return 1;                                                     \
        }                                                                 \
    } while (0)

/* =========================================================
   Тест массива int
   ========================================================= */

int test_array_int()
{
    array *a = array_create(&obj_int);
    ASSERT(a, "create int array");

    ASSERT(array_resize(a, 10), "resize int");

    for (int i = 0; i < 10; i++) ASSERT(array_set(a, i, &i), "set int");

    for (int i = 0; i < 10; i++)
    {
        int *v = array_at(a, i);
        ASSERT(v && *v == i, "get int");
    }

    array *b = array_create_copy(a);
    ASSERT(b, "copy int array");

    for (int i = 0; i < 10; i++)
    {
        int *v = array_at(b, i);
        ASSERT(v && *v == i, "copy check");
    }

    array_free(a);
    array_free(b);

    return 0;
}

/* =========================================================
   Тест массива double
   ========================================================= */

int test_array_double()
{
    array *a = array_create(&obj_double);
    ASSERT(a, "create double array");

    ASSERT(array_resize(a, 8), "resize double");

    for (int i = 0; i < 8; i++)
    {
        double v = i * 0.5;
        ASSERT(array_set(a, i, &v), "set double");
    }

    for (int i = 0; i < 8; i++)
    {
        double *v = array_at(a, i);
        ASSERT(v && *v == i * 0.5, "get double");
    }

    array_free(a);

    return 0;
}

/* =========================================================
   Тест массива строк
   ========================================================= */

int test_array_string()
{
    array *a = array_create(&obj_cstring);
    ASSERT(a, "create string array");

    ASSERT(array_resize(a, 3), "resize string");

    const char *s1 = "alpha";
    const char *s2 = "beta";
    const char *s3 = "gamma";

    ASSERT(array_set(a, 0, &s1), "set s1");
    ASSERT(array_set(a, 1, &s2), "set s2");
    ASSERT(array_set(a, 2, &s3), "set s3");

    ASSERT(strcmp(*(char **) array_at(a, 0), "alpha") == 0, "get s1");
    ASSERT(strcmp(*(char **) array_at(a, 1), "beta") == 0, "get s2");
    ASSERT(strcmp(*(char **) array_at(a, 2), "gamma") == 0, "get s3");

    array *b = array_create_copy(a);
    ASSERT(b, "copy string array");

    ASSERT(strcmp(*(char **) array_at(b, 1), "beta") == 0, "copy check");

    array_free(a);
    array_free(b);

    return 0;
}

/* =========================================================
   Тест двумерного массива int
   ========================================================= */

int test_array_2d_int()
{
    obj_type row_type = obj_mktype_array(&obj_int);

    array *matrix = array_create(&row_type);
    ASSERT(matrix, "create matrix");

    ASSERT(array_resize(matrix, 3), "resize rows");

    for (int r = 0; r < 3; r++)
    {
        array *row = array_at(matrix, r);
        ASSERT(row, "row access");

        ASSERT(array_resize(row, 4), "resize row");

        for (int c = 0; c < 4; c++)
        {
            int v = r * 10 + c;
            ASSERT(array_set(row, c, &v), "set matrix value");
        }
    }

    for (int r = 0; r < 3; r++)
    {
        array *row = array_at(matrix, r);

        for (int c = 0; c < 4; c++)
        {
            int *v = array_at(row, c);
            ASSERT(*v == r * 10 + c, "matrix check");
        }
    }

    array_free(matrix);

    return 0;
}

/* =========================================================
   Тест двумерного массива строк
   ========================================================= */

int test_array_2d_string()
{
    obj_type row_type = obj_mktype_array(&obj_cstring);

    array *matrix = array_create(&row_type);
    ASSERT(matrix, "create string matrix");

    ASSERT(array_resize(matrix, 2), "resize rows");

    for (int r = 0; r < 2; r++)
    {
        array *row = array_at(matrix, r);
        ASSERT(row, "row access");

        ASSERT(array_resize(row, 2), "resize row");

        const char *s1 = "hello";
        const char *s2 = "world";

        ASSERT(array_set(row, 0, &s1), "set str1");
        ASSERT(array_set(row, 1, &s2), "set str2");
    }

    for (int r = 0; r < 2; r++)
    {
        array *row = array_at(matrix, r);

        ASSERT(strcmp(*(char **) array_at(row, 0), "hello") == 0,
               "check1");
        ASSERT(strcmp(*(char **) array_at(row, 1), "world") == 0,
               "check2");
    }

    array_free(matrix);

    return 0;
}

/* =========================================================
   MAIN
   ========================================================= */

int main()
{
    int ret = 0;

    ret |= test_array_int();
    ret |= test_array_double();
    ret |= test_array_string();
    ret |= test_array_2d_int();
    ret |= test_array_2d_string();

    if (ret == 0)
        printf("ALL ARRAY TESTS PASSED\n");
    else
        printf("ARRAY TEST FAILED\n");

    return ret;
}
