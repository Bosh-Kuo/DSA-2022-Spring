#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct heightNode
{
    int height;
    struct heightNode *last;
} heightNode;

typedef struct addedSetNode
{
    int setIdx;
    struct addedSetNode *last;
} addedSetNode;

typedef struct DisjointSet
{
    int parentIdx;
    addedSetNode *addedSetList; // 其他 set 併入的歷史紀錄（當前->...->初始）
    heightNode *heightList;     // 該 set 隨其他 set 併入的高度歷史（當前->...->初始）

} DisjointSet;

heightNode *createHighNode(int height)
{
    heightNode *newHighNode = (heightNode *)malloc(sizeof(heightNode));
    newHighNode->height = height;
    return newHighNode;
}

addedSetNode *createAddedSetNode(int setIdx)
{
    addedSetNode *newAddedSetNode = (addedSetNode *)malloc(sizeof(addedSetNode));
    newAddedSetNode->setIdx = setIdx;
    return newAddedSetNode;
}

void makeSet(int setIdx, DisjointSet *setArray, bool *setExist)
{
    setArray[setIdx].parentIdx = setIdx;
    setArray[setIdx].addedSetList = NULL;
    setArray[setIdx].heightList = createHighNode(1);
    setArray[setIdx].heightList->last = NULL;
    setExist[setIdx] = true;
}

int findSet(int setIdx, DisjointSet *setArray, bool *setExist)
{
    // 若 set 不存在，先 makeSet
    if (!setExist[setIdx])
        makeSet(setIdx, setArray, setExist);

    int parentIdx = setIdx;
    // 找到 root 的 Idx
    if (setArray[setIdx].parentIdx != setIdx)
        parentIdx = findSet(setArray[setIdx].parentIdx, setArray, setExist);
    return parentIdx;
}

// 若 Union 失敗回傳 false, 成功回傳 true
bool Union(int set_A_Idx, int set_B_Idx, DisjointSet *setArray, bool *setExist)
{
    int root_a = findSet(set_A_Idx, setArray, setExist);
    int root_b = findSet(set_B_Idx, setArray, setExist);

    // 兩 set 其實是同個 set 就不能合併
    if (root_a == root_b)
        return false;

    // 決定誰要併入誰 (second_root 併入 main_root)
    int main_root, second_root;
    if (setArray[root_a].heightList->height > setArray[root_b].heightList->height)
    {
        main_root = root_a;
        second_root = root_b;
    }
    else if (setArray[root_b].heightList->height > setArray[root_a].heightList->height)
    {
        main_root = root_b;
        second_root = root_a;
    }
    else
    {
        // 高度一樣時，編號大的併入編號小的
        if (root_a < root_b)
        {
            main_root = root_a;
            second_root = root_b;
        }
        else
        {
            main_root = root_b;
            second_root = root_a;
        }
    }
    // 更新 second_root 的 root
    setArray[second_root].parentIdx = main_root;

    // 新增高度歷史紀錄
    int newHeight = ((setArray[second_root].heightList->height + 1) > setArray[main_root].heightList->height) ? (setArray[second_root].heightList->height + 1) : setArray[main_root].heightList->height;
    heightNode *newHighNode = createHighNode(newHeight);
    newHighNode->last = setArray[main_root].heightList;
    setArray[main_root].heightList = newHighNode;

    // 新增合併歷史紀錄
    addedSetNode *newAddedSetNode = createAddedSetNode(second_root);
    newAddedSetNode->last = setArray[main_root].addedSetList;
    setArray[main_root].addedSetList = newAddedSetNode;
    return true;
}

void reverse(int set_A_Idx, int set_B_Idx, DisjointSet *setArray, bool *setExist)
{
    // 兩個 set 的 root 一定會一樣，那就從 Idx 編號小的開始找比較快
    int main_root = (set_A_Idx < set_B_Idx) ? findSet(set_A_Idx, setArray, setExist) : findSet(set_B_Idx, setArray, setExist);
    int second_root = setArray[main_root].addedSetList->setIdx;

    // second_root 將 parent 指回自己
    setArray[second_root].parentIdx = second_root;

    // 回到前一個高度歷史紀錄
    setArray[main_root].heightList = setArray[main_root].heightList->last;

    // 回到前一個合併歷史紀錄
    setArray[main_root].addedSetList = setArray[main_root].addedSetList->last;
}

void test1(int N, DisjointSet *setArray, bool *setExist)
{
    bool success;
    success = Union(1, 2, setArray, setExist);
    success = Union(3, 4, setArray, setExist);
    success = Union(5, 6, setArray, setExist);
    success = Union(2, 6, setArray, setExist);
    reverse(3, 4, setArray, setExist);
    reverse(2, 6, setArray, setExist);

    for (int i = 1; i <= N; i++)
    {
        printf("Set %d: root is %d\n", i, findSet(i, setArray, setExist));

        addedSetNode *addedSet = setArray[i].addedSetList;
        printf("addedSet list: ");
        while (addedSet != NULL)
        {
            printf("%d ", addedSet->setIdx);
            addedSet = addedSet->last;
        }
        printf("\n");

        heightNode *height = setArray[i].heightList;
        printf("height list: ");
        while (height != NULL)
        {
            printf("%d ", height->height);
            height = height->last;
        }
        printf("\n\n");
    }
}

typedef struct Operation
{
    char type; // M: merge, Q: query, B: boom, I: init
    int merge1;
    int merge2;
    int boom;
    bool undo;
    int parentIdx;

} Operation;

void add_OperationNode(Operation *operationNodeHistory, int day, char type, int merge1, int merge2, int boom, bool undo)
{
    operationNodeHistory[day].type = type;
    operationNodeHistory[day].merge1 = merge1;
    operationNodeHistory[day].merge2 = merge2;
    operationNodeHistory[day].boom = boom;
    operationNodeHistory[day].undo = undo;
    operationNodeHistory[day].parentIdx = -1;
}

void test2(int M, Operation *operationNodeHistory)
{
    for (int i = 0; i <= M; i++)
    {
        int currentDay = i;
        while (currentDay != -1)
        {
            printf("%d%c -> ", currentDay, operationNodeHistory[currentDay].type);
            currentDay = operationNodeHistory[currentDay].parentIdx;
        }
        printf("NULL \n\n");
    }
}

typedef struct Queue
{
    int *queue;
    int head;
    int tail;
    int length;
} Queue;
Queue *createQueue(int size)
{
    Queue *newQueue = (Queue *)malloc(sizeof(Queue));
    newQueue->queue = (int *)malloc((size + 1) * sizeof(int));
    return newQueue;
}
void initQueue(Queue *myQueue)
{
    myQueue->head = 0;
    myQueue->tail = -1;
    myQueue->length = 0;
}
void enqueue(Queue *myQueue, int day)
{
    myQueue->tail += 1;
    myQueue->queue[myQueue->tail] = day;
    myQueue->length += 1;
}
int dequeue(Queue *myQueue)
{
    myQueue->head += 1;
    myQueue->length -= 1;
    return myQueue->queue[myQueue->head - 1];
}
int pop(Queue *myQueue)
{
    myQueue->tail -= 1;
    myQueue->length -= 1;
    return myQueue->queue[myQueue->tail + 1];
}

void findShortPath(int currentDay, int targetDay, Queue *QueuefromCurrent, Queue *QueuefromTarget, Operation *operationNodeHistory)
{
    initQueue(QueuefromCurrent);
    initQueue(QueuefromTarget);

    // 從 current 出發
    while (currentDay != -1)
    {
        enqueue(QueuefromCurrent, currentDay);
        if (currentDay == targetDay)
        {
            enqueue(QueuefromTarget, targetDay);
            return;
        }
        currentDay = operationNodeHistory[currentDay].parentIdx;
    }
    // 從 target 出發
    while (targetDay != -1)
    {
        enqueue(QueuefromTarget, targetDay);
        targetDay = operationNodeHistory[targetDay].parentIdx;
    }

    // 找出最短路徑
    while (QueuefromCurrent->length > 1 && QueuefromTarget->length > 1 && QueuefromTarget->queue[QueuefromTarget->tail - 1] == QueuefromCurrent->queue[QueuefromCurrent->tail - 1])
    {
        int temp = pop(QueuefromCurrent);
        temp = pop(QueuefromTarget);
    }
}

int main()
{
    int N, M, shops;
    scanf("%d %d", &N, &M);
    shops = N;

    // init DisjointSet array and exist array
    DisjointSet *setArray = (DisjointSet *)malloc(sizeof(DisjointSet) * (N + 1));
    bool *setExist = (bool *)calloc((N + 1), sizeof(bool));

    // // test DisjointSet 功能
    // test(N, setArray, setExist);

    // 建立 operationNodeHistory  arrray (一維陣列，存Operation)
    Operation *operationNodeHistory = (Operation *)malloc(sizeof(Operation) * (M + 1));
    int currentDay = 0;
    add_OperationNode(operationNodeHistory, 0, 'I', -1, -1, -1, true);

    // 建立 boomQueue
    Queue *QueuefromCurrent = createQueue(M);
    Queue *QueuefromTarget = createQueue(M);

    // 建立 operation Queue
    Queue *operationQueue = createQueue(M);
    initQueue(operationQueue);
    enqueue(operationQueue, 0);

    // Input
    char opertation[10];
    for (int i = 1; i <= M; i++)
    {
        scanf("%s", opertation);
        if (strcmp(opertation, "merge") == 0)
        {
            int merge1, merge2;
            scanf("%d %d", &merge1, &merge2);

            // 加入 operationNodeHistory array
            add_OperationNode(operationNodeHistory, i, 'M', merge1, merge2, -1, false);

            // build tree
            operationNodeHistory[i].parentIdx = currentDay;
            currentDay = i;

            // 加入 operationQueue
            enqueue(operationQueue, i);
        }
        else if (strcmp(opertation, "query") == 0)
        {
            // 加入 operationNodeHistory array
            add_OperationNode(operationNodeHistory, i, 'Q', -1, -1, -1, true);

            // build tree
            operationNodeHistory[i].parentIdx = currentDay;
            currentDay = i;

            // 加入 operationQueue
            enqueue(operationQueue, i);
        }
        else if (strcmp(opertation, "boom") == 0)
        {
            int boom;
            scanf("%d", &boom);

            // 加入 operationNodeHistory array
            add_OperationNode(operationNodeHistory, i, 'B', -1, -1, boom, true);

            // 若 boom 到某天也是 boom 就繼續 boom (算是一種path compression 吧？)
            int targetDay = boom;
            while (operationNodeHistory[targetDay].type == 'B')
                targetDay = operationNodeHistory[targetDay].boom;

            // build tree
            operationNodeHistory[i].parentIdx = targetDay;
            // 直接移到 最後一個 boom 的那天
            currentDay = targetDay;

            // 加入 operationQueue
            while (operationNodeHistory[operationQueue->queue[operationQueue->tail]].type != 'I' && operationNodeHistory[operationQueue->queue[operationQueue->tail]].type != 'Q')
            {
                int temp = pop(operationQueue);
            }    
            enqueue(operationQueue, i);
        }
    }

    // 執行 operation Queue 的 operation
    currentDay = 0;
    while (operationQueue->length > 0)
    {
        int day = dequeue(operationQueue);
        
        // 執行 merge
        if (operationNodeHistory[day].type == 'M')
        {
            currentDay = day;
            int merge1 = operationNodeHistory[currentDay].merge1;
            int merge2 = operationNodeHistory[currentDay].merge2;
            bool success = Union(merge1, merge2, setArray, setExist);
            if (!success)
                operationNodeHistory[currentDay].undo = true;
            else
                shops -= 1;
        }        
        else if (operationNodeHistory[day].type == 'I')
            currentDay = day;
        // 執行 query
        else if (operationNodeHistory[day].type == 'Q')
        {
            currentDay = day;
            printf("%d\n", shops);
        }
        // 執行 boom
        else if (operationNodeHistory[day].type == 'B')
        {
            int targetDay = operationNodeHistory[day].boom;
            while (operationNodeHistory[targetDay].type == 'B')
                targetDay = operationNodeHistory[targetDay].boom;
            // printf("\ntargetDay: %d\n", targetDay);

            // 找 boom 最短路徑
            findShortPath(currentDay, targetDay, QueuefromCurrent, QueuefromTarget, operationNodeHistory);
            
            // 往共同祖先走
            // printf("\nreverse: ");
            while(QueuefromCurrent->length > 1)
            {
                currentDay = dequeue(QueuefromCurrent);   
                // printf("%d ", currentDay);             
                if (!operationNodeHistory[currentDay].undo)
                {
                    reverse(operationNodeHistory[currentDay].merge1, operationNodeHistory[currentDay].merge2, setArray, setExist);
                    shops += 1;
                }
            }
            // 往target祖先走
            // printf("\nto target: ");
            currentDay = pop(QueuefromTarget);
            while(QueuefromTarget->length > 0)
            {
                currentDay = pop(QueuefromTarget);   
                // printf("%d ", currentDay);                 
                if (!operationNodeHistory[currentDay].undo)
                {
                    bool success = Union(operationNodeHistory[currentDay].merge1, operationNodeHistory[currentDay].merge2, setArray, setExist);
                    if (!success)
                        operationNodeHistory[currentDay].undo = true;
                    else
                        shops -= 1;
                }
            }
        }
    }
}