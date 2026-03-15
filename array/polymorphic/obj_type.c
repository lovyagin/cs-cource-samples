/**
 * @file obj_type.c
 * @brief Реализация абстракции типа объекта.
 */

#include "obj_type.h"

#include <stdlib.h>
#include <string.h>


/* =========================================================
   Универсальные функции работы с объектами
   ========================================================= */

void *obj_create(const obj_type *type)
{
    void *obj = malloc(type->size);
    if (!obj) return NULL;

    if (type->init(obj, type->context))
    {
        free(obj);
        return NULL;
    }

    return obj;
}


void obj_free(void *obj, const obj_type *type)
{
    type->destroy(obj, type->context);
    free(obj);
}


void *obj_clone(const void *src, const obj_type *type)
{
    void *dst = malloc(type->size);
    if (!dst) return NULL;

    if (type->copy(dst, src, type->context))
    {
        free(dst);
        return NULL;
    }
    return dst;
}


void *obj_assign(void *dst, const void *src, const obj_type *type)
{
    type->destroy(dst, type->context);

    return (type->copy(dst, src, type->context) == 0) ? dst : NULL;
}


/* =========================================================
   Реализация типа int
   ========================================================= */

/**
 * @brief Значение по умолчанию для типа int.
 *
 * Используется стандартным типом obj_int.
 */
static const int int_default = 0;


/**
 * @brief Инициализация объекта int.
 *
 * Если задан context, используется значение по умолчанию,
 * иначе объект инициализируется нулем.
 */
static int int_init(void *dst, const void *ctx)
{
    *(int *)dst = ctx ? *(const int *)ctx : 0;
    return 0;
}


/**
 * @brief Уничтожение объекта int.
 *
 * Для простых типов никаких действий не требуется.
 */
static void int_destroy(void *obj, const void *ctx)
{
    (void)obj;
    (void)ctx;
}


/**
 * @brief Копирование объекта int.
 */
static int int_copy(void *dst, const void *src, const void *ctx)
{
    (void)ctx;
    *(int *)dst = *(const int *)src;
    return 0;
}


/**
 * @brief Сравнение двух объектов int.
 */
static int int_cmp(const void *a, const void *b, const void *ctx)
{
    (void)ctx;

    return *(const int *)a - *(const int *)b;
}


const obj_type obj_int = {
    sizeof(int),
    &int_default,
    int_init,
    int_destroy,
    int_copy,
    int_cmp
};


obj_type obj_mktype_int(const int *def)
{
    obj_type t = obj_int;
    t.context = def;

    return t;
}


/* =========================================================
   Реализация типа double
   ========================================================= */

/**
 * @brief Значение по умолчанию для типа double.
 */
static const double double_default = 0.0;


/**
 * @brief Инициализация объекта double.
 */
static int double_init(void *dst, const void *ctx)
{
    *(double *)dst = ctx ? *(const double *)ctx : 0.0;
    return 0;
}


/**
 * @brief Уничтожение объекта double.
 *
 * Для простых типов никаких действий не требуется.
 */
static void double_destroy(void *obj, const void *ctx)
{
    (void)obj;
    (void)ctx;
}


/**
 * @brief Копирование объекта double.
 */
static int double_copy(void *dst, const void *src, const void *ctx)
{
    (void)ctx;
    *(double *)dst = *(const double *)src;
    return 0;
}


/**
 * @brief Сравнение двух объектов double.
 */
static int double_cmp(const void *a, const void *b, const void *ctx)
{
    (void)ctx;

    return *(const double *)a - *(const double *)b;
}


const obj_type obj_double = {
    sizeof(double),
    &double_default,
    double_init,
    double_destroy,
    double_copy,
    double_cmp
};


obj_type obj_mktype_double(const double *def)
{
    obj_type t = obj_double;
    t.context = def;

    return t;
}


/* =========================================================
   Реализация типа cstring
   ========================================================= */

/**
 * @brief Значение по умолчанию для строкового типа.
 *
 * Используется пустая строка.
 */
static const char * const cstring_default = "";


/**
 * @brief Инициализация строкового объекта.
 *
 * Создает копию строки из context.
 */
static int cstring_init(void *dst, const void *ctx)
{
    const char *def = ctx ? (const char *)ctx : "";
    char *copy = strdup(def);
    if (!copy) return -1;

    *(char **)dst = copy;
    return 0;
}


/**
 * @brief Уничтожение строкового объекта.
 *
 * Освобождает динамически выделенную строку.
 */
static void cstring_destroy(void *obj, const void *ctx)
{
    (void)ctx;

    char **str = obj;
    free(*str);
}


/**
 * @brief Копирование строкового объекта.
 *
 * Создается новая копия строки.
 */
static int cstring_copy(void *dst, const void *src, const void *ctx)
{
    (void)ctx;

    const char *s = *(char *const *)src;
    char *copy = strdup(s);
    if (!copy) return -1;

    *(char **)dst = copy;
    return 0;
}


/**
 * @brief Лексикографическое сравнение строк.
 */
static int cstring_cmp(const void *a, const void *b, const void *ctx)
{
    (void)ctx;

    return strcmp(*(char *const *)a, *(char *const *)b);
}


const obj_type obj_cstring = {
    sizeof(char *),
    cstring_default,
    cstring_init,
    cstring_destroy,
    cstring_copy,
    cstring_cmp
};


obj_type obj_mktype_cstring(const char *def)
{
    obj_type t = obj_cstring;
    t.context = def ? def : "";
    return t;
}


/* =========================================================
   Типобезопасные функции
   ========================================================= */

/* ---------- int ---------- */

int *obj_int_create(int value)
{
    int *x = obj_create(&obj_int);
    if (x) *x = value;
    return x;
}

void obj_int_free(int *obj)
{
    obj_free(obj, &obj_int);
}

int *obj_int_clone(const int *src)
{
    return obj_clone(src, &obj_int);
}

int *obj_int_assign(int *dst, const int *src)
{
    return obj_assign(dst, src, &obj_int);
}

int *obj_int_get(void *src)
{
    return (int *)src;
}

const int *obj_int_get_const(const void *src)
{
    return (const int *)src;
}


/* ---------- double ---------- */

double *obj_double_create(double value)
{
    double *x = obj_create(&obj_double);
    if (x) *x = value;
    return x;
}

void obj_double_free(double *obj)
{
    obj_free(obj, &obj_double);
}

double *obj_double_clone(const double *src)
{
    return obj_clone(src, &obj_double);
}

double *obj_double_assign(double *dst, const double *src)
{
    return obj_assign(dst, src, &obj_double);
}

double *obj_double_get(void *src)
{
    return (double *)src;
}

const double *obj_double_get_const(const void *src)
{
    return (const double *)src;
}


/* ---------- cstring ---------- */

char **obj_cstring_create(const char *str)
{
    return obj_create(&(obj_type){
        obj_cstring.size,
        str,
        obj_cstring.init,
        obj_cstring.destroy,
        obj_cstring.copy,
        obj_cstring.cmp
    });
}
void obj_cstring_free(char **obj)
{
    obj_free(obj, &obj_cstring);
}

char **obj_cstring_clone(char *const *src)
{
    return obj_clone(src, &obj_cstring);
}

char **obj_cstring_assign(char **dst, char *const *src)
{
    return obj_assign(dst, src, &obj_cstring);
}

char **obj_cstring_get(void *src)
{
    return (char **)src;
}

const char **obj_cstring_get_const(const void *src)
{
    return (const char **)src;
}
