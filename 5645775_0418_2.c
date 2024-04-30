#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_EXPRESSION_LENGTH 100

typedef int element;
typedef struct StackType {
    element* data;
    int capacity;
    int top;
} StackType;

void init_stack(StackType* s, int capacity) {
    s->data = (element*)malloc(sizeof(element) * capacity);
    s->capacity = capacity;
    s->top = -1;
}

void push(StackType* s, element item) {
    s->data[++(s->top)] = item;
}

element pop(StackType* s) {
    return s->data[(s->top)--];
}

element peek(StackType* s) {
    return s->data[s->top];
}

int is_empty(StackType* s) {
    return (s->top == -1);
}

int precedence(char operator) {
    if (operator == '+' || operator == '-')
        return 1;
    else if (operator == '*' || operator == '/')
        return 2;
    else
        return 0;
}

void infix_to_postfix(char* infixExpression, char* postfixExpression) {
    StackType stack;
    init_stack(&stack, MAX_EXPRESSION_LENGTH);
    int i = 0, j = 0;

    while (infixExpression[i] != '\0') {
        char current_char = infixExpression[i];
        if (isdigit(current_char)) {
            postfixExpression[j++] = current_char;
        }
        else if (current_char == '(') {
            push(&stack, current_char);
        }
        else if (current_char == ')') {
            while (!is_empty(&stack) && peek(&stack) != '(') {
                postfixExpression[j++] = pop(&stack);
            }
            pop(&stack); 
        }
        else if (current_char == '+' || current_char == '-' || current_char == '*' || current_char == '/') {
            while (!is_empty(&stack) && precedence(peek(&stack)) >= precedence(current_char)) {
                postfixExpression[j++] = pop(&stack);
            }
            push(&stack, current_char);
        }
        i++;
    }
    while (!is_empty(&stack)) {
        postfixExpression[j++] = pop(&stack);
    }
    postfixExpression[j] = '\0'; 
}

int evaluate_postfix(char* expression) {
    StackType stack;
    init_stack(&stack, MAX_EXPRESSION_LENGTH);
    int i = 0;

    while (expression[i] != '\0') {
        char current_char = expression[i];
        if (isdigit(current_char)) {
            push(&stack, current_char - '0');
        }
        else {
            int operand2 = pop(&stack);
            int operand1 = pop(&stack);
            switch (current_char) {
            case '+':
                push(&stack, operand1 + operand2);
                break;
            case '-':
                push(&stack, operand1 - operand2);
                break;
            case '*':
                push(&stack, operand1 * operand2);
                break;
            case '/':
                push(&stack, operand1 / operand2);
                break;
            }
        }
        i++;
    }
    return pop(&stack);
}

int main() {
    char infixExpression[MAX_EXPRESSION_LENGTH];
    char postfixExpression[MAX_EXPRESSION_LENGTH];
    int choice;

    while (1) {
        printf("\n다음과 같은 메뉴로 동작하는 프로그램입니다:\n");
        printf("1. 중위식을 입력 받음\n");
        printf("2. 중위식을 후위식으로 변환\n");
        printf("3. 후위식을 계산\n");
        printf("4. 종료\n");
        printf("메뉴를 입력하세요: ");
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("중위식 입력하세요: ");
            getchar(); 
            fgets(infixExpression, sizeof(infixExpression), stdin);
            printf("입력된 중위식: %s", infixExpression);
            break;

        case 2:
            infix_to_postfix(infixExpression, postfixExpression);
            printf("후위식: %s\n", postfixExpression);
            break;

        case 3: {
            int result = evaluate_postfix(postfixExpression);
            printf("결과: %d\n", result);
            break;
        }

        case 4:
            printf("프로그램을 종료합니다.\n");
            exit(0);

        default:
            printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
            break;
        }
       
        while (getchar() != '\n');
    }

    return 0;
}