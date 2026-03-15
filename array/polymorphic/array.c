/**
 * @file array.c
 * @brief Реализация универсального полиморфного динамического массива
 */

#include "array.h"

#include <stdlib.h>
#include <string.h>

/* =========================================================
   Внутренняя структура массива
   ========================================================= */

struct array
{
    const obj_type *type;

    size_t size;
    size_t capacity;

    size_t min_capacity;

    array_policy grow_policy;
    array_policy shrink_policy;

    void *data;
};

/* =========================================================
   Вспомогательные функции
   ========================================================= */

static void *array_elem(array *arr, size_t i)
{
    return (char *) arr->data + i * arr->type->size;
}

static const void *array_elem_const(const array *arr, size_t i)
{
    return (const char *) arr->data + i * arr->type->size;
}

/* =========================================================
   Политики по умолчанию
   ========================================================= */

size_t array_policy_default_grow(size_t required_size)
{
    return required_size ? required_size * 2 : 1;
}

size_t array_policy_default_shrink(size_t current_size)
{
    return current_size * 4;
}

/* =========================================================
   Создание и уничтожение
   ========================================================= */

array *array_create(const obj_type *el_type)
{
    array *arr = malloc(sizeof(array));
    if (!arr) return NULL;

    arr->type = el_type;

    arr->size         = 0;
    arr->capacity     = 0;
    arr->min_capacity = 0;

    arr->grow_policy   = array_policy_default_grow;
    arr->shrink_policy = array_policy_default_shrink;

    arr->data = NULL;

    return arr;
}

void array_free(array *arr)
{
    if (!arr) return;

    for (size_t i = 0; i < arr->size; i++)
        arr->type->destroy(array_elem(arr, i), arr->type->context);

    free(arr->data);
    free(arr);
}

/* =========================================================
   Копирование
   ========================================================= */

array *array_create_copy(const array *src)
{
    array *dst = array_create(src->type);
    if (!dst) return NULL;

    dst->min_capacity  = src->min_capacity;
    dst->grow_policy   = src->grow_policy;
    dst->shrink_policy = src->shrink_policy;

    if (!array_resize(dst, src->size))
    {
        array_free(dst);
        return NULL;
    }

    for (size_t i = 0; i < src->size; i++)
    {
        if (src->type->copy(array_elem(dst, i),
                            array_elem_const(src, i),
                            src->type->context))
        {
            array_free(dst);
            return NULL;
        }
    }

    return dst;
}

array *array_assign(array *dst, const array *src)
{
    if (dst == src) return dst;

    if (!array_resize(dst, src->size)) return NULL;

    for (size_t i = 0; i < src->size; i++)
    {
        if (dst->type->copy(array_elem(dst, i),
                            array_elem_const(src, i),
                            dst->type->context))
            return NULL;
    }

    return dst;
}

/* =========================================================
   Размеры
   ========================================================= */

size_t array_get_size(const array *arr)
{
    return arr->size;
}

size_t array_get_capacity(const array *arr)
{
    return arr->capacity;
}

/* =========================================================
   Управление вместимостью
   ========================================================= */

array *array_set_capacity(array *arr, size_t new_capacity)
{
    if (new_capacity < arr->size) return NULL;

    if (new_capacity < arr->min_capacity) new_capacity = arr->min_capacity;

    void *new_data = realloc(arr->data, new_capacity * arr->type->size);
    if (!new_data && new_capacity) return NULL;

    arr->data     = new_data;
    arr->capacity = new_capacity;

    return arr;
}

array *array_set_min_capacity(array *arr, size_t min_capacity)
{
    arr->min_capacity = min_capacity;

    if (arr->capacity < min_capacity)
        return array_set_capacity(arr, min_capacity);

    return arr;
}

array *array_set_grow_policy(array *arr, array_policy policy)
{
    arr->grow_policy = policy;
    return arr;
}

array *array_set_shrink_policy(array *arr, array_policy policy)
{
    arr->shrink_policy = policy;
    return arr;
}

/* =========================================================
   Изменение размера
   ========================================================= */

array *array_resize(array *arr, size_t new_size)
{
    const obj_type *type = arr->type;

    if (new_size > arr->capacity)
    {
        size_t new_capacity = arr->grow_policy(new_size);

        if (!array_set_capacity(arr, new_capacity)) return NULL;
    }

    if (new_size > arr->size)
    {
        for (size_t i = arr->size; i < new_size; i++)
        {
            if (type->init(array_elem(arr, i), type->context)) return NULL;
        }
    }
    else if (new_size < arr->size)
    {
        for (size_t i = new_size; i < arr->size; i++)
            type->destroy(array_elem(arr, i), type->context);

        if (arr->capacity > arr->shrink_policy(new_size))
            array_set_capacity(arr, arr->grow_policy(new_size));
    }

    arr->size = new_size;

    return arr;
}

/* =========================================================
   Доступ к элементам
   ========================================================= */

void *array_at(array *arr, size_t index)
{
    if (index >= arr->size) return NULL;

    return array_elem(arr, index);
}

const void *array_at_const(const array *arr, size_t index)
{
    if (index >= arr->size) return NULL;

    return array_elem_const(arr, index);
}

void *array_get(array *arr, size_t index)
{
    return array_elem(arr, index);
}

const void *array_get_const(const array *arr, size_t index)
{
    return array_elem_const(arr, index);
}

array *array_set(array *arr, size_t index, const void *value)
{
    if (index >= arr->size) return NULL;

    if (arr->type->copy(array_elem(arr, index), value, arr->type->context))
        return NULL;

    return arr;
}

/* =========================================================
   obj_type для array
   ========================================================= */

/**
 * @brief Возвращает размер объекта array.
 */
static size_t array_object_size(void)
{
    return sizeof(array);
}

/**
 * @brief Инициализирует объект array в выделенной памяти.
 */
static int array_init(void *dst, const void *obj_type)
{
    array *arr = dst;

    arr->type = obj_type;

    arr->size     = 0;
    arr->capacity = 0;

    arr->min_capacity = 0;

    arr->grow_policy   = array_policy_default_grow;
    arr->shrink_policy = array_policy_default_shrink;

    arr->data = NULL;

    return 0;
}

/**
 * @brief Уничтожает объект array.
 */
static void array_destroy(void *dst, const void *ctx)
{
    (void) ctx;

    array *arr = dst;

    for (size_t i = 0; i < arr->size; i++)
        arr->type->destroy(array_elem(arr, i), arr->type->context);

    free(arr->data);
}

/**
 * @brief Создает копию массива.
 */
static int array_put_copy(void *dst, const void *src, const void *obj_type)
{
    (void) obj_type;

    const array *s = src;
    array       *d = dst;

    if (array_init(d, s->type)) return -1;

    if (!array_resize(d, s->size)) return -1;

    for (size_t i = 0; i < s->size; i++)
    {
        if (s->type->copy(array_elem(d, i),
                          array_elem_const(s, i),
                          s->type->context))
            return -1;
    }

    return 0;
}

obj_type obj_mktype_array(const obj_type *el_type)
{
    obj_type t = {array_object_size(),
                  el_type,
                  array_init,
                  array_destroy,
                  array_put_copy,
                  NULL};

    return t;
}

/* =========================================================
   Дополнительные функции
   ========================================================= */

void array_set_obj_type(array *arr, const obj_type *obj_type)
{
    arr->type = obj_type;
}
