#include <stdio.h>
#include <stdlib.h>

typedef int key_t;

typedef struct pheap_node 
{
    key_t key;
    struct pheap_node *child;
    struct pheap_node *sibling;
} pheap_node;

typedef struct 
{
    pheap_node *root;
} pheap;

/* добавить n в список детей h слева */
static pheap_node *pheap_add_child(pheap_node *h, pheap_node *n)
{
    n->sibling = h->child;
    h->child = n;
    return h;
}

/* слияние двух куч */
static pheap_node *pheap_merge(pheap_node *a, pheap_node *b)
{
    if (!a) return b;
    if (!b) return a;

    return a->key <= b->key ? pheap_add_child(a, b)
                            : pheap_add_child(b, a);
}

/* "Превращение в кучу" после удаления корня */
static pheap_node *pheap_merge_pairs(pheap_node *n)
{
    if (!n || !n->sibling)
        return n;

    pheap_node *a = n;
    pheap_node *b = n->sibling;
    pheap_node *c = b->sibling;

    a->sibling = b->sibling = NULL;

    return pheap_merge( pheap_merge(a, b), pheap_merge_pairs(c) );
}

/* init */
void pheap_init(pheap *h)
{
    h->root = NULL;
}

/* insert */
int pheap_insert(pheap *h, key_t k)
{
    pheap_node *n = malloc(sizeof(*n));
    if (!n) return -1;

    n->key = k;
    n->child = n->sibling = NULL;

    h->root = pheap_merge(h->root, n);
    return 0;
}

/* min */
key_t *pheap_min(pheap *h)
{
    return h->root ? &h->root->key : NULL;
}

/* pop */
int pheap_pop(pheap *h)
{
    if (!h->root) return -1;

    pheap_node *old = h->root;
    h->root = pheap_merge_pairs(old->child);

    free(old);
    return 0;
}

/* destroy */
static void pheap_destroy_rec(pheap_node *n)
{
    if (!n) return;
    pheap_destroy_rec(n->child);
    pheap_destroy_rec(n->sibling);
    free(n);
}

void pheap_destroy(pheap *h)
{
    pheap_destroy_rec(h->root);
}

/* demo */
int main(void)
{
    pheap h;
    pheap_init(&h);

    int a[] = {5, 3, 8, 1, 6};

    for (int i = 0; i < 5; i++)
        pheap_insert(&h, a[i]);

    while (pheap_min(&h)) {
        printf("%d ", *pheap_min(&h));
        pheap_pop(&h);
    }

    printf("\n");
    pheap_destroy(&h);
    return 0;
}
