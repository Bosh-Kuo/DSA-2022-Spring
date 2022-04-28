# **DSA-Hw0**

## **Problem 1 - Greatest Common Divisor of Big Integers (Programming)**

### **大數運算**
測試function
```C
// test: 輸入
printf("%s length: %lu\n", input1, strlen(input1));
printf("%s length: %lu\n", input2, strlen(input2));

// test: print BigInt
printBigInt(num1, ARR_SPACE);
printf("\n");
printBigInt(num2, ARR_SPACE);

test: compare and swap, than print Big and small:
if (bigIntCompare(num2, num1, ARR_SPACE))
{
    // pointer 指向互換
    swapArray(&bigNum, &smallNum);
    printf("\n");
    printBigInt(bigNum, ARR_SPACE);
    printf("\n");
    printBigInt(smallNum, ARR_SPACE);
    printf("\n");

    // pointer 指向再次互換
    swapArray(&bigNum, &smallNum);
    printBigInt(bigNum, ARR_SPACE);
    printf("\n");
    printBigInt(smallNum, ARR_SPACE);
}

// test: 大數減法
bigIntSub(bigNum, smallNum, bigNum, ARR_SPACE);
printBigInt(bigNum, ARR_SPACE);

// test: 大數乘法(*2)
bigIntMultiTwo(bigNum, bigNum, ARR_SPACE);
printBigInt(bigNum, ARR_SPACE);

// test: 大數除法(/2)
bigIntDivideTwo(bigNum, bigNum, ARR_SPACE);
printBigInt(bigNum, ARR_SPACE);

// test: 大數比較
printf("%s", formatBool(bigIntCompare(num1, num2, ARR_SPACE)));
printf("\n");

// test: 大數 isZero
printf("%s", formatBool(bigIntIsZero(num1, ARR_SPACE)));
printf("\n");

// test: 大數 isEven
printf("%s", formatBool(bigIntIsEven(bigNum, ARR_SPACE)));
printf("\n");
```
reference: https://openhome.cc/zh-tw/algorithm/numbers/bigint/


---

## **Problem 2 - Nonogram Solver (Programming)**

### **first version**
1. 讀row, col數
2. 創建二維整數陣列 dim 0 = row, dim 1 = col
3. 創建一維整數指標陣列，存二維陣列每個row的開頭位置
4. 把一維指標陣列的記憶體位置assign給gameBoardPtr->cells (int**)

#### 檢測 isBoardValid:

`在已經有累計黑色的情況下碰到白色或是已經到最末端 -> 檢查`

```
1. 該行hint為0但是有累積黑色格子:   
hint:0  
_o__ -> false

2. 累計黑色數與hint不符：  
hint: 1 2  
_o__o    

3. 檢查到最末端時檢查次數與hint length不一樣長
hint: 1 1  
____o -> false  
o_o_o -> false 
o___o -> true  
o_o__ -> true  
```

---
## **Problem 3**

### **Problem 3-(a) Swaps two arrays using pointers.**
Ans:  
(1) real_a  
(2) real_b  
(3) real_b  
(4) tmp  

### **Problem 3-(b) An array supporting negative indices.**
Ans:  
(1) MAXN

### **Problem 3-(c) Traverses data nodes in a linked list. Please familiarize yourself with linked list in advance. Related topics are covered in Prof. Liu’s videos.**
Ans:  
(1) root != NULL
(2) tmp->data
(3) tmp->nxt


### **Problem 3-(d) Traverses data nodes in a binary tree. Please familiarize yourself with binary trees in advance. Related topics are covered in Prof. Liu’s videos.**
Ans:  
(1) root != NULL
(2) root->left
(3) root->right
(4) root