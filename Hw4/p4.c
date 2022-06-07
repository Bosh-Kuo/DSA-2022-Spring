// reference: https://cp-algorithms.com/data_structures/treap.html#implicit-treaps
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int prior, size;
    long long value, sum, update;
    bool rev;
    struct Node *left, *right;
} Node;

int size(Node *Treap)
{
    return Treap ? Treap->size : 0;
}

long long sum(Node *Treap)
{
    return Treap ? Treap->sum : 0;
}

void pull(Node *Treap)
{
    if (Treap)
    {
        Treap->size = 1 + size(Treap->left) + size(Treap->right);
        Treap->sum = Treap->value + sum(Treap->left) + sum(Treap->right);
    }
}
void push(Node *Treap)
{
    if (Treap && Treap->rev)
    {
        Treap->rev = false;
        // swap left and right
        Node *temp = Treap->left;
        Treap->left = Treap->right;
        Treap->right = temp;

        if (Treap->left)
            Treap->left->rev ^= true;
        if (Treap->right)
            Treap->right->rev ^= true;
    }
    if (Treap && Treap->update != 0)
    {
        long long update = Treap->update;
        if (Treap->value > update)
            Treap->value = update;
        Treap->update = 0;
        if (Treap->left)
            Treap->left->update = update;
        if (Treap->right)
            Treap->right->update = update;
    }
}

void split(Node *Treap, Node **Left_ptr, Node **Right_ptr, int val)
{
    push(Treap);
    if (!Treap)
    {
        *Left_ptr = *Right_ptr = NULL;
        return;
    }
    else if (size(Treap->left) < val)
    {
        split(Treap->right, &(Treap->right), Right_ptr, val - size(Treap->left) - 1);
        *Left_ptr = Treap;
    }
    else
    {
        split(Treap->left, Left_ptr, &(Treap->left), val);
        *Right_ptr = Treap;
    }
    pull(Treap);
}

void merge(Node **Treap_ptr, Node *Left, Node *Right)
{
    push(Left);
    push(Right);
    if (!Left || !Right)
        *Treap_ptr = Left ? Left : Right;
    else if (Left->prior > Right->prior)
    {
        *Treap_ptr = Left;
        merge(&(Left->right), Left->right, Right);
    }
    else
    {
        *Treap_ptr = Right;
        merge(&(Right->left), Left, Right->left);
    }
    pull(*Treap_ptr);
}

Node *createNode(int value, int prior)
{
    Node *Treap = (Node *)malloc(sizeof(Node));
    Treap->value = value;
    Treap->sum = value;
    Treap->prior = prior;
    Treap->left = NULL;
    Treap->right = NULL;
    Treap->size = 1;
    Treap->rev = false;
    Treap->update = 0;
    return Treap;
}

Node *build(long long *array, int length)
{
    Node *Treap = NULL;
    for (int i = 0; i < length; i++)
    {
        Node *newNode = createNode(array[i], rand());
        merge(&Treap, Treap, newNode);
    }
    return Treap;
}

void InOrder_traversal(Node *Treap)
{
    if (!Treap)
        return;
    if (Treap->left)
        InOrder_traversal(Treap->left);
    printf("value: %lld,  sum: %lld,  size: %d, prior: %d\n", Treap->value, Treap->sum, Treap->size, Treap->prior);
    if (Treap->right)
        InOrder_traversal(Treap->right);
}

void rangeSum(Node **Treap, int l, int r)
{
    Node *Left = NULL;
    Node *Mid = NULL;
    Node *Right = NULL;
    split(*Treap, &Mid, &Right, r);
    split(Mid, &Left, &Mid, l - 1);
    printf("%lld\n", Mid->sum);
    merge(Treap, Left, Mid);
    merge(Treap, *Treap, Right);
}

void insert(Node **Treap, int p, long long k)
{
    Node *newNode = createNode(k, rand());
    Node *Left = NULL;
    Node *Right = NULL;
    split(*Treap, &Left, &Right, p);
    merge(Treap, Left, newNode);
    merge(Treap, *Treap, Right);
}

void delete (Node **Treap, int p)
{
    Node *Left = NULL;
    Node *Right = NULL;
    Node *Mid = NULL;
    split(*Treap, &Left, &Right, p);
    split(Left, &Left, &Mid, p - 1);
    merge(Treap, Left, Right);
}

void reverse(Node **Treap, int l, int r)
{
    Node *Left = NULL;
    Node *Right = NULL;
    Node *Mid = NULL;
    split(*Treap, &Left, &Right, r);
    split(Left, &Left, &Mid, l - 1);
    Mid->rev ^= true;
    merge(Treap, Left, Mid);
    merge(Treap, *Treap, Right);
}

void blockSwap(Node **Treap, int l, int r, int x, int y)
{
    if (l > x)
    {
        blockSwap(Treap, x, y, l, r);
        return;
    }
    Node *A1 = NULL;
    Node *A2 = NULL;
    Node *A3 = NULL;
    Node *A4 = NULL;
    Node *A5 = NULL;
    split(*Treap, &A4, &A5, y);
    split(A4, &A3, &A4, x - 1);
    split(A3, &A2, &A3, r);
    split(A2, &A1, &A2, l - 1);
    merge(Treap, A1, A4);
    merge(Treap, *Treap, A3);
    merge(Treap, *Treap, A2);
    merge(Treap, *Treap, A5);
}

// void update(Node *Treap, long long k)
// {
//     if (!Treap)
//         return;
//     if (Treap->left)
//         update(Treap->left, k);
//     if (Treap->value > k)
//         Treap->value = k;
//     if (Treap->right)
//         update(Treap->right, k); 
// }

void blockUpdate(Node **Treap, int l, int r, long long k)
{
    Node *Left = NULL;
    Node *Right = NULL;
    Node *Mid = NULL;
    split(*Treap, &Left, &Right, r);
    split(Left, &Left, &Mid, l - 1);
    Mid->update = k;
    merge(Treap, Left, Mid);
    merge(Treap, *Treap, Right);
}

int main()
{
    // input
    int N, Q;
    scanf("%d %d", &N, &Q);
    long long *array = (long long *)malloc(N * sizeof(long long));
    for (int i = 0; i < N; i++)
        scanf("%lld", array + i);
    Node *Treap = build(array, N);

    // operations
    for (int i = 0; i < Q; i++)
    {
        int task;
        scanf("%d", &task);

        if (task == 1)
        {
            int p;
            long long k;
            scanf("%d %lld", &p, &k);
            insert(&Treap, p, k);
        }
        else if (task == 2)
        {
            int p;
            scanf("%d", &p);
            delete (&Treap, p);
        }
        else if (task == 3)
        {
            int l, r;
            scanf("%d %d", &l, &r);
            reverse(&Treap, l, r);
        }
        else if (task == 4)
        {
            int l, r, x, y;
            scanf("%d %d %d %d", &l, &r, &x, &y);
            blockSwap(&Treap, l, r, x, y);
        }
        else if (task == 5)
        {
            int l, r;
            long long k;
            scanf("%d %d %lld", &l, &r, &k);
            blockUpdate(&Treap, l, r, k);
        }
        else if (task == 6)
        {
            int l, r;
            scanf("%d %d", &l, &r);
            rangeSum(&Treap, l, r);
        }
        // InOrder_traversal(Treap);
        // printf("\n\n");
    }
}
