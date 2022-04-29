# **DSA-Hw3**

## **Problem 0 - Proper References**


## **Problem 1 - Cracking The Interview With How-How!**
### 1.

Ans: 7  

<br>

### 2.
`human algorithm`:  
藉由 merge-sort 的概念，將問題轉為重複比較 sorted left array 與 sorted right array 然後將兩 sub-array 合併成一 sorted array。當 left-sub-array[leftIdx] < right-sub-array[rightIdx]，表示 left-sub-array[leftIdx] < right-sub-array[rightIdx] < ... < right-sub-array[rightSub.length]
，便能累計一次 Merge 中的 feasible pairs

`pseudo code`:
```C
divide-and-conquer(array A, int pairs):
    length = A.length 
    if length >= 2:
        m = floor((1+n)/2)
        pairs = divide-and-conquer(A[1...m],  pairs)
        pairs = divide-and-conquer(A[m+1...length],  pairs)

        leftSub = A[1...m]
        leftSub.insert_to_end(Inf)  // suppose O(1)
        rightSub = A[m+1...length]
        rightSub.insert_to_end(Inf)  // suppose O(1)
        leftIdx = 1, rightIdx = 1
        leftLength = m, rightLength = length - m
        
        for i = 1 to i = length:
            if (leftSub[leftIdx] < rightSub[rightIdx])
                A[i] = leftSub[leftIdx]
                pairs += (rightLength - rightIdx + 1)
                leftIdx += 1
            else
                A[i] = rightSub[rightIdx]
                rightIdx += 1
    return pairs

find-pairs(array A):
    pairs = 0
    pairs = divide-and-conquer(A, pairs)
    return pairs
```

`time complexity`:  
共分割了 n 次，分割的 time complexity 為 O(n)，共合併了 log(n)次，每次合併平均的 time complexity 為 O(n)，合併的 time complexity 為 nlog(n)，整體 time complexity 為 O(n) + O(nlog(n))


<br>


### 3-a.
Ans: 5,2,4,3


### 3-b.
在 while(!Ordered(P)) loop 中，function Ordered () 的 time complexity 為 $O(n)$，執行 while (i+1 < len(P)) loop 的 time complexity 為 $O(n)$，也就是說 while(!Ordered(P)) loop 內的 time complexity 為 $O(n) + O(n) = O(n)$。  
最糟糕的情況為： $k = n-1$，且在 while(!Ordered(P)) loop 內每次只移除 2 個 element，因此執行了 $floor(\frac{n-1}{2}$)次的 while(!Ordered(P)) loop，因此最糟糕的情況下的 time complexity 為 $floor(\frac{n-1}{2}) \cdot O(n) = O(n^2)$


<br>

### 4.

若每次觸發 `if (P[i]. difficulty > P[i +1]. difficulty ) -> remove P[i] and P[i+1]` 時，P[i], P[i+1]只有其中一者為 misplaced problem，演算法結束時最多會觸發 k 次。 因此移除的 problems 數最多為 2k 個。

<br>

### 5.
`human algorithm`:   
上述 pseudo code 若觸發了`if (P[i]. difficulty > P[i +1]. difficulty ) -> remove P[i] and P[i+1]` 後 p[i-1] > p[i] 需要待下一次觸發 `while(!Ordered(P))` 才會被檢查到，因此改成在同個 `while (i+1 < len (P))` loop 中可以向前檢查就可以在 O(n) 內達到同樣的效果。

`pseudo code`:
```C
Remove-out-of-order-pairs(Problem P []):
    i = 0
    while (i < len (P)):
        if (i+1 < len(P) and P[i].difficulty > P[i+1].difficulty ):
            remove (P[i]) 
            remove(P[i])
        else if (i-1 >= 0 and P[i-1].difficulty > P[i].difficulty):
            i = i-1
            remove (P[i])
            remove (P[i])
        else :
            i += 1
```

<br>

### 6.
`human algorithm`:  
將 P array 中的 unordered problems 與 ordered problems 分別存在兩個 array 中，
再用 Merge sort 將存 unordered problems 的 array 進行排序，最後將排序好的 unordered problems array 與原本就已經排序好的 ordered problems array merge 起來。


```C
(index start from 1)
Merge(array A1, array A2):
    leftLength = A1.length
    rightLength = A2.length
    length = leftLength + rightLength
    array A[length] = {0}
    A1.insert_to_end(Inf)  // suppose O(1)
    A2.insert_to_end(Inf)  // suppose O(1)
    leftIdx = 1, rightIdx = 1
    
    for i = 1 to i = length:
        if (leftSub[leftIdx] < rightSub[rightIdx])
            A[i] = leftSub[leftIdx]
            leftIdx += 1
        else
            A[i] = rightSub[rightIdx]
            rightIdx += 1


Merge-Sort(array A, front, end):
    length = A.length
    if (length >= 2):
        mid = floor((front+end)/2)
        A1 = Merge-Sort(A[1...m])
        A2 = Merge-Sort(A[m+1...length])
        A = Merge(A1, A2)
    return A


Sort-P(Problem P []):
    unordered_array = empty array
    ordered_array = empty array
    unordered_array_idx = 1
    ordered_array_idx = 1

    for (i = 1 to P.length):
        ordered_array[ordered_array_idx] = P[i]
        if (ordered_array.length >= 2 and ordered_array[ordered_array_idx] < ordered_array[ordered_array_idx-1]):
            unordered_array[unordered_array_idx] = ordered_array[ordered_array_idx]
            unordered_array_idx += 1
            unordered_array[unordered_array_idx] = ordered_array[ordered_array_idx-1]
            unordered_array_idx += 1
            ordered_array_idx -= 2
        ordered_array_idx ++
    
    sorted_unordered_array = Merge-Sort(unordered_array, 1, unordered_array.length)
    sorted_P = Merge(ordered_array, sorted_unordered_array)
    return Sort-P
```

`time complexity`:   
- 將unordered problems 與 ordered problems 分別存在兩個 array 中的 time complexity 為 $O(n)$
- 由於 unordered problems 的數目最多為 2k，因此排序 unordered problems array 的 time complexity 為 $O(klogk)$
- Merge 排序好的 unordered problems array 與原本就排序好的ordered problems array 的 time complexity 為 $O(n)$
- 整體 time complexity 為 $O(n) + O(klogk) + O(n)$ = $O(n + klogk)$