// ref: https://www.programiz.com/dsa/graph-adjacency-list
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct CityNode
{
    int city;
    struct CityNode *next;
} CityNode;

typedef struct Graph
{
    int numOfCity;
    CityNode **adjList;
} Graph;

typedef struct PairNode
{
    int currentCity;
    int parentCity;
} PairNode;

typedef struct Queue
{
    PairNode *queue;
    int head;
    int tail;
    int length;
} Queue;

CityNode *createCityNode(int city)
{
    CityNode *newNode = (CityNode *)malloc(sizeof(CityNode));
    newNode->next = NULL;
    newNode->city = city;
    return newNode;
}

Graph *createGraph(int numOfCity)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numOfCity = numOfCity;
    graph->adjList = (CityNode **)malloc(numOfCity * sizeof(CityNode *));
    for (int i = 0; i < numOfCity; i++)
    {
        graph->adjList[i] = NULL;
    }
    return graph;
}

void addEdge(Graph *graph, int city1, int city2)
{
    // add City1Node to list
    CityNode *City1Node = createCityNode(city1);
    City1Node->next = graph->adjList[city2 - 1];
    graph->adjList[city2 - 1] = City1Node;

    // add City2Node to list
    CityNode *City2Node = createCityNode(city2);
    City2Node->next = graph->adjList[city1 - 1];
    graph->adjList[city1 - 1] = City2Node;
}

Queue *createQueue(int size)
{
    Queue *newQueue = (Queue *)malloc(sizeof(Queue));
    newQueue->queue = (PairNode *)malloc(size * sizeof(PairNode));
    newQueue->length = 0;
    newQueue->head = 0;
    newQueue->tail = 0;
    return newQueue;
}

void initQueue(Queue *queue)
{
    queue->length = 0;
    queue->head = 0;
    queue->tail = 0;
}

void enqueue(Queue *CityQueue, int currentCity, int parentCity)
{
    CityQueue->queue[CityQueue->tail].parentCity = parentCity;
    CityQueue->queue[CityQueue->tail].currentCity = currentCity;
    CityQueue->tail += 1;
    CityQueue->length += 1;
}

PairNode dequeue(Queue *CityQueue)
{
    PairNode cityPair = CityQueue->queue[CityQueue->head];
    CityQueue->head += 1;
    CityQueue->length -= 1;
    return cityPair;
}

// Print the adj list for test
void printGraph(struct Graph *graph)
{
    int v;
    for (v = 0; v < graph->numOfCity; v++)
    {
        CityNode *temp = graph->adjList[v];
        printf("\n City %d\n: ", v + 1);
        while (temp)
        {
            printf("%d -> ", temp->city);
            temp = temp->next;
        }
        printf("\n");
    }
}

// ??? S ?????? BFS adj-list
void BFS(Graph *graph, int *result, int *levelSum, Queue *cityQueue, int *critical_ptr, const int S, int R)
{

    int level = 0;
    enqueue(cityQueue, S, 0); // S ?????? root

    while (cityQueue->length != 0)
    {
        level += 1;
        int numOfNodes = cityQueue->length; // ?????? level ???????????? nodes ???
        for (int i = 0; i < numOfNodes; i++)
        {
            PairNode cityPair = dequeue(cityQueue); // ??????????????? city ????????? parent
            int currentCity = cityPair.currentCity;
            int parentCity = cityPair.parentCity;

            // critical_level_sum?????????????????? critical_level_sum
            if (*critical_ptr < 0 && currentCity == R)
            {
                *critical_ptr = 1 + level;
                result[currentCity] = currentCity; // ??????????????????
            }

            // ?????? currentCity ??? level
            levelSum[currentCity] += level;

            // ?????? critical_level_sum ???????????????
            if (*critical_ptr > 0)
            {
                // ?????? level >= critical_level_sum result ?????? parentCity ??? result
                if (level >= *critical_ptr || levelSum[currentCity] > *critical_ptr)
                    result[currentCity] = result[parentCity];
                else
                {
                    // ????????? BFS ???????????? S-R???????????? city result
                    if (levelSum[currentCity] - level != 0)
                        result[currentCity] = currentCity;
                }
            }

            // enqueue ????????? level ???????????? citys
            CityNode *node = graph->adjList[currentCity - 1];
            while (node != NULL)
            {
                // ?????? node ???????????? currrentCity ????????? node ????????????????????????????????????
                if (node->city != parentCity && result[node->city] == 0)
                    enqueue(cityQueue, node->city, currentCity);
                node = node->next;
            }
        }
    }
}

int main()
{
    int N, Q, S, R; // cities, queries, capital city, resort city
    scanf("%d %d %d %d", &N, &Q, &S, &R);

    // build adj-list
    Graph *graph = createGraph(N);
    for (int i = 0; i < N - 1; i++)
    {
        int city1, city2;
        scanf("%d %d", &city1, &city2);
        addEdge(graph, city1, city2);
    }
    // printGraph(graph);

    // result[N+1]: ?????? city ??? query ?????????????????? idx 0, ????????? city ?????? idx ??????
    int *result = (int *)calloc(N + 1, sizeof(int));

    // levelSum[N+1]: ?????? city ??? level sum ????????? idx 0, ????????? city ?????? idx
    int *levelSum = (int *)calloc(N + 1, sizeof(int));

    // cityQueue->queue[N]: BFS ????????????????????? level ??? city
    Queue *cityQueue = createQueue(N);

    int critical_level_sum = -1;
    int *critical_ptr = &critical_level_sum;

    // from S to R
    BFS(graph, result, levelSum, cityQueue, critical_ptr, S, R);

    // ??????queue
    initQueue(cityQueue);

    // from R to S
    BFS(graph, result, levelSum, cityQueue, critical_ptr, R, S);

    // read query and answer
    for (int i = 0; i < Q; i++)
    {
        int query;
        scanf("%d", &query);
        printf("%d\n", result[query]);
    }
}