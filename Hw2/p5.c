// ref: textbook p.154
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "price.h"
#define MAX_A 1024
#define MAX_k 1000001

typedef struct Info
{
    unsigned long long price;
    int s_idx;
} Info;

void min_heapify(Info *min_heap, int heap_size, int i)
{
    int leftIdx = i * 2;
    int rightIdx = i * 2 + 1;
    int smallest = i;
    if (leftIdx <= heap_size && min_heap[leftIdx].price < min_heap[i].price)
        smallest = leftIdx;
    else
        smallest = i;
    if (rightIdx <= heap_size && min_heap[rightIdx].price < min_heap[smallest].price)
        smallest = rightIdx;
    if (smallest != i)
    {
        Info temp = min_heap[i];
        min_heap[i] = min_heap[smallest];
        min_heap[smallest] = temp;
        min_heapify(min_heap, heap_size, smallest);
    }
}

Info *init_min_heap(int N, int A, int heap_size, int s[])
{
    Info *min_heap = (Info *)malloc((A * N + 1) * sizeof(Info));
    // 挑出 A*N 個價格, 以及來自哪張股票
    for (int i = 0; i < A; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            min_heap[i * N + j].price = price(s[i], j);
            min_heap[i * N + j].s_idx = i;
        }
    }

    // heapify
    for (int i = heap_size / 2; i >= 1; i--)
        min_heapify(min_heap, heap_size, i);

    return min_heap;
}

unsigned long long *find_k_sorted_price(int A, int N, Info *min_heap, int heap_size, int s[])
{
    unsigned long long *brain_k_sorted_price = (unsigned long long *)calloc(MAX_k, sizeof(unsigned long long));
    int *next_t = (int *)malloc(A * sizeof(int)); // 存每個 s_idx 對應的股票下一個要補進來的 t
    for (int i = 0; i < A; i++)
        next_t[i] = N + 1;

    for (int i = 1; i <= MAX_k - 1; i++)
    {
        // 抓出 min_heap 中最小 price 的出來 insert 進 brain_k_sorted_price
        brain_k_sorted_price[i] = min_heap[1].price;

        // 將 top node 換成同一張股票的下一個價格，
        int s_idx = min_heap[1].s_idx;
        min_heap[1].price = price(s[s_idx], next_t[s_idx]);
        min_heapify(min_heap, heap_size, 1);
        next_t[s_idx] += 1;
    }
    return brain_k_sorted_price;
}

int find_index(unsigned long long *brain_k_sorted_price, unsigned long long target_price, int size)
{
    int left = 1;
    int right = size;
    if (brain_k_sorted_price[right] <= target_price)
        return right;
    else if (brain_k_sorted_price[left] > target_price)
        return 0;
    else
    {
        while (left != right - 1)
        {
            int mid = (left + right) / 2;
            if (target_price >= brain_k_sorted_price[mid])
                left = mid;
            else
                right = mid;
        }
        return left;
    }
}

int compare(const void *data1, const void *data2)
{
    unsigned long long *ptr1 = (unsigned long long *)data1; //指標轉型
    unsigned long long *ptr2 = (unsigned long long *)data2; //指標轉型
    if (*ptr1 < *ptr2)
        return -1;
    else if (*ptr1 > *ptr2)
        return 1;
    else
        return 0;
}

int main()
{
    // 輸入
    int A, Q, N; // Brain's stock 數, 問題數, 通膨週期
    int s[MAX_A] = {0};
    scanf("%d %d %d", &A, &Q, &N);
    for (int i = 0; i < A; i++)
    {
        scanf("%d", &s[i]);
    }

    // build basical min_heap
    int min_heap_size = N * A;
    Info *min_heap = init_min_heap(N, A, min_heap_size, s);
    unsigned long long *brain_k_sorted_price = find_k_sorted_price(A, N, min_heap, min_heap_size, s); // idx from 1 to k

    // Question
    int k;
    int s_extra;
    int uncertained_range = 2 * N - 1;
    for (int i = 0; i < Q; i++)
    {
        scanf("%d %d", &s_extra, &k);
        if (s_extra == 0)
            printf("%llu\n", brain_k_sorted_price[k]);
        else
        {
            // 確保 (right - 1) * (2N - 1) >= k
            int left = 1;                                                           // 左搜索點
            int right = (int)(ceil((double)(k) / (double)(uncertained_range))) + 1; // 右搜索點
            int left_limit = 1;
            int right_limit = uncertained_range * right;

            // 縮小 left, right 範圍
            while (left != right - 1)
            {
                int mid = (left + right) / 2;
                int target_t = (mid - 1) * uncertained_range + N;
                int numOfSmaller = (mid - 1) * uncertained_range;
                int brain_array_idx = find_index(brain_k_sorted_price, price(s_extra, target_t), k);

                if (numOfSmaller + brain_array_idx >= k)
                {
                    right = mid;
                    right_limit = right * uncertained_range;
                }
                else if (numOfSmaller + uncertained_range + brain_array_idx <= k)
                {
                    left = mid;
                    left_limit = (left - 1) * uncertained_range + 1;
                }
                else
                    break;
            }
            
            int search_left = left_limit == 1 ? 0 : left_limit;
            int search_right = (k + N) < right_limit ? (k + N) : right_limit;
            right_limit = search_right;
            int size = right_limit - left_limit + 1;
            unsigned long long *sorted_extra_s = (unsigned long long *)malloc((size) * sizeof(unsigned long long));
            for (int j = 0; j < size; j++)
                sorted_extra_s[j] = price(s_extra, left_limit + j);
            qsort(sorted_extra_s, size, sizeof(unsigned long long), compare);
            search_right = k < search_right ? k : search_right;
            right_limit = search_right;
            while (search_left <= search_right)
            {
                int search_mid = (search_left + search_right) / 2;
                unsigned long long l1 = (search_mid == 0) ? 0 : sorted_extra_s[search_mid - left_limit];
                unsigned long long r1 = (search_mid == right_limit) ? ULLONG_MAX : sorted_extra_s[search_mid + 1 - left_limit];
                int brain_t = k - search_mid;
                unsigned long long l2 = brain_t == 0 ? 0 : brain_k_sorted_price[brain_t];
                unsigned long long r2 = brain_t == k ? ULLONG_MAX : brain_k_sorted_price[brain_t + 1];

                if (l1 <= r2 && l2 <= r1)
                {
                    unsigned long long target_price = (l1 >= l2) ? l1 : l2;
                    printf("%llu\n", target_price);
                    break;
                }
                else if (l1 > r2) // 往左半邊找
                {
                    search_right = search_mid - 1;
                }
                else if (l2 > r1) // 往右半邊找
                {
                    search_left = search_mid + 1;
                }
            }
        }
    }
}