#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LEN 33

typedef struct disjointSet
{
    // TODO: Determine fields to use by your method
    int rootHash;
    int rank;
} DisjointSet;

DisjointSet ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'z')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z')
        return c - 'A' + 36;
    return -1;
}

int hash(const char *s)
{
    int ret = 0;
    int mask = (1 << 24) - 1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char *s)
{
    // TODO: Initialize a set with hash value
    int myHash = hash(s);
    ds[myHash].rootHash = myHash;
}

inline void static init(const char *s)
{
    int i = hash(s);
    if (!set[i])
    {
        makeset(s);
        set[i] = 1;
    }
}

int path_compression(int myHash)
{
    if (ds[myHash].rootHash != myHash)
    {
        ds[myHash].rootHash = path_compression(ds[myHash].rootHash);
    }
    return ds[myHash].rootHash;
}

int find_set(const char *s)
{
    init(s);
    int i = hash(s);
    // TODO: Implement your find algorithm here
    if (ds[i].rootHash != i)
    {
        ds[i].rootHash = path_compression(ds[i].rootHash);
    }
    return ds[i].rootHash; /* something */
}

void group(const char *ra, const char *rb)
{
    int a = find_set(ra), b = find_set(rb);
    // TODO: Implement your union algorithm here
    // add a to b
    ds[a].rootHash = b;
}

bool same_set(const char *a, const char *b)
{
    // TODO: Implement your algorithm here
    int root_a = find_set(a), root_b = find_set(b);
    return root_a == root_b ? true : false;
}

void split_string(char **splitStrings, char *input)
{
    splitStrings[0] = strtok(input, " ");
    splitStrings[1] = strtok(NULL, " ");
    splitStrings[2] = strtok(NULL, " ");
}
int main()
{
    // TODO: Implement your algorithm here
    int N ;
    char **splitStrings = (char**)malloc(3*sizeof(char*));
    for (int i = 0; i<3;i++)
        splitStrings[i] = (char*)malloc(13*sizeof(char));
    
    char input[MAX_LEN];
    scanf("%d ", &N);


    for (int i = 0; i < N; i++)
    {
        // fgets(input, MAX_LEN, stdin);
        scanf ("%[^\n]%*c", input);
        split_string(splitStrings, input);
        bool same = same_set(splitStrings[1], splitStrings[2]);
        if (splitStrings[0][0] == 'g')
        {    
            if (!same)
                group(splitStrings[1], splitStrings[2]);
        }
        else
        {
            if (same)
                printf("Positive\n");
            else
                printf("Negative\n");
        }
        
    }
}