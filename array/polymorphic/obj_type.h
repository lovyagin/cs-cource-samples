/**
 * @file obj_type.h
 * @brief Абстракция типа элемента для универсальных контейнеров.
 *
 * Модуль определяет интерфейс жизненного цикла объектов,
 * используемый универсальными полиморфными контейнерами
 * (массивами, списками, деревьями и др.).
 *
 * Тип элемента описывается структурой obj_type,
 * содержащей функции инициализации, уничтожения,
 * копирования и сравнения объектов.
 *
 * Потокобезопасность:
 * - Модуль не является потокобезопасным.
 */

#ifndef _OBJ_TYPE_H
#define _OBJ_TYPE_H

#include <stddef.h>

/**
 * @brief Описание жизненного цикла объекта.
 *
 * Контейнеры используют данный интерфейс
 * для управления памятью элементов.
 */
typedef struct obj_type
{

    /** Размер объекта в байтах */
    size_t size;

    /**
     * Пользовательский контекст типа.
     *
     * Может использоваться для хранения
     * параметров типа (например, значения по умолчанию).
     */
    const void *context;

    /**
     * @brief Инициализация объекта.
     *
     * @param[out] dst Указатель на неинициализированную память.
     * @param[in] context Контекст типа.
     *
     * @return 0 при успехе, -1 при ошибке.
     */
    int (*init)(void *dst, const void *context);

    /**
     * @brief Уничтожение объекта.
     *
     * @param[in,out] obj Ранее инициализированный объект.
     * @param[in] context Контекст типа.
     */
    void (*destroy)(void *obj, const void *context);

    /**
     * @brief Копирование объекта.
     *
     * dst должен указывать на неинициализированную память.
     *
     * @param[out] dst Целевой объект.
     * @param[in] src Исходный объект.
     * @param[in] context Контекст типа.
     *
     * @return 0 при успехе, -1 при ошибке.
     */
    int (*copy)(void *dst, const void *src, const void *context);

    /**
     * @brief Сравнение объектов.
     *
     * @param[in] a Первый объект.
     * @param[in] b Второй объект.
     * @param[in] context Контекст типа.
     *
     * @return
     *  - < 0 если a < b
     *  - = 0 если a == b
     *  - > 0 если a > b
     *
     * Семантика аналогична strcmp().
     */
    int (*cmp)(const void *a, const void *b, const void *context);

} obj_type;

/* =========================================================
   Универсальные функции работы с объектами
   ========================================================= */

/**
 * @brief Создает новый объект на куче.
 *
 * Объект инициализируется с помощью type->init().
 *
 * @param[in] type Описание типа объекта.
 *
 * @return Указатель на созданный объект
 *         или NULL при ошибке.
 */
void *obj_create(const obj_type *type);

/**
 * @brief Уничтожает объект.
 *
 * Сначала вызывается type->destroy(),
 * затем освобождается память объекта.
 *
 * @param[in,out] obj Объект.
 * @param[in] type Описание типа объекта.
 */
void obj_free(void *obj, const obj_type *type);

/**
 * @brief Создает копию объекта.
 *
 * @param[in] src Исходный объект.
 * @param[in] type Описание типа объекта.
 *
 * @return Новый объект или NULL при ошибке.
 */
void *obj_clone(const void *src, const obj_type *type);

/**
 * @brief Присваивает значение одного объекта другому.
 *
 * @param[in,out] dst Целевой объект (должен быть инициализирован).
 * @param[in] src Исходный объект.
 * @param[in] type Описание типа объекта.
 *
 * @return dst при успехе или NULL при ошибке.
 */
void *obj_assign(void *dst, const void *src, const obj_type *type);

/* =========================================================
   Стандартные типы
   ========================================================= */

/**
 * @brief Тип int, значение по умолчанию — 0.
 */
extern const obj_type obj_int;

/**
 * @brief Тип double, значение по умолчанию — 0.0.
 */
extern const obj_type obj_double;

/**
 * @brief Тип C-строки.
 *
 * Элемент имеет тип `char *`.
 *
 * Значение хранится как указатель на строку,
 * которая динамически выделяется (например через strdup).
 *
 * Значение по умолчанию — пустая строка "".
 */
extern const obj_type obj_cstring;

/**
 * @brief Создает тип int с заданным значением по умолчанию.
 *
 * @param[in] def Значение по умолчанию.
 *
 * @return Описание типа int.
 *
 * @warning **Важно:** в obj_type копируется только указатель, поэтому объект def не должен быть временным.
 */
obj_type obj_mktype_int(const int *def);

/**
 * @brief Создает тип double с заданным значением по умолчанию.
 *
 * @param[in] def Значение по умолчанию.
 *
 * @return Описание типа double.
 *
 * @warning **Важно:** в obj_type копируется только указатель, поэтому объект def не должен быть временным.
 */
obj_type obj_mktype_double(const double *def);

/**
 * @brief Создает тип C-строки с заданным значением по умолчанию.
 *
 * @param[in] def Строка по умолчанию.
 *
 * @return Описание строкового типа.
 *
 * @warning **Важно:** в obj_type копируется только указатель, поэтому объект def не должен быть временным.
 */
obj_type obj_mktype_cstring(const char *def);

/* =========================================================
   Типобезопасные функции для стандартных типов
   ========================================================= */

/* ---------- int ---------- */

/**
 * @brief Создает объект типа int на куче.
 *
 * @param[in] value Начальное значение.
 *
 * @return Указатель на созданный объект
 *         или NULL при ошибке.
 */
int *obj_int_create(int value);

/**
 * @brief Освобождает объект типа int.
 *
 * @param[in,out] obj Объект.
 */
void obj_int_free(int *obj);

/**
 * @brief Создает копию объекта int.
 *
 * @param[in] src Исходный объект.
 *
 * @return Новый объект или NULL при ошибке.
 */
int *obj_int_clone(const int *src);

/**
 * @brief Присваивает значение src объекту dst.
 *
 * @param[in,out] dst Целевой объект.
 * @param[in] src Исходный объект.
 *
 * @return dst при успехе или NULL при ошибке.
 */
int *obj_int_assign(int *dst, const int *src);

/**
 * @brief Получает указатель на объект int
 *        из универсального указателя.
 *
 * Используется для безопасного приведения
 * типов при работе с универсальными контейнерами.
 *
 * @param[in] src Указатель на объект.
 *
 * @return Указатель на int.
 */
int *obj_int_get(void *src);

/**
 * @brief Константная версия obj_int_get().
 *
 * @param[in] src Указатель на объект.
 *
 * @return Указатель на const int.
 */
const int *obj_int_get_const(const void *src);

/* ---------- double ---------- */

/**
 * @brief Создает объект типа double на куче.
 *
 * @param[in] value Начальное значение.
 *
 * @return Новый объект или NULL при ошибке.
 */
double *obj_double_create(double value);

/**
 * @brief Освобождает объект типа double.
 *
 * @param[in,out] obj Объект.
 */
void obj_double_free(double *obj);

/**
 * @brief Создает копию объекта double.
 *
 * @param[in] src Исходный объект.
 *
 * @return Новый объект или NULL при ошибке.
 */
double *obj_double_clone(const double *src);

/**
 * @brief Присваивает значение src объекту dst.
 *
 * @param[in,out] dst Целевой объект.
 * @param[in] src Исходный объект.
 *
 * @return dst при успехе или NULL при ошибке.
 */
double *obj_double_assign(double *dst, const double *src);

/**
 * @brief Получает указатель на объект double
 *        из универсального указателя.
 *
 * Используется для безопасного приведения
 * типов при работе с универсальными контейнерами.
 *
 * @param[in] src Указатель на объект.
 *
 * @return Указатель на double.
 */
double *obj_double_get(void *src);

/**
 * @brief Константная версия obj_double_get().
 *
 * @param[in] src Указатель на объект.
 *
 * @return Указатель на const double.
 */
const double *obj_double_get_const(const void *src);

/* ---------- cstring ---------- */

/**
 * @brief Создает строковый объект.
 *
 * Строка копируется (обычно через strdup).
 *
 * @param[in] str Исходная строка.
 *
 * @return Новый строковый объект или NULL при ошибке.
 */
char **obj_cstring_create(const char *str);

/**
 * @brief Освобождает строковый объект.
 *
 * @param[in,out] obj Объект.
 */
void obj_cstring_free(char **obj);

/**
 * @brief Создает копию строкового объекта.
 *
 * @param[in] src Исходный объект.
 *
 * @return Новый объект или NULL при ошибке.
 */
char **obj_cstring_clone(char *const *src);

/**
 * @brief Присваивает строку.
 *
 * @param[in,out] dst Целевой объект.
 * @param[in] src Исходный объект.
 *
 * @return dst при успехе или NULL при ошибке.
 */
char **obj_cstring_assign(char **dst, char *const *src);

/**
 * @brief Получает указатель на объект cstring
 *        из универсального указателя.
 *
 * Используется для безопасного приведения
 * типов при работе с универсальными контейнерами.
 *
 * @param[in] src Указатель на объект.
 *
 * @return Указатель на cstring (cstring понимается как char *).
 */
char **obj_cstring_get(void *src);

/**
 * @brief Константная версия obj_double_get().
 *
 * @param[in] src Указатель на объект.
 *
 * @return Указатель на const double.
 */
const char **obj_cstring_get_const(const void *src);

#endif // _OBJ_TYPE_H
