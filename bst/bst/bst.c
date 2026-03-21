#include <stdio.h>
#include <stdlib.h>

typedef int    key_t;
typedef double element_t;

typedef struct bst
{
    key_t       key;
    element_t   data;
    struct bst *left, *right;
} bst;

bst *bst_insert(bst *root, key_t k, element_t e)
{
    if (root == NULL) /* Если дерево пусто */
    {
        bst *node = malloc(sizeof(bst));
        if (!node) return NULL;
        node->left = node->right = NULL;
        node->key                = k;
        node->data               = e;
        return node;
    }
    /* запрет повторяющихся ключей                    */
    if (root->key == k) return NULL;
    if (root->key < k)
    { /* вставляем в правое поддерево              */
        bst *node = bst_insert(root->right, k, e);
        if (!node) return NULL; /* вставка не удалась */
        root->right = node;
    }
    else
    { /* вставляем в левое поддерево               */
        bst *node = bst_insert(root->left, k, e);
        if (!node) return NULL;
        root->left = node;
    }
    return root;
}

bst *bst_insert_iter(bst *root, key_t k, element_t e)
{
    bst *cur = root, *par;
    
    /* сразу создается новый элемент */
    bst *node = malloc(sizeof(bst));
    if (!node) return NULL;
    node->left = node->right = NULL;
    node->key                = k;
    node->data               = e;
    
    /* если дерево было пустым, возвращаем новый узел           */
    if (root == NULL) return node;
    
    /* cur пробегает элементы от корня до листа и NULL          */
    /* par указывает на родительский по отношению к cur элемент */
    do
    {
        if (cur->key == k)
        {
            free(node);
            return NULL;
        }
        par = cur;
        cur = k > cur->key ? cur->right : cur->left;
    } while (cur);

    /* включаем узел в дерево в качестве левого или правого потомка */
    if (k > par->key)
        par->right = node;
    else
        par->left = node;
    return root;
}

bst *bst_search(bst *root, key_t k)
{
    bst *cur = root;
    while (cur)
    {
        if (k < cur->key)
            cur = cur->left;
        else if (k > cur->key)
            cur = cur->right;
        else
            return cur;
    }
    return NULL;
}

bst *bst_max(bst *root)
{
    bst *cur = root;
    if (!root) return NULL;
    while (cur->right) cur = cur->right;
    return cur;
}
bst *bst_min(bst *root)
{
    bst *cur = root;
    if (!root) return NULL;
    while (cur->left) cur = cur->left;
    return cur;
}

bst *bst_successor(bst *root, bst *node)
{
    bst *succ = NULL;
    
    /* если правое поддерево есть, то это наименьший элемент в нем */
    if (node->right) return bst_min(node->right);
    
    /* иначе это ближайший левый предок */
    while (root)
    {
        if (node->key < root->key)
        {
            succ = root;
            root = root->left;
        }
        else if (node == root)
            return succ;
        else
            root = root->right;
    }
    return succ;
}

bst *bst_predecessor(bst *root, bst *node)
{
    bst *succ = NULL;
    if (node->left) return bst_max(node->left);
    while (root)
    {
        if (node->key > root->key)
        {
            succ = root;
            root = root->right;
        }
        else if (node == root)
            return succ;
        else
            root = root->left;
    }
    return succ;
}

int bst_remove(bst **root, bst *node)
{
    if (!*root)
        return 1; /* дерево пусто, в тч.. элемент не найден при рекурсии   */
    if (node
        == *root) /* нашли удаляемый элемент                               */
    {
        if (node->left
            && node->right) /* есть оба поддерева                      */
        {
            /* находим и удаляем наименьший элемент    */
            /* в правом поддереве                      */

            bst *min   = bst_min(node->right);
            node->key  = min->key;
            node->data = min->data;
            return bst_remove(&(*root)->right, min);
        }
        /* важно использование указателя на        */
        /* (*root)->right, т.к. именно он может    */
        /* быть изменен при удалении               */
        else
        {                   /* есть только одно поддерево              */
            bst *t = *root; /* запоминаем удаляемый элемент            */
            /* заменяем корень единственным поддеревом */
            *root = (*root)->left ? (*root)->left : (*root)->right;
            /* освобождаем память                      */
            free(t);
            return 0;
        }
    }
    else if (node->key >= (*root)->key) /* удаляемый элемент в правом поддереве    */
        return bst_remove(&(*root)->right, node);
    else /* удаляемый элемент в левом поддереве     */
        return bst_remove(&(*root)->left, node);
}

void bst_free(bst *tree)
{
    if (tree)
    {
        bst_free(tree->left);
        bst_free(tree->right);
        free(tree);
    }
}

/* вывод поддерева рекурсивно */
void bst_print_req(const bst *tree)
{
    if (!tree) return;
    bst_print_req(tree->left);
    if (tree->left) printf(", ");
    printf("(%i => %lg)", tree->key, tree->data);
    if (tree->right) printf(", ");
    bst_print_req(tree->right);
}
void bst_print(const bst *tree)
{
    printf("BST: ");
    bst_print_req(tree);
    printf("\n");
}

int main()
{
    bst      *tree = NULL;
    int       i;
    key_t     k;
    element_t e;
    while (1)
    {
        printf("BST demo:\n"
               "1 - Insert (reqursive)\n"
               "2 - Insert (iterative)\n"
               "3 - Remove\n"
               "4 - Minimum\n"
               "5 - Maximim\n"
               "6 - Successor\n"
               "7 - Predecessor\n"
               "8 - Print\n"
               "0 - Exit\n");
        scanf("%i", &i);
        switch (i)
        {
            bst *t;
        case 1:
            printf("Enter new key and element: ");
            if (scanf("%i%lf", &k, &e) != 2)
            {
                printf("Bad format\n");
                break;
            };
            t = bst_insert(tree, k, e);
            if (!t)
                fprintf(stderr,
                        "Duplicate key or bad alloc - out of memory?\n");
            else
                tree = t;
            bst_print(tree);
            break;
        case 2:
            printf("Enter new key and element: ");
            if (scanf("%i%lf", &k, &e) != 2)
            {
                printf("Bad format\n");
                break;
            };
            t = bst_insert_iter(tree, k, e);
            if (!t)
                fprintf(stderr,
                        "Duplicate key or bad alloc - out of memory?\n");
            else
                tree = t;
            bst_print(tree);
            break;
        case 3:
            printf("Enter key to delete: ");
            if (scanf("%i", &k) != 1)
            {
                printf("Bad format\n");
                break;
            };
            t = bst_search(tree, k);
            if (!t)
            {
                printf("key not found\n");
                break;
            }
            if (bst_remove(&tree, t))
            {
                printf("element not found in the tree\n");
                break;
            }
            bst_print(tree);
            break;
        case 4:
            t = bst_min(tree);
            if (!t)
            {
                printf("Empty tree\n");
                break;
            }
            printf("Minimum: (%i, %lf)\n", t->key, t->data);
            break;
        case 5:
            t = bst_max(tree);
            if (!t)
            {
                printf("Empty tree\n");
                break;
            }
            printf("Maximum: (%i, %lf)\n", t->key, t->data);
            break;
        case 6:
            printf("Enter key: ");
            if (scanf("%i", &k) != 1)
            {
                printf("Bad format\n");
                break;
            };
            t = bst_search(tree, k);
            if (!t)
            {
                printf("key not found\n");
                break;
            }
            t = bst_successor(tree, t);
            if (!t)
            {
                printf("No successor found\n");
                break;
            }
            printf("Successor: (%i, %lf)\n", t->key, t->data);
            break;
        case 7:
            printf("Enter key and element: ");
            if (scanf("%i", &k) != 1)
            {
                printf("Bad format\n");
                break;
            };
            t = bst_search(tree, k);
            if (!t)
            {
                printf("key not found\n");
                break;
            }
            t = bst_predecessor(tree, t);
            if (!t)
            {
                printf("No predecessor found\n");
                break;
            }
            printf("Predecessor: (%i, %lf)\n", t->key, t->data);
            break;
        case 8:
            bst_print(tree);
            break;
        case 0:
            bst_free(tree);
            exit(0);
        default:
            printf("Bad command\n");
        }
    }
}
