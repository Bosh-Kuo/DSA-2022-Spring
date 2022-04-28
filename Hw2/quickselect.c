// ref: textbook p.154
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "price.h"
#define MAX_A 1024
#define MAX_k 1000001

void swap(unsigned long long *A, int left, int right)
{
    unsigned long long temp = A[left];
    A[left] = A[right];
    A[right] = temp;
}

int partition(unsigned long long *A, unsigned long long pivot, int left, int right)
{
    // int pivot = left;
    while (left < right)
    {
        while (A[left] < pivot)
            left++;
        while (A[right] > pivot)
            right--;
        if (A[left] == A[right])
            left++;
        else if (left < right)
            swap(A, left, right);
    }
    return right;
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

unsigned long long get_median_pivot(unsigned long long *A, int left, int right)
{

    int length_A = right - left + 1;
    if (length_A == 1)
        return A[0];
    else
    {
        int numOfBuckets = (int)(ceil((double)(length_A) / 5));
        unsigned long long *medians = (unsigned long long *)malloc(sizeof(unsigned long long) * numOfBuckets);
        int medianIdx = 0;
        while (left <= right)
        {
            int temp_size = (right - left + 1) < 5 ? (right - left + 1) : 5;
            unsigned long long *temp = (unsigned long long *)malloc(sizeof(unsigned long long) * temp_size);
            for (int i = 0; i < temp_size; i++)
            {
                temp[i] = A[left];
                left++;
            }
            qsort(temp, temp_size, sizeof(unsigned long long), compare);
            medians[medianIdx] = temp[temp_size / 2];  // 提出該 bucket 的 median
            medianIdx++;
        }
        return get_median_pivot(medians, 0, numOfBuckets - 1);
    }
}

unsigned long long find_k_smallest(unsigned long long *A, int k, int left, int right)
{
    int length_A = right - left + 1;
    if (length_A == 1)
        return A[left];
    else
    {
        // partition
        unsigned long long pivot = get_median_pivot(A, left, right);
        int pivot_position = partition(A, pivot, left, right); // 回傳該 pivot 在 sorted array 的 index
        printf("pivot: %llu\n", pivot);
        printf("pivot_position: %d\n", pivot_position);
        for (int i = left; i <= right; i++)
        {
            printf("%llu ", A[i]);
        }
        printf("\n");

        int order = pivot_position - left + 1; // 該 pivot 在 A 中排第幾小
        if (k == order)
            return A[pivot_position];
        else if (order > k)
        {
            printf("向左找\n");
            return find_k_smallest(A, k, left, pivot_position - 1); // 第k小在左半邊
        }
        else
        {
            printf("向右找\n");
            return find_k_smallest(A, k - order, pivot_position + 1, right); // 第k小在右半邊
        }
    }
}

int main()
{
    // unsigned long long A[9] = {1, 2, 45, 134, 134, 6, 3, 3, 3,};
    // int k = 4;
    // unsigned long long test = find_k_smallest(A, k, 5, 8);
    // printf("%d-th: %llu\n", k, test);
    int a = 2 + 1 < 2 ? 3 : 4;
    int b = (2 + 1) < 2 ? 3 : 4;
    printf("%d\n", a);
    printf("%d\n", b);
}