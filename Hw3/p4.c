// ref: https://www.prepbytes.com/blog/strings/minimum-characters-required-to-add-to-given-string-to-make-it-palindrome/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 10000001

char select_char(char *foward, char *reverse, int length, int index)
{
    char selected;
    if (index == length)
        selected = ' ';
    else if (index > length)
        selected = reverse[index - length - 1];
    else
        selected = foward[index];
    return selected;
}

// LPS means Longest Prefix that is suffix
int *compute_LPS(char *foward, char *reverse, int length)
{
    int LPS_length = length * 2 + 1;
    int *LPS = (int *)malloc(sizeof(int) * LPS_length);
    LPS[0] = -1;
    int k = -1;
    char prefix, suffix;
    for (int i = 1; i < LPS_length; i++)
    {
        suffix = select_char(foward, reverse, length, i);
        prefix = select_char(foward, reverse, length, k + 1);
        while (k > -1 && suffix != prefix)
        {
            k = LPS[k];
            prefix = select_char(foward, reverse, length, k + 1);
        }
        if (prefix == suffix)
            k = k + 1;
        LPS[i] = k;
    }
    return LPS;
}

void printTwoString(char *first, char *second)
{
    printf("%s", first);
    printf("%s\n", second);
}

int main()
{
    // input
    char *foward = (char *)malloc(sizeof(char) * MAX_LEN);
    char *reverse = (char *)malloc(sizeof(char) * MAX_LEN);
    scanf("%s\n", foward);
    int length = strlen(foward); // 輸入字串長度

    // reverse
    if (length > 0)
    {
        for (int i = 0; i < length; i++)
            reverse[i] = foward[length - 1 - i];
    }
    reverse[length] = '\0';

    // calculate LPS of foward string
    int LPS_length = length * 2 + 1;
    int *foward_LPS = compute_LPS(foward, reverse, length);
    int *reverse_LPS = compute_LPS(reverse, foward, length);


    if (foward_LPS[LPS_length - 1] + 1 == length)
    {
        printf("0\n");
        printf("%s\n", foward);
    }
    else
    {
        // calculate LPS of reversed string
        int *reverse_LPS = compute_LPS(reverse, foward, length);

        // calculate max length of sysmetric substring
        int foward_palindrome = foward_LPS[LPS_length - 1] + 1;
        int reverse_palindrome = reverse_LPS[LPS_length - 1] + 1;

        // print result
        if (foward_palindrome > reverse_palindrome)
        {
            int numOfNeeded = length - foward_palindrome;
            reverse[numOfNeeded] = '\0';
            printf("%d\n", numOfNeeded);
            printTwoString(reverse, foward);
        }
        else if (foward_palindrome == reverse_palindrome)
        {
            int numOfNeeded = length - foward_palindrome;
            printf("%d\n", numOfNeeded);

            char temp = reverse[numOfNeeded];
            reverse[numOfNeeded] = '\0';
            printTwoString(reverse, foward);

            reverse[numOfNeeded] = temp;
            foward[numOfNeeded] = '\0';
            printTwoString(foward, reverse);
        }
        else
        {
            int numOfNeeded = length - reverse_palindrome;
            foward[numOfNeeded] = '\0';
            printf("%d\n", numOfNeeded);
            printTwoString(foward, reverse);
        }
    }
}

