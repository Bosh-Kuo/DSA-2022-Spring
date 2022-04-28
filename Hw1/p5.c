#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// for USB xor linked list
typedef struct USB
{
    int ID;
    struct USB *LRPtr;
} USB;

// for group xor linked list
typedef struct GroupNode
{
    int number;
    struct GroupNode *LRPtr;
} GroupNode;

// xor操作 for USB*
USB *xorUSB(USB *left, USB *right)
{
    return (USB *)((unsigned long)(left) ^ (unsigned long)(right));
}
// xor操作 for GroupNode*
GroupNode *xorGroupNode(GroupNode *left, GroupNode *right)
{
    return (GroupNode *)((unsigned long)(left) ^ (unsigned long)(right));
}

// for GroupInfo[M][K] Array
typedef struct Group
{
    USB *head;
    USB *tail;
    int length;
} Group;

// for BathroomInfo[M] Array
typedef struct Bathroom
{
    GroupNode *G_head;
    GroupNode *G_tail;
    int numOfGroup;
    int targetLine; // 每個隊伍有自己close後的目標隊伍編號
    int lastLine;   // 每個隊伍有自己的上一隊隊伍編號
} Bathroom;

// init GroupInfo[M][K]
Group **initGroupInfo(int M, int K)
{
    Group **GroupInfo = (Group **)malloc(M * sizeof(Group *));

    for (int i = 0; i < M; i++)
    {
        GroupInfo[i] = (Group *)malloc(K * sizeof(Group));
        for (int j = 0; j < K; j++)
        {
            GroupInfo[i][j].head = NULL;
            GroupInfo[i][j].tail = NULL;
            GroupInfo[i][j].length = 0;
        }
    }
    return GroupInfo;
}

// init BathroomInfo[M]
Bathroom *initBathroomInfo(int M)
{
    Bathroom *BathroomInfo = (Bathroom *)malloc(M * sizeof(Bathroom));
    for (int i = 0; i < M; i++)
    {
        BathroomInfo[i].G_head = NULL;
        BathroomInfo[i].G_tail = NULL;
        BathroomInfo[i].numOfGroup = 0;
        if (i == 0)
        {
            BathroomInfo[i].targetLine = M - 1;
            BathroomInfo[i].lastLine = i + 1;
        }
        else if (i == M - 1)
        {
            BathroomInfo[i].targetLine = i - 1;
            BathroomInfo[i].lastLine = 0;
        }
        else
        {
            BathroomInfo[i].targetLine = i - 1;
            BathroomInfo[i].lastLine = i + 1;
        }
    }
    return BathroomInfo;
}

void enter(Group **GroupInfo, Bathroom *BathroomInfo, int i, int j, int m)
{
    if (GroupInfo[m][i].length > 0) // group i 有人在隊伍中，USB from group i 可以直接插隊 => 不需更新 BathroomInfo
    {
        // init NewUSB
        USB *newUSB = (USB *)malloc(sizeof(USB));
        newUSB->ID = j;
        // update GroupInfo[m][i]
        USB *USB_tail = GroupInfo[m][i].tail;
        USB_tail->LRPtr = xorUSB(xorUSB(USB_tail->LRPtr, NULL), newUSB); // 舊 tail USB 接上新 tail USB
        newUSB->LRPtr = xorUSB(USB_tail, NULL);                          // 新 tail USB 接上舊 tail USB
        GroupInfo[m][i].tail = newUSB;
        GroupInfo[m][i].length += 1; // 更新 m 排 i group 人數
    }
    else // group i 沒人在隊伍中，USB from group i 沒得插隊，且 USB j 排入等於新增一個 group 進隊伍 => 需更新 BathroomInfo
    {
        if (BathroomInfo[m].numOfGroup == 0) // m 排沒有任何group
        {
            // BathroomInfo
            // init NewGroupNode
            GroupNode *NewGroupNode = (GroupNode *)malloc(sizeof(GroupNode));
            NewGroupNode->number = i;
            NewGroupNode->LRPtr = NULL;
            // update BathroomInfo[m]
            BathroomInfo[m].G_head = NewGroupNode;
            BathroomInfo[m].G_tail = NewGroupNode;
            BathroomInfo[m].numOfGroup += 1; // 更新 m 排 group 數
        }
        else // m 排有其他 group 在排
        {
            // BathroomInfo
            // init NewGroupNode
            GroupNode *NewGroupNode = (GroupNode *)malloc(sizeof(GroupNode));
            NewGroupNode->number = i;
            // update BathroomInfo[m]
            GroupNode *G_tail = BathroomInfo[m].G_tail;
            G_tail->LRPtr = xorGroupNode(xorGroupNode(G_tail->LRPtr, NULL), NewGroupNode); // 舊 tail GroupNode 接上新 tail GroupNode
            NewGroupNode->LRPtr = xorGroupNode(G_tail, NULL);                              // 新 tail GroupNode 接上舊 tail GroupNode
            BathroomInfo[m].G_tail = NewGroupNode;
            BathroomInfo[m].numOfGroup += 1; // 更新 m 排 group 數
        }
        // GroupInfo
        // init newUSB
        USB *newUSB = (USB *)malloc(sizeof(USB));
        newUSB->ID = j;
        newUSB->LRPtr = NULL;
        // update GroupInfo[m][i]
        GroupInfo[m][i].head = newUSB;
        GroupInfo[m][i].tail = newUSB;
        GroupInfo[m][i].length += 1; // 更新 m 排 i group 人數
    }
}

void leave(Group **GroupInfo, Bathroom *BathroomInfo, int m)
{
    // 找出 m 排最後一個 USB 的 group idx
    int groupIdx = BathroomInfo[m].G_tail->number;

    // 如果該 group 只剩一個 USB，leave 會造成 m 排的 group 數減少 => 需要更新 BathroomInfo[m]
    if (GroupInfo[m][groupIdx].length == 1)
    {
        // BathroomInfo
        if (BathroomInfo[m].numOfGroup == 1) // m 排只剩下該 group
        {
            BathroomInfo[m].G_head = NULL;
            BathroomInfo[m].G_tail = NULL;
        }
        else // m 排除了該 group 還有其他 group
        {
            GroupNode *G_tail = BathroomInfo[m].G_tail;
            GroupNode *lastGroupNode = xorGroupNode(G_tail->LRPtr, NULL);
            lastGroupNode->LRPtr = xorGroupNode(xorGroupNode(lastGroupNode->LRPtr, G_tail), NULL); // 將 G_tail 前一個 group node 更新為新的 G_tail
            BathroomInfo[m].G_tail = lastGroupNode;
        }
        BathroomInfo[m].numOfGroup -= 1; // m 排少一個group

        // GroupInfo
        GroupInfo[m][groupIdx].head = NULL;
        GroupInfo[m][groupIdx].tail = NULL;
    }
    else // 該 group 還有其他 USB，不需更新 BathroomInfo[m]
    {
        // GroupInfo
        USB *tailUSB = GroupInfo[m][groupIdx].tail;
        USB *lastUSB = xorUSB(tailUSB->LRPtr, NULL);
        lastUSB->LRPtr = xorUSB(xorUSB(lastUSB->LRPtr, tailUSB), NULL);
        GroupInfo[m][groupIdx].tail = lastUSB;
    }
    GroupInfo[m][groupIdx].length -= 1; // 該 group 少一個 USB
}

void go(Group **GroupInfo, Bathroom *BathroomInfo, int m)
{
    // 找出 m 排第一個 USB 的 group idx
    int groupIdx = BathroomInfo[m].G_head->number;

    // 如果該 group 只剩一個 USB，go 會造成 m 排的 group 數減少 => 需要更新 BathroomInfo[m]
    if (GroupInfo[m][groupIdx].length == 1)
    {
        // BathroomInfo
        if (BathroomInfo[m].numOfGroup == 1) // m 排只剩下該 group
        {
            BathroomInfo[m].G_head = NULL;
            BathroomInfo[m].G_tail = NULL;
        }
        else // m 排除了該 group 還有其他 group
        {
            GroupNode *G_head = BathroomInfo[m].G_head;
            GroupNode *nextGroupNode = xorGroupNode(NULL, G_head->LRPtr);
            nextGroupNode->LRPtr = xorGroupNode(NULL, xorGroupNode(G_head, nextGroupNode->LRPtr)); // 將 G_head 後一個 group node 更新為新的 G_head
            BathroomInfo[m].G_head = nextGroupNode;
        }
        BathroomInfo[m].numOfGroup -= 1; // m 排少一個group

        // GroupInfo
        GroupInfo[m][groupIdx].head = NULL;
        GroupInfo[m][groupIdx].tail = NULL;
    }
    else // 該 group 還有其他 USB，不需更新 BathroomInfo[m]
    {
        // GroupInfo
        USB *headUSB = GroupInfo[m][groupIdx].head;
        USB *nextUSB = xorUSB(NULL, headUSB->LRPtr);
        nextUSB->LRPtr = xorUSB(NULL, xorUSB(headUSB, nextUSB->LRPtr));
        GroupInfo[m][groupIdx].head = nextUSB;
    }
    GroupInfo[m][groupIdx].length -= 1; // 該 group 少一個 USB
}

void close(Group **GroupInfo, Bathroom *BathroomInfo, int m)
{
    // 找出離 m 排的最近的隊伍 idx, 將 m 排做為target line 隊伍的 last idx
    int target_m = BathroomInfo[m].targetLine;
    int last_m = BathroomInfo[m].lastLine;
    // 維護 target line, lastLine關係
    BathroomInfo[last_m].targetLine = target_m;
    BathroomInfo[target_m].lastLine = last_m;

    // 從 G_tail 開始遍歷 m 排的 Group Node xor linked list
    GroupNode *G_tail = BathroomInfo[m].G_tail;
    GroupNode *G_old_tail = NULL;
    while (G_tail != NULL)
    {
        int groupIdx = G_tail->number;
        int tar_group_len = GroupInfo[target_m][groupIdx].length;

        if (tar_group_len <= 0) // 表示 m 排有該 group，target_m 排沒有
        {
            // BathroomInfo
            // 將 G_tail 複製一份接到 BathroomInfo[target_m] 的 G_tail上
            GroupNode *source_G_tail = (GroupNode *)malloc(sizeof(GroupNode));
            source_G_tail->number = G_tail->number;

            if (BathroomInfo[target_m].numOfGroup > 0) // target_m 排有其他 group
            {
                GroupNode *target_G_tail = BathroomInfo[target_m].G_tail;
                target_G_tail->LRPtr = xorGroupNode(xorGroupNode(target_G_tail->LRPtr, NULL), source_G_tail); // 舊 tail GroupNode 接上新 tail GroupNode
                source_G_tail->LRPtr = xorGroupNode(target_G_tail, NULL);                                     // 新 tail GroupNode 接上舊 tail GroupNode
                BathroomInfo[target_m].G_tail = source_G_tail;
            }
            else // target_m 排目前為空
            {
                source_G_tail->LRPtr = NULL;
                BathroomInfo[target_m].G_tail = source_G_tail;
                BathroomInfo[target_m].G_head = source_G_tail;
            }
            BathroomInfo[target_m].numOfGroup += 1;

            // GroupInfo
            // 將 GroupInfo[m][groupIdx] 複製給 GroupInfo[target_m][groupIdx] ，但 tail, head 互相交換
            GroupInfo[target_m][groupIdx].length = GroupInfo[m][groupIdx].length;
            GroupInfo[target_m][groupIdx].tail = GroupInfo[m][groupIdx].head;
            GroupInfo[target_m][groupIdx].head = GroupInfo[m][groupIdx].tail;
        }
        else // 表示 m 排, target_m 排都有該 group
        {
            // 不需更新 BathroomInfo
            // GroupInfo
            // 將 GroupInfo[target_m][groupIdx] 的 tail 接到 GroupInfo[m]][groupIdx] 的 tail
            USB *source_USB_tail = GroupInfo[m][groupIdx].tail;
            USB *target_USB_tail = GroupInfo[target_m][groupIdx].tail;
            source_USB_tail->LRPtr = xorUSB(target_USB_tail, xorUSB(NULL, source_USB_tail->LRPtr)); // source tail USB 接上 target tail USB
            target_USB_tail->LRPtr = xorUSB(xorUSB(target_USB_tail->LRPtr, NULL), source_USB_tail); // target tail USB 接上 source tail USB
            GroupInfo[target_m][groupIdx].tail = GroupInfo[m][groupIdx].head; // source USB linked list 的 head 成為 taget USB linked list 的 tail
            GroupInfo[target_m][groupIdx].length += GroupInfo[m][groupIdx].length;
        }

        // 向前走一個 Group Node
        GroupNode *last = xorGroupNode(G_tail->LRPtr, G_old_tail);
        G_old_tail = G_tail;
        G_tail = last;
    }
    BathroomInfo[m].numOfGroup = 0;
}

// 依序 output 某 group 的 USB
void printGroupList(USB *head)
{
    USB *last = NULL;
    // USB 從 head 開始遍歷
    while (head != NULL)
    {
        printf("%d ", head->ID);
        USB *next = xorUSB(last, head->LRPtr);
        last = head;
        head = next;
    }
}

// output M lines
void printLines(Group **GroupInfo, Bathroom *BathroomInfo, int M)
{
    for (int m = 0; m < M; m++)
    {
        if (BathroomInfo[m].numOfGroup == 0) // 此隊伍沒有任何 group
        {
            printf("\n");
        }
        else
        {
            // 第 m 排隊伍間的先後順續由 GroupNode xor linked list來維護
            GroupNode *headGroupNode = BathroomInfo[m].G_head;
            GroupNode *LastGroupNode = NULL;
            while (headGroupNode != NULL)
            {
                int GroupIdx = headGroupNode->number; // 依序取出隊伍中的 group idx
                // printf("Group: %d ", GroupIdx);
                USB *head = GroupInfo[m][GroupIdx].head;
                printGroupList(head);

                // group node 依序遍歷
                GroupNode *next = xorGroupNode(LastGroupNode, headGroupNode->LRPtr);
                LastGroupNode = headGroupNode;
                headGroupNode = next;
            }
            printf("\n"); // m 排印完換行
        }
    }
}

void freeGroupInfo(Group **GroupInfo, int M)
{
    for (int i = 0; i < M; i++)
        free(GroupInfo[i]);
    free(GroupInfo);
}

void freeBathroomInfo(Bathroom *BathroomInfo)
{
    free(BathroomInfo);
}
int main()
{
    // Bathrooms, situation, Groups
    int M, N, K;
    scanf("%d %d %d", &M, &N, &K);

    // GroupInfo [M][K] 2d Array, to record the K group in M line
    Group **GroupInfo = initGroupInfo(M, K);
    // BathroomInfo [M] 1d Array, to record M line
    Bathroom *BathroomInfo = initBathroomInfo(M);

    // Input situations
    char situation[10];
    for (int i = 0; i < N; i++)
    {
        scanf("%s", situation);
        if (strcmp(situation, "enter") == 0)
        {
            int i, j, m;
            scanf("%d %d %d", &i, &j, &m);
            enter(GroupInfo, BathroomInfo, i, j, m);
        }
        else if (strcmp(situation, "leave") == 0)
        {
            int m;
            scanf("%d", &m);
            leave(GroupInfo, BathroomInfo, m);
        }
        else if (strcmp(situation, "go") == 0)
        {
            int m;
            scanf("%d", &m);
            go(GroupInfo, BathroomInfo, m);
        }
        else if (strcmp(situation, "close") == 0)
        {
            int m;
            scanf("%d", &m);
            close(GroupInfo, BathroomInfo, m);
        }
    }

    printLines(GroupInfo, BathroomInfo, M);

    // freeGroupInfo(GroupInfo, M);
    // freeBathroomInfo(BathroomInfo);
}