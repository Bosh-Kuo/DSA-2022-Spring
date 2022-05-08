#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct strInfo
{
    long long Rabin_Karp;
    int index;
} strInfo;

void calculate_Robin_Karp(long long *Rabin_Karp_array, char **stringArray, int k, int l, long long d, long long q)
{
    long long t = 0;
    for (int i = 0; i < k; i++)
    {
        long long t = 0;
        for (int j = 0; j < l; j++)
        {
            t = (d * t + (stringArray[i][j] - '!')) % q;
        }
        Rabin_Karp_array[i] = t;
    }
}

int compare(const void *data1, const void *data2)
{
    strInfo s1 = *(strInfo *)data1; //指標轉型
    strInfo s2 = *(strInfo *)data2; //指標轉型
    if (s1.Rabin_Karp < s2.Rabin_Karp)
        return -1;
    else if (s1.Rabin_Karp > s2.Rabin_Karp)
        return 1;
    else
        return 0;
}

long long count_pairs(strInfo *sortedArray, int k)
{
    // 找相似
    long long pairs = 0;
    long long accumulate = 1;
    for (int i = 1; i < k; i++)
    {
        if (sortedArray[i].Rabin_Karp == sortedArray[i - 1].Rabin_Karp)
        {
            accumulate += 1;
            if (i == k - 1)
                pairs += accumulate * (accumulate - 1) / 2;
        }
        else
        {
            if (accumulate > 1)
                pairs += accumulate * (accumulate - 1) / 2;
            accumulate = 1;
        }
    }
    return pairs;
}

long long count_unmasked_pairs(strInfo *sortedArray, long long *Rabin_Karp_array, int k, int l)
{
    // 排列每個 string 沒被遮住一位數的 Rabin_Karp
    for (int i = 0; i < k; i++)
    {
        sortedArray[i].Rabin_Karp = Rabin_Karp_array[i];
        sortedArray[i].index = i;
    }
    // sort
    qsort(sortedArray, k, sizeof(strInfo), compare);

    // count pairs
    long long pairs = count_pairs(sortedArray, k);
    return pairs;
}

int main()
{
    // define d and q
    long long d = '~' - '!' + 1;     // 94
    long long q = LLONG_MAX / d + 1; // 98120979115476339

    // input k, l, flag
    int k, l, flag;
    scanf("%d %d %d", &k, &l, &flag);
    // create string Array
    char **stringArray = (char **)malloc(k * sizeof(char *));
    for (int i = 0; i < k; i++)
        stringArray[i] = (char *)malloc((l + 1) * sizeof(char));
    // input strings
    for (int i = 0; i < k; i++)
        scanf("%s", stringArray[i]);

    // init string robin-karp array and sorted robin-karp array
    long long *Rabin_Karp_array = (long long *)calloc(k, sizeof(long long));
    strInfo *sortedArray = (strInfo *)malloc(k * sizeof(strInfo));

    // calculate Robin Karp value of each string
    calculate_Robin_Karp(Rabin_Karp_array, stringArray, k, l, d, q);


    if (flag == 0) // 找到即可停止
    {
        long long mask = 1;
        int find = 0;

        // 從最小位數開始輪流遮
        for (int digit = 0; digit < l; digit++)
        {
            // 算每個 string 被遮住一位數的 Rabin_Karp
            for (int i = 0; i < k; i++)
            {
                long long masked_Rabin_Karp = (Rabin_Karp_array[i] - (stringArray[i][l - digit - 1] - '!') * mask) % q;
                sortedArray[i].Rabin_Karp = masked_Rabin_Karp < 0 ? masked_Rabin_Karp + q : masked_Rabin_Karp;
                sortedArray[i].index = i;
            }

            // sort
            qsort(sortedArray, k, sizeof(strInfo), compare);

            // 找相似
            for (int i = 1; i < k; i++)
            {
                if (sortedArray[i].Rabin_Karp == sortedArray[i - 1].Rabin_Karp)
                {
                    printf("Yes\n");
                    printf("%d %d\n", sortedArray[i - 1].index, sortedArray[i].index);
                    find = 1;
                    break;
                }
            }
            if (find == 1)
                break;

            // 更新下個位數的 mask
            mask = (mask * d) % q;
        }
        if (find == 0)
            printf("No\n");
    }
    else // 必須全部找完
    {
        long long mask = 1;
        long long unmasked_pairs = count_unmasked_pairs(sortedArray, Rabin_Karp_array, k, l);
        long long pairs = unmasked_pairs;

        // 從最小位數開始輪流遮
        for (int digit = 0; digit < l; digit++)
        {
            // 算每個 string 被遮住一位數的 Rabin_Karp
            for (int i = 0; i < k; i++)
            {
                long long masked_Rabin_Karp = (Rabin_Karp_array[i] - (stringArray[i][l - digit - 1] - '!') * mask) % q;
                sortedArray[i].Rabin_Karp = masked_Rabin_Karp < 0 ? masked_Rabin_Karp + q : masked_Rabin_Karp;
                sortedArray[i].index = i;
            }

            // sort
            qsort(sortedArray, k, sizeof(strInfo), compare);

            // count pairs
            pairs += count_pairs(sortedArray, k) - unmasked_pairs;

            // 更新下個位數的 mask
            mask = (mask * d) % q;
        }
        if (pairs != 0)
        {
            printf("Yes\n");
            printf("%lld", pairs);
        }
        else
            printf("No");
    }
}