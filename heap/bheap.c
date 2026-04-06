#include <stdio.h>
#include <stdlib.h>

typedef int key_t;

typedef struct bheap
{
    size_t size, cap;
    key_t *data;
} bheap;

/* Индексы */
static inline size_t parent(size_t i) { return (i - 1) / 2; }
static inline size_t left  (size_t i) { return 2 * i + 1;   }
static inline size_t right (size_t i) { return 2 * i + 2;   }


/* Инициализация */
bheap *bheap_init(size_t cap)
{
    bheap *h = malloc (sizeof (*h));
    if (!h) return NULL;

    h->data = malloc (sizeof(key_t) * cap);
    if (!h->data) {free(h); return NULL; };

    h->size = 0;
    h->cap = cap;
    return h;
}

void bheap_free(bheap *h)
{
    if (h) free(h->data);
    free (h);
}

/* Минимум */
key_t *bheap_min(bheap *h)
{
    return h->size ? h->data : NULL;
}

/* Вставка (heapify-up) */
int bheap_insert(bheap *h, key_t k)
{
    if (h->size == h->cap) return -1;

    size_t i = h->size++;

    /* Поднимаем k вверх */
    while (i > 0)
    {
        size_t p = parent(i);

        /* если родитель меньше - найдено место для вставки */
        if (h->data[p] <= k)
            break;

        /* опускаем родителя вниз */
        h->data[i] = h->data[p];
        i = p;
    }

    /* вставляем элемент */
    h->data[i] = k;
    return 0;
}

/* Удаление минимума (heapify-down) */
int bheap_pop(bheap *h)
{
    if (h->size == 0)
        return -1;

    key_t x = h->data[--h->size];  // последний элемент

    size_t i = 0;

    while (1)
    {
        size_t l = left(i);
        size_t r = right(i);

        if (l >= h->size)
            break; // нет детей

        /* выбираем меньшего ребенка, правый проверяется только если он есть */
        size_t s = (r < h->size && h->data[r] < h->data[l]) ? r : l;

        /* если x уже меньше меньшего ребенка, то найдено место вставки */
        if (x <= h->data[s])
            break;

        /* поднимаем ребенка */
        h->data[i] = h->data[s];
        i = s;
    }

    h->data[i] = x;
    return 0;
}

/* Heap sort */
int bheap_sort(key_t *a, size_t n)
{
    bheap *h = bheap_init(n);
    if (!h) return -1;

    /* Построение кучи */
    for (size_t i = 0; i < n; ++i)
        if (bheap_insert(h, a[i]))
        {
            bheap_free(h);
            return -1;
        }

    /* Извлечение массива */
    for (size_t i = 0; i < n; ++i)
    {
        key_t *m = bheap_min(h);
        if (!m)
        {
            bheap_free(h);
            return -1;
        }

        a[i] = *m;
        if (bheap_pop(h))
        {
            bheap_free(h);
            return -1;
        }
    }

    bheap_free(h); /* Нужно освобождать даже пустую кучу */

    return 0;
}

/* Демо */
int main(void)
{
    key_t a[] = {5, 3, 8, 1, 6};
    size_t n = sizeof(a)/sizeof(a[0]);

    bheap_sort(a, n);

    for (size_t i = 0; i < n; ++i)
        printf("%d ", a[i]);

    printf("\n");
    return 0;
}
