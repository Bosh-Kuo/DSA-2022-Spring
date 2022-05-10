#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LEN 33

typedef struct disjointSet
{
    // TODO: Determine fields to use by your method
    char name[13];
    int rootHash;
    // int rank;
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
    strcpy(ds[myHash].name, s);
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

int find_set(const char *s)
{
    init(s);
    int i = hash(s);
    // TODO: Implement your find algorithm here
    if (ds[i].rootHash != i)
    {
        int rootHash = ds[i].rootHash;
        ds[i].rootHash = find_set(ds[rootHash].name);
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
    int j = 0, cnt = 0;
    for (int i = 0; i <= (strlen(input)); i++)
    {
        // if space or NULL found, assign NULL into splitStrings[cnt]
        if (input[i] == ' ' || input[i] == '\0' )
        {
            splitStrings[cnt][j] = '\0';
            cnt++; // for next word
            j = 0; // for next word, init index to 0
        }
        else
        {
            splitStrings[cnt][j] = input[i];
            j++;
        }
    }
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

        // int first_root = ds[hash(splitStrings[1])].rootHash;
        // int second_root = ds[hash(splitStrings[2])].rootHash;
        // printf("1: %d, 2: %d\n", first_root, second_root);

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