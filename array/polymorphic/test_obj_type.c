/**
 * @file test_obj_type.c
 * @brief Unit-тесты для obj_type.
 */

#include "obj_type.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "ASSERT FAILED: %s\n", msg); \
            return 1; \
        } \
    } while (0)

int test_int_basic()
{
    int val = 42;
    obj_type t = obj_mktype_int(&val);

    int *x = obj_create(&t);
    ASSERT(x != NULL, "obj_create int failed");
    ASSERT(*x == 42, "obj_create int value mismatch");

    *x = 100;
    int *y = obj_clone(x, &t);
    ASSERT(y != NULL, "obj_clone int failed");
    ASSERT(*y == 100, "obj_clone int value mismatch");

    int new_val = 200;
    obj_type t2 = obj_mktype_int(&new_val);
    ASSERT(obj_assign(x, &new_val, &t2) != NULL, "obj_assign int failed");
    ASSERT(*x == 200, "obj_assign int value mismatch");

    obj_free(x, &t);
    obj_free(y, &t);

    // type-safe functions
    int *a = obj_int_create(123);
    ASSERT(a && *a == 123, "obj_int_create failed");
    int *b = obj_int_clone(a);
    ASSERT(b && *b == 123, "obj_int_clone failed");
    *b = 456;
    obj_int_assign(a, b);
    ASSERT(*a == 456, "obj_int_assign failed");
    obj_int_free(a);
    obj_int_free(b);

    return 0;
}

int test_double_basic()
{
    double val = 3.14;
    obj_type t = obj_mktype_double(&val);

    double *x = obj_create(&t);
    ASSERT(x != NULL, "obj_create double failed");
    ASSERT(*x == 3.14, "obj_create double value mismatch");

    *x = 2.718;
    double *y = obj_clone(x, &t);
    ASSERT(y != NULL, "obj_clone double failed");
    ASSERT(*y == 2.718, "obj_clone double value mismatch");

    double new_val = 1.618;
    obj_type t2 = obj_mktype_double(&new_val);
    ASSERT(obj_assign(x, &new_val, &t2) != NULL, "obj_assign double failed");
    ASSERT(*x == 1.618, "obj_assign double value mismatch");

    obj_free(x, &t);
    obj_free(y, &t);

    // type-safe
    double *a = obj_double_create(6.28);
    ASSERT(a && *a == 6.28, "obj_double_create failed");
    double *b = obj_double_clone(a);
    ASSERT(b && *b == 6.28, "obj_double_clone failed");
    *b = 9.81;
    obj_double_assign(a, b);
    ASSERT(*a == 9.81, "obj_double_assign failed");
    obj_double_free(a);
    obj_double_free(b);

    return 0;
}

int test_cstring_basic()
{
    const char *val = "hello";
    obj_type t = obj_mktype_cstring(val);

    char **x = obj_create(&t);
    ASSERT(x != NULL, "obj_create cstring failed");
    ASSERT(strcmp(*x, "hello") == 0, "obj_create cstring value mismatch");

    free(*x); // чтобы не перезаписывать strdup, используем clone для проверки
    *x = strdup("world");

    char **y = obj_clone(x, &t);
    ASSERT(y != NULL, "obj_clone cstring failed");
    ASSERT(strcmp(*y, "world") == 0, "obj_clone cstring value mismatch");

    obj_assign(x, y, &t);
    ASSERT(strcmp(*x, "world") == 0, "obj_assign cstring failed");

    obj_free(x, &t);
    obj_free(y, &t);

    // type-safe
    char **a = obj_cstring_create("foo");
    ASSERT(a && strcmp(*a, "foo") == 0, "obj_cstring_create failed");
    char **b = obj_cstring_clone(a);
    ASSERT(b && strcmp(*b, "foo") == 0, "obj_cstring_clone failed");
    free(*b); *b = strdup("bar");
    obj_cstring_assign(a, b);
    ASSERT(strcmp(*a, "bar") == 0, "obj_cstring_assign failed");
    obj_cstring_free(a);
    obj_cstring_free(b);

    return 0;
}

int main(void)
{
    int ret = 0;

    ret |= test_int_basic();
    ret |= test_double_basic();
    ret |= test_cstring_basic();

    if (ret == 0) 
        printf("All tests passed!\n");
    else 
        printf("Some tests failed!\n");

    return ret;
}
