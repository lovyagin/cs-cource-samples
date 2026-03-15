/**
 * @file array.h
 * @brief Универсальный полиморфный динамический массив
 *
 * Данный модуль реализует полиморфный динамический массив,
 * работающий с элементами произвольного типа, и поддерживающий
 * задание пользовательской политики управления вместимостью.
 *
 * Потокобезопасность:
 * - Массив не является потокобезопасным.
 *
 * Обработка ошибок:
 * - Функции, возвращающие указатель, возвращают NULL при ошибке.
 * - В случае ошибки массив сохраняет корректное состояние.
 */

#ifndef _ARRAY_H
#define _ARRAY_H

#include "obj_type.h"

/**
 * @brief Неполный тип динамического массива (инкапсуляция).
 */
typedef struct array array;

/**
 * @brief Тип функции политики изменения вместимости.
 *
 * @param size Требуемый или текущий размер.
 * @return Новая или пороговая вместимость.
 */
typedef size_t (*array_policy)(size_t size);

/**
 * @brief Создает новый пустой массив.
 *
 * @param[in] el_type Тип элементов массива.
 * @return Указатель на созданный массив или NULL при ошибке.
 * @warning Созданный массив должен быть освобожден с помощью array_free
 */
array *array_create(const obj_type *el_type);

/**
 * @brief Освобождает массив и уничтожает все элементы.
 *
 * @param[in,out] arr Массив.
 */
void array_free(array *arr);

/**
 * @brief Создает полную копию массива.
 *
 * @param[in] src Исходный массив.
 * @return Новый массив или NULL при ошибке.
 * @warning Созданный массив должен быть освобожден с помощью array_free
 */
array *array_create_copy(const array *src);

/**
 * @brief Присваивает массив src массиву dst.
 *
 * Эквивалент операции присваивания (=).
 *
 * @param[in,out] dst Целевой массив.
 * @param[in] src Исходный массив.
 * @return dst при успехе или NULL при ошибке.
 */
array *array_assign(array *dst, const array *src);

/**
 * @brief Возвращает текущий размер массива.
 *
 * @param[in] arr Массив.
 * @return Количество элементов.
 */
size_t array_get_size(const array *arr);

/**
 * @brief Возвращает текущую вместимость массива.
 *
 * @param[in] arr Массив.
 * @return вместимость массива.
 */
size_t array_get_capacity(const array *arr);

/**
 * @brief Изменяет размер массива.
 *
 * При увеличении размера новые элементы инициализируются.
 * При уменьшении лишние элементы уничтожаются.
 *
 * @param[in,out] arr Массив.
 * @param[in] new_size Новый размер.
 * @return arr при успехе или NULL при ошибке.
 */
array *array_resize(array *arr, size_t new_size);

/**
 * @brief Устанавливает вместимость массива.
 *
 * Может привести к перераспределению памяти.
 *
 * @param[in,out] arr Массив.
 * @param[in] new_capacity Новая вместимость.
 * @return arr при успехе или NULL при ошибке.
 */
array *array_set_capacity(array *arr, size_t new_capacity);

/**
 * @brief Обеспечивает минимальную вместимость массива.
 *
 * Вместимость никогда не будет установлена ниже данной независимо от функций политики
 *
 * @param[in,out] arr Массив.
 * @param[in] min_capacity Минимальная вместимость.
 * @return arr при успехе или NULL при ошибке.
 */
array *array_set_min_capacity(array *arr, size_t min_capacity);

/**
 * @brief Устанавливает политику увеличения емкости.
 *
 * Политика роста должна вернуть значение вместимости,
 * которое требуется установить при заданном новом размере массива.
 *
 * @param[in,out] arr Массив.
 * @param[in] policy Функция политики роста.
 * @return arr.
 */
array *array_set_grow_policy(array *arr, array_policy policy);

/**
 * @brief Устанавливает политику уменьшения емкости.
 *
 * Политика уменьшения должна вернуть значение вместимости,
 * при превышении которого требуется уменьшить вместимость.
 *
 * Порог проверяется при уменьшении размера массива.
 *
 * @param[in,out] arr Массив.
 * @param[in] policy Функция политики сжатия.
 * @return arr.
 */
array *array_set_shrink_policy(array *arr, array_policy policy);

/**
 * @brief Возвращает указатель на элемент с проверкой границ.
 *
 * @param[in,out] arr Массив.
 * @param[in] index Индекс.
 * @return Указатель на элемент или NULL при выходе за границы.
 */
void *array_at(array *arr, size_t index);

/**
 * @brief Константная версия array_at().
 */
const void *array_at_const(const array *arr, size_t index);

/**
 * @brief Возвращает указатель на элемент без проверки границ.
 *
 * Поведение не определено при некорректном индексе.
 */
void *array_get(array *arr, size_t index);

/**
 * @brief Константная версия array_get().
 */
const void *array_get_const(const array *arr, size_t index);

/**
 * @brief Присваивает значение элементу массива.
 *
 * @param[in,out] arr Массив.
 * @param[in] index Индекс.
 * @param[in] value Указатель на значение.
 * @return arr при успехе или NULL при ошибке.
 */
array *array_set(array *arr, size_t index, const void *value);

/**
 * @brief Политика роста по умолчанию.
 *
 * Вместимость устанавливается как:
 *
 * capacity = 2 * required_size
 */
size_t array_policy_default_grow(size_t required_size);

/**
 * @brief Политика уменьшения вместимости по умолчанию.
 *
 * Уменьшение происходит если:
 *
 * capacity > 4 * current_size
 */
size_t array_policy_default_shrink(size_t current_size);

/**
 * @brief Создает описание типа для объекта array.
 *
 * Позволяет использовать массив как элемент другого массива.
 *
 * @param[in] el_type Тип элементов вложенного массива.
 * @return Описание типа array.
 */
obj_type obj_mktype_array(const obj_type *el_type);

#endif