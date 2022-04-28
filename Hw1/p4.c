#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define MAX_LEN 1000001

// For postfix linked list
typedef struct token
{
    long long int val;
    char oprator;
    bool isNumber;
    struct token *next;
} ListNode;

ListNode *createListNode()
{
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    newNode->next = NULL;
    return newNode;
}

// For infix to postfix process
typedef struct Stack_Op
{
    char oprators[MAX_LEN];
    int length;
} Stack_Op;

Stack_Op stack_op;

// For postfix evaluation
typedef struct Stack_Num
{
    long long int numbers[MAX_LEN];
    int length;
} Stack_Num;

Stack_Num stack_num;

void init_Stack_Op(Stack_Op *s)
{
    s->length = 0;
}
void init_Stack_Num(Stack_Num *s)
{
    s->length = 0;
}
bool is_empty_Stack_Op(Stack_Op *s)
{
    return (s->length == 0);
}
void push_Stack_Op(Stack_Op *s, char oprator)
{
    s->length += 1;
    s->oprators[s->length - 1] = oprator;
}
void push_Stack_Num(Stack_Num *s, long long int number)
{
    s->length += 1;
    s->numbers[s->length - 1] = number;
}
char pop_Stack_Op(Stack_Op *s)
{
    s->length -= 1;
    return s->oprators[s->length];
}
long long int pop_Stack_Num(Stack_Num *s)
{
    s->length -= 1;
    return s->numbers[s->length];
}

//依序印出 prefix linked list node
void print_linked_list(ListNode *head)
{
    for (; head != NULL; head = head->next)
    {
        if (head->isNumber)
            printf("%lld ", head->val);
        else
            printf("%c ", head->oprator);
    }
    printf("\n");
}

//比較 operator 位階，若 stack peep 的 operator 位階 >= input 的 operator 回傳 true
bool compareOperator(char stackOP, char inputOP)
{
    // () > */% > +-=
    int stackOPScore = (stackOP == '(') ? 2 : ((stackOP == '*' || stackOP == '/' || stackOP == '%') ? 1 : 0);
    int inputOPScore = (inputOP == '(') ? 2 : ((inputOP == '*' || inputOP == '/' || inputOP == '%') ? 1 : 0);
    return stackOPScore >= inputOPScore;
}

ListNode *InfixToPostfix(ListNode *head, char input[])
{
    ListNode *current = head;
    // Stack_Op stack_op;
    init_Stack_Op(&stack_op);

    int input_length = strlen(input);
    int number = 0;
    int numberDigit = 0;
    char number_str[2] = {'\0', '\0'};

    // 遍歷input每個 char
    for (int i = 0; i < input_length; i++)
    {
        if (isdigit(input[i])) // 不考慮 input[i] 為負數
        {
            number_str[0] = input[i];
            if (numberDigit > 0)
                number = 10 * number + atoi(number_str);
            else
                number = atoi(number_str);
            numberDigit += 1;
        }
        else if (ispunct(input[i])) // 遇到 + - * / % ( ) =
        {
            // 遇到 operator 先將 number 加到 linked list
            if (numberDigit > 0)
            {
                ListNode *newNode_number = createListNode();
                newNode_number->isNumber = true;
                newNode_number->val = (long long int)number;
                current->next = newNode_number; // 將當前的 linked list 接上 newNode_number
                current = current->next;        // 將 current 移到 linked list tail
                numberDigit = 0;
            }
            if (input[i] == ')')
            {
                char topOperator = pop_Stack_Op(&stack_op);
                while (topOperator != '(') // 遇到 ) => pop 並加到 linked list 直到遇到 ( 才停止
                {
                    ListNode *newNode_char = createListNode();
                    newNode_char->isNumber = false;
                    newNode_char->oprator = topOperator;
                    current->next = newNode_char; // 將當前的 linked list 接上 newNode_number
                    current = current->next;      // 將 current 移到 linked list tail
                    topOperator = pop_Stack_Op(&stack_op);
                }
            }
            else // operator 為 + - * / % ( =
            {
                // 決定 stack 中的 operator token 是否該加入 linked list
                while (compareOperator(stack_op.oprators[stack_op.length - 1], input[i]) && stack_op.oprators[stack_op.length - 1] != '(' && !is_empty_Stack_Op(&stack_op))
                {
                    ListNode *newNode_char = createListNode();
                    newNode_char->isNumber = false;
                    newNode_char->oprator = pop_Stack_Op(&stack_op);
                    current->next = newNode_char; // 將當前的 linked list 接上 newNode_number
                    current = current->next;      // 將 current 移到 linked list tail
                }

                // push 進 stack
                push_Stack_Op(&stack_op, input[i]);

                // operator 為 = 也要加到 linked list
                if (input[i] == '=')
                {
                    ListNode *newNode_char = createListNode();
                    newNode_char->isNumber = false;
                    newNode_char->oprator = pop_Stack_Op(&stack_op);
                    current->next = newNode_char; // 將當前的 linked list 接上 newNode_number
                    current = current->next;      // 將 current 移到 linked list tail
                }
            }
        }
    }
    return head;
}

long long int operation(long long int left, long long int right, char operator)
{
    long long int ans;
    if (operator== '+')
        ans = left + right;
    else if (operator== '-')
        ans = left - right;
    else if (operator== '*')
        ans = left * right;
    else if (operator== '/')
    {
        if (llabs(left) % llabs(right) != 0 && ((left < 0 && right > 0) || (left > 0 && right < 0)))
            ans = left / right - 1;
        else
            ans = left / right;
    }
    else
        ans = left % right;
    // printf("%lld %c %lld = %lld\n", left, operator, right, ans);
    return ans;
}

void PostfixEvaluation(ListNode *head)
{
    ListNode *current = head;
    init_Stack_Num(&stack_num);
    while (current != NULL)
    {
        if (current->isNumber)
        {
            push_Stack_Num(&stack_num, current->val);
        }
        else
        {
            if (current->oprator == '=')
            {
                printf("Print: %lld\n", stack_num.numbers[stack_num.length - 1]);
            }
            else
            {
                long long int right = pop_Stack_Num(&stack_num);
                long long int left = pop_Stack_Num(&stack_num);
                long long int ans = operation(left, right, current->oprator);
                // printf("%lld\n", ans);
                push_Stack_Num(&stack_num, ans);
            }
        }
        current = current->next;
    }
}

void freeList(ListNode *node)
{
    while (node != NULL)
    {
        ListNode *nextNode = node->next;
        free(node);
        node = nextNode;
    }
}

int main()
{
    // 輸入
    char input[MAX_LEN];
    scanf("%s", input);

    // 將輸入轉postfix
    ListNode *head;
    head = createListNode(); // dummy head
    head = InfixToPostfix(head, input);
    print_linked_list(head->next); // check postfix

    // Postfix Evaluation
    PostfixEvaluation(head->next);

    // free Memory
    freeList(head);
}
