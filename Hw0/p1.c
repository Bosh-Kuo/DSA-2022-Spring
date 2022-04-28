#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define formatBool(b) ((b) ? "true" : "false")
#define MAX_DIGIT 256
#define ARR_SPACE 257

// 字元轉整數
int charToDigit(char a)
{
    return a - '0';
}

// 將char陣列轉為int大數陣列做運算(輸入不考慮負數)
void toIntArray(char str[], int num[])
{
    int charIdx = strlen(str) - 1;           // char array最後一位為個位數
    for (int i = ARR_SPACE - 1; i >= 0; i--) //從個位數開始填入num[]，
    {
        if (charIdx >= 0)
            num[i] = charToDigit(str[charIdx]);
        else
            num[i] = 0;
        charIdx--;
    }
}

// 大數減法(不考慮負數)
void bigIntSub(int num1[], int num2[], int *diff)
{
    int borrow = 0;
    for (int i = ARR_SPACE - 1; i >= 0; i--)
    {
        diff[i] = num1[i] - num2[i] - borrow;
        if (diff[i] >= 0)
        {
            borrow = 0;
        }
        else
        {
            borrow = 1;
            diff[i] = diff[i] + 10;
        }
    }
}

// 大數乘法
void bigIntMultiTwo(int num[], int ans[])
{
    int carry = 0;
    const int multiNum = 2;
    for (int i = ARR_SPACE - 1; i >= 0; i--)
    {
        ans[i] = num[i] * multiNum + carry;
        if (ans[i] < 10)
        {
            carry = 0;
        }
        else
        {
            ans[i] = ans[i] - 10;
            carry = 1;
        }
    }
}

// 大數除法(只考慮*2)
void bigIntDivideTwo(int num[], int ans[])
{
    int temp, remain = 0;
    const int divNum = 2;
    for (int i = 0; i < ARR_SPACE; i++)
    {
        temp = num[i] + remain;
        ans[i] = temp / divNum;
        remain = (temp % divNum) * 10;
    }
}

// num1 >= num2回傳true
bool bigIntCompare(int num1[], int num2[])
{
    int *sub = calloc(ARR_SPACE, sizeof(int));
    bigIntSub(num1, num2, sub);
    if (sub[0] == 0)
    {
        free(sub);
        return true;
    }
    else
    {
        free(sub);
        return false;
    }
}

bool bigIntIsZero(int num[])
{
    bool isZero = true;
    for (int i = 0; i < ARR_SPACE; i++)
    {
        if (num[i] != 0)
            return false;
    }
    return true;
}

bool bigIntIsEven(int num[])
{
    return (num[ARR_SPACE - 1] % 2 == 0 ? true : false);
}

void swapArray(int **a, int **b)
{
    int *tmp = *a;
    *a = *b;
    *b = tmp;
}

// 印出大數
void printBigInt(int num[])
{
    bool start = false;
    for (int i = 0; i < ARR_SPACE; i++)
    {
        if (num[i] != 0)
            start = true;
        if (start)
        {
            int digit = ARR_SPACE - i;
            printf("%d", num[i]);
        }
    }

    // BigInt: 0
    if (!start)
        printf("%d", num[ARR_SPACE - 1]);
}

int main()
{
    // const int MAX_DIGIT = 256;
    // const int ARR_SPACE = MAX_DIGIT + 1;
    char input1[ARR_SPACE], input2[ARR_SPACE];
    int num1[ARR_SPACE] = {0}, num2[ARR_SPACE] = {0}, power = 0;
    int *bigNum = num1;
    int *smallNum = num2;

    // Input
    scanf("%s", input1);
    scanf("%s", input2);

    // char[] to int[]
    toIntArray(input1, num1);
    toIntArray(input2, num2);
    if (!bigIntCompare(num1, num2))
        swapArray(&bigNum, &smallNum);

    // Binary Algorithm for Greatest Common Divisor
    while (!bigIntIsZero(bigNum) && !bigIntIsZero(smallNum))
    {
        if (bigIntIsEven(bigNum) && bigIntIsEven(smallNum))
        {
            power += 1;
            bigIntDivideTwo(bigNum, bigNum);
            bigIntDivideTwo(smallNum, smallNum);
        }
        else if (bigIntIsEven(smallNum))
            bigIntDivideTwo(smallNum, smallNum);
        else if (bigIntIsEven(bigNum))
            bigIntDivideTwo(bigNum, bigNum);
        else
        {
        }

        if (bigIntCompare(smallNum, bigNum))
            swapArray(&bigNum, &smallNum);

        bigIntSub(bigNum, smallNum, bigNum);
    }

    for (int i = 0; i < power; i++)
    {
        bigIntMultiTwo(smallNum, smallNum);
    }
    printBigInt(smallNum);
    printf("\n");
}

/*
ref:
print bool: https://www.codegrepper.com/code-examples/c/how+to+print+boolean+in+c
bigInt: https://openhome.cc/zh-tw/algorithm/numbers/bigint/
array swap: https://stackoverflow.com/questions/49751409/swapping-2-arrays-in-c
*/