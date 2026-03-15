/**
 * @file int_array.c
 * @brief Реализация динамического массива целых чисел.
 */

#include "int_array.h"

#include <stdlib.h>
#include <string.h>

/* =========================================================
   Внутренняя структура массива
   ========================================================= */

/**
 * @brief Определение структуры массива.
 *
 * Скрыта от пользователя (инкапсуляция).
 */
struct int_array
{
    size_t size;
    size_t capacity;

    int_array_policy grow_policy;
    int_array_policy shrink_policy;

    int *data;
};

/* =========================================================
   Политики по умолчанию
   ========================================================= */

size_t int_array_policy_default_grow(size_t required_size)
{
    return required_size ? required_size * 2 : 1;
}

size_t int_array_policy_default_shrink(size_t size)
{
    return size * 4;
}

/* =========================================================
   Внутренние функции модуля (скрыты от пользователя)
   ========================================================= */

/** 
 * @brief Изменяет размер выделенного блока данных массива
 *
 * Явно изменяет вместимость массива.
 * Может установить вместимость ниже размера (удаление элементов).
 *
 * @return 1 при успехе, 0 при ошибке
 */
static int int_array_realloc(int_array *arr, size_t new_capacity)
{
    int *new_data = realloc(arr->data, new_capacity * sizeof(int));

    if (!new_data) return 0;

    arr->data     = new_data;
    arr->capacity = new_capacity;

    return 1;
}

/* =========================================================
   Управление вместимостью
   ========================================================= */

int_array *int_array_adjust_capacity(int_array *arr)
{
    size_t desired = arr->grow_policy(arr->size);

    if (desired == arr->capacity) return arr;

    if (!int_array_realloc(arr, desired)) return NULL;

    return arr;
}

int_array *int_array_set_capacity(int_array *arr, size_t new_capacity)
{
    if (new_capacity < arr->size) return NULL;

    if (new_capacity == arr->capacity) return arr;

    if (!int_array_realloc(arr, new_capacity)) return NULL;

    return arr;
}

/* =========================================================
   Создание и уничтожение
   ========================================================= */

int_array *int_array_create(void)
{
    int_array *arr = malloc(sizeof(int_array));

    if (!arr) return NULL;

    arr->size     = 0;
    arr->capacity = 0;
    arr->data     = NULL;

    arr->grow_policy   = int_array_policy_default_grow;
    arr->shrink_policy = int_array_policy_default_shrink;

    if (!int_array_adjust_capacity(arr))
    {
        free(arr);
        return NULL;
    }

    return arr;
}

void int_array_free(int_array *arr)
{
    if (!arr) return;

    free(arr->data);
    free(arr);
}

/* =========================================================
   Копирование
   ========================================================= */

int_array *int_array_create_copy(const int_array *src)
{
    int_array *dst = int_array_create();

    if (!dst) return NULL;

    dst->grow_policy   = src->grow_policy;
    dst->shrink_policy = src->shrink_policy;

    dst->size = src->size;

    if (!int_array_adjust_capacity(dst))
    {
        int_array_free(dst);
        return NULL;
    }

    memcpy(dst->data, src->data, src->size * sizeof(int));

    return dst;
}

int_array *int_array_assign(int_array *dst, const int_array *src)
{
    if (dst == src) return dst;

    dst->grow_policy   = src->grow_policy;
    dst->shrink_policy = src->shrink_policy;

    dst->size = src->size;

    if (!int_array_adjust_capacity(dst)) return NULL;

    memcpy(dst->data, src->data, src->size * sizeof(int));

    return dst;
}

/* =========================================================
   Размер
   ========================================================= */

size_t int_array_get_size(const int_array *arr)
{
    return arr->size;
}

size_t int_array_get_capacity(const int_array *arr)
{
    return arr->capacity;
}

/* =========================================================
   Изменение размера
   ========================================================= */

int_array *int_array_resize(int_array *arr, size_t new_size)
{
    size_t old_size = arr->size;

    arr->size = new_size;

    if (new_size > arr->capacity
        || (new_size < old_size
            && arr->capacity > arr->shrink_policy(new_size)))
    {
        if (!int_array_adjust_capacity(arr))
        {
            arr->size = old_size;
            return NULL;
        }
    }

    return arr;
}

/* =========================================================
   Политики
   ========================================================= */

int_array *int_array_set_grow_policy(int_array       *arr,
                                     int_array_policy policy)
{
    int_array_policy old = arr->grow_policy;

    arr->grow_policy = policy;

    if (!int_array_adjust_capacity(arr))
    {
        arr->grow_policy = old;
        return NULL;
    }

    return arr;
}

int_array *int_array_set_shrink_policy(int_array       *arr,
                                       int_array_policy policy)
{
    int_array_policy old = arr->shrink_policy;

    arr->shrink_policy = policy;

    if (!int_array_adjust_capacity(arr))
    {
        arr->shrink_policy = old;
        return NULL;
    }

    return arr;
}

/* =========================================================
   Доступ к элементам
   ========================================================= */

int *int_array_at(int_array *arr, size_t index)
{
    return index < arr->size ? arr->data + index : NULL;
}

const int *int_array_at_const(const int_array *arr, size_t index)
{
    return index < arr->size ? arr->data + index : NULL;
}

int *int_array_get(int_array *arr, size_t index)
{
    return arr->data + index;
}

const int *int_array_get_const(const int_array *arr, size_t index)
{
    return arr->data + index;
}

int_array *int_array_set(int_array *arr, size_t index, int value)
{
    if (index >= arr->size) return NULL;

    arr->data[index] = value;

    return arr;
}

int_array *int_array_push_back(int_array *arr, int value)
{
    size_t old_size = int_array_get_size(arr);

    if (!int_array_resize(arr, old_size + 1)) return NULL;

    arr->data[old_size] = value;

    return arr;
}

int_array *int_array_pop_back(int_array *arr)
{
    size_t old_size = int_array_get_size(arr);

    if (old_size == 0) return NULL;

    return int_array_resize(arr, old_size - 1);
}

int_array *int_array_remove_swap(int_array *arr, size_t index)
{
    size_t old_size = int_array_get_size(arr);
    if (index >= old_size) return NULL;

    if (old_size == 1) return int_array_resize(arr, 0);

    int last_value = *int_array_get(arr, old_size - 1);

    if (!int_array_resize(arr, old_size - 1)) return NULL;

    if (index != old_size - 1) arr->data[index] = last_value;

    return arr;
}
