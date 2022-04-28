#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct board
{
    int row;
    int col;
    int **cells; // 0: empty, 1: black, -1:white
} Board;

typedef struct hint
{
    int hintLen;
    int *hintArray;
} Hint;

void initBoard(Board *gameBoardPtr, int row, int col)
{
    gameBoardPtr->row = row;
    gameBoardPtr->col = col;
    gameBoardPtr->cells = (int **)malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++)
        gameBoardPtr->cells[i] = (int *)calloc(col, sizeof(int));
}

Hint *initHint(int hintLines)
{
    Hint *gameHint = (Hint *)malloc(hintLines * sizeof(Hint)); // gameHint 作為rowHintor colHint的一維陣列，存一排Hint
    int hintLen;
    for (int i = 0; i < hintLines; i++)
    {
        scanf("%d", &hintLen);
        gameHint[i].hintLen = hintLen; // 該行hint的長度(hint array的長度)
        gameHint[i].hintArray = (int *)malloc(hintLen * sizeof(int));

        for (int j = 0; j < hintLen; j++)
        {
            scanf("%d", &gameHint[i].hintArray[j]);
        }
    }
    return gameHint;
}

void freeMemory(Board *gameBoardPtr, Hint *rowHint, Hint *colHint)
{
    // board
    for (int i = 0; i < gameBoardPtr->row; i++)
    {
        free(gameBoardPtr->cells[i]);
    }
    free(gameBoardPtr->cells);

    // Hint
    for (int i = 0; i < gameBoardPtr->row; i++)
    {
        free(rowHint[i].hintArray);
    }
    free(rowHint);
    for (int i = 0; i < gameBoardPtr->col; i++)
    {
        free(colHint[i].hintArray);
    }
    free(colHint);
}

void printBoard(Board *gameBoardPtr)
{
    for (int i = 0; i < gameBoardPtr->row; i++)
    {
        for (int j = 0; j < gameBoardPtr->col; j++)
        {
            printf("%c", (gameBoardPtr->cells[i][j] == 1 ? 'o' : '_'));
        }
        printf("\n");
    }
}

void printHint(Hint *gameHint, int hintLines)
{
    for (int i = 0; i < hintLines; i++)
    {
        for (int j = 0; j < gameHint[i].hintLen; j++)
        {
            printf("%d ", gameHint[i].hintArray[j]);
        }
        printf("\n");
    }
}

bool isBoardEmpty(Board *gameBoardPtr)
{
    int row = gameBoardPtr->row;
    int col = gameBoardPtr->col;
    if (gameBoardPtr->cells[row - 1][col - 1] == 0)
        return true;
    else
        return false;
}

// 從找到的第一個黑色開始，累計連續黑色格子，在已經有累計黑色的情況下碰到白色或是已經到最末端，就終止累計，並且檢查
// 依序搜尋每一row，每一col
bool isBoardValid(Board *gameBoardPtr, Hint *rowHint, Hint *colHint)
{
    int checkIdx = 0, numOfBlack = 0;
    for (int i = 0; i < gameBoardPtr->row; i++)
    {
        checkIdx = 0;
        numOfBlack = 0;
        for (int j = 0; j < gameBoardPtr->col; j++)
        {
            if (gameBoardPtr->cells[i][j] == 1)
                numOfBlack += 1;
            if (numOfBlack != 0 &&
                (gameBoardPtr->cells[i][j] == -1 || j == gameBoardPtr->col - 1))
            {
                if (numOfBlack != rowHint[i].hintArray[checkIdx])
                    return false;
                numOfBlack = 0;
                checkIdx++;
            }
        }
        if (checkIdx != rowHint[i].hintLen)
            return false;
    }
    for (int j = 0; j < gameBoardPtr->col; j++)
    {
        checkIdx = 0;
        numOfBlack = 0;
        for (int i = 0; i < gameBoardPtr->row; i++)
        {
            if (gameBoardPtr->cells[i][j] == 1)
                numOfBlack++;
            if (numOfBlack != 0 &&
                (gameBoardPtr->cells[i][j] == -1 || i == gameBoardPtr->row - 1))
            {
                if (numOfBlack != colHint[j].hintArray[checkIdx])
                    return false;
                numOfBlack = 0;
                checkIdx++;
            }
        }
        if (checkIdx != colHint[j].hintLen)
            return false;
    }
    return true;
}

// 回傳empty cell的位址
int *findEmpty(Board *gameBoardPtr)
{
    for (int i = 0; i < gameBoardPtr->row; i++)
    {
        for (int j = 0; j < gameBoardPtr->col; j++)
        {
            if (gameBoardPtr->cells[i][j] == 0)
                return &(gameBoardPtr->cells[i][j]);
        }
    }
    return NULL;
}

void paintBlack(int *cell)
{
    *cell = 1; // 直接透過pointer修改數值
}

void paintWhite(int *cell)
{
    *cell = -1; // 直接透過pointer修改數值
}

bool Recur(Board *gameBoardPtr, Hint *rowHint, Hint *colHint)
{
    if (!isBoardEmpty(gameBoardPtr))
    {
        if (isBoardValid(gameBoardPtr, rowHint, colHint))
        {
            printBoard(gameBoardPtr);
            // test:
            // printf("true\n");
            return true;
        }
        else
        {
            // test:
            // printBoard(gameBoardPtr);
            // printf("false\n");
            return false;
        }
    }
    else
    {
        bool succeed;
        // find empty cell
        int *cell = findEmpty(gameBoardPtr);

        // paint cell black
        paintBlack(cell);
        succeed = Recur(gameBoardPtr, rowHint, colHint);
        if (succeed)
            return true;

        // paint cell white
        paintWhite(cell);
        succeed = Recur(gameBoardPtr, rowHint, colHint);
        if (succeed)
            return true;

        // make cell empty;
        *cell = 0;
        return false;
    }
}

int main()
{
    int row, col;
    scanf("%d %d", &row, &col);

    Board gameBoard;
    Board *gameBoardPtr = &gameBoard;
    initBoard(gameBoardPtr, row, col);

    Hint *rowHint = initHint(gameBoardPtr->row); // dynamic array, hintLines = row
    Hint *colHint = initHint(gameBoardPtr->col); // dynamic array, hintLines = col

    // test: init Board
    // printf("%d %d\n", gameBoard.row, gameBoard.col);
    // printBoard(gameBoardPtr);

    // test: init Hint
    // printf("\n");
    // printHint(rowHint, gameBoardPtr->row);
    // printf("\n");
    // printHint(colHint, gameBoardPtr->col);

    bool succeed = Recur(gameBoardPtr, rowHint, colHint);

    // free Memory
    freeMemory(gameBoardPtr, rowHint, colHint);
}
