# **DSA-Hw2**

## **Problem 4 - Teacher Lightning Bear’s Kingdom**
本題要求ㄧ city 前往 S, R 兩條路徑上最後一個相同的 city 使得兩條路徑前半段完全相同，可以將問題反過來看待成求分別從 S, R 出發，第一個出現在 S 也出現在 R 兩條路徑上的 city。

將各 city 間的路徑畫出來後可以觀察到以下幾點：
1. 分別以 S, R 做為起點 BFS 累計每個 city 的 level-sum， S-R路徑上的 city 的 level-sum 與 S, R的 level-sum 相同，這些 city 的答案就是本身。其餘 city 的 level-sum 必定大於 S,R 的 level-sum，這些 city 的答案會與 自即上個 city 的答案相同，以 tree 的概念來看，這些 city 的答案與他們的 parent 的答案相同。
2. 若已知一個 city 的答案，其路徑上的 child city 的答案也會是已知的
3. 以 S, R 的 level-sum 作為 critical level sum，若一個 city 在一次的 BFS 中的 level 就大於等於 critical level sum，就代表該 city 不在 S-R 的路徑上，他的答案必為其 parent 的答案。
4. 如果在第一輪(以 S 作為起點)BFS 就已經知道該 city 的答案，根據第二點，他以及他的 child city 在第二輪 BFS 都可以不用再 traverse 一遍。


`level-order traversal BFS `
```C
BFS(Graph *graph, int *result, int *levelSum, Queue *cityQueue, int *critical_ptr, const int S, int R)
{
    level = 0
    enqueue(S,0)
    while (cityQueue->length != 0)
    {
        level += 1;
        numOfNodes = cityQueue->length; // 同個 level 要處理的 nodes 數
        for (int i = 1 to numOfNodes)
        {
            currentCity, parentCity = dequeue(cityQueue)
            if (currentCity == R)
                critical_level_sum = 1 + level
                result[currentCity] = currentCity; // 答案就是自己
        }

        // 填入 currentCity 的 level
        levelSum[currentCity] += level;

        // 當前 level >= critical_level_sum result 就是 parentCity 的 result
        if (level >= critical_level_sum || levelSum[currentCity] > critical_level_sum)
            result[currentCity] = result[parentCity];
        else
        {
            // 第二次 BFS 才確認在 S-R路徑上的 city result
            if (levelSum[currentCity] - level != 0)
                result[currentCity] = currentCity;
        }

        // enqueue 下一個 level 要處理的 citys (在 adj-list 的 idx = currentCity 找)
        node = graph->adjList[currentCity];
        while (node != NULL)
        {
            // 如果 node 為本輪的 currrentCity 或者該 node 已經有答案，就不繼續處理
            if (node->city != parentCity && result[node->city] == 0)
                enqueue(cityQueue, node->city, currentCity);
            node = node->next;
        }
    }
}

```