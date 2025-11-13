/**
 * @file parser.c
 * @brief Implementation of expression parsing with Shunting Yard algorithm
 */

#include "expression/parser.h"
#include "conversion/converter.h"
#include "app/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Helper: Operator stack for Shunting Yard */
typedef struct {
    char* operators;
    int top;
    int capacity;
} OperatorStack;

/* Helper: Create operator stack */
static OperatorStack* createOperatorStack(int capacity) {
    OperatorStack* stack = (OperatorStack*)malloc(sizeof(OperatorStack));
    if (stack == NULL) return NULL;

    stack->operators = (char*)malloc(capacity);
    if (stack->operators == NULL) {
        free(stack);
        return NULL;
    }

    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

/* Helper: Destroy operator stack */
static void destroyOperatorStack(OperatorStack* stack) {
    if (stack != NULL) {
        free(stack->operators);
        free(stack);
    }
}

/* Helper: Push to operator stack */
static bool pushOperator(OperatorStack* stack, char op) {
    if (stack->top >= stack->capacity - 1) {
        char* newOps = (char*)realloc(stack->operators, stack->capacity * 2);
        if (newOps == NULL) return false;
        stack->operators = newOps;
        stack->capacity *= 2;
    }
    stack->operators[++stack->top] = op;
    return true;
}

/* Helper: Pop from operator stack */
static char popOperator(OperatorStack* stack) {
    if (stack->top < 0) return '\0';
    return stack->operators[stack->top--];
}

/* Helper: Peek at top of operator stack */
static char peekOperator(OperatorStack* stack) {
    if (stack->top < 0) return '\0';
    return stack->operators[stack->top];
}

/* Helper: Check if operator stack is empty */
static bool isOperatorStackEmpty(OperatorStack* stack) {
    return stack->top < 0;
}

/**
 * @brief Gets operator precedence
 * Precedence levels: ! > ^  > * / % > + -
 */
int getOperatorPrecedence(char op) {
    switch (op) {
        case '!': return 4;  /* Highest (postfix) */
        case '^': return 3;  /* Power */
        case '*':
        case '/':
        case '%': return 2;  /* Multiplicative */
        case '+':
        case '-': return 1;  /* Additive (lowest) */
        default: return -1;
    }
}

/**
 * @brief Checks if operator is right-associative
 */
bool isRightAssociative(char op) {
    return op == '^';  /* Only power is right-associative */
}

/* Helper: Check if character is an operator */
static bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '!';
}

/**
 * @brief Validates an arithmetic expression
 */
bool validateExpression(const char* expr) {
    int parenCount = 0;
    const char* p = expr;
    bool expectOperand = true;  /* true if expecting number/operand */

    if (expr == NULL || *expr == '\0') return false;

    while (*p != '\0') {
        /* Skip whitespace */
        if (isspace(*p)) {
            p++;
            continue;
        }

        /* Left parenthesis */
        if (*p == '(') {
            if (!expectOperand) return false;
            parenCount++;
            p++;
            continue;
        }

        /* Right parenthesis */
        if (*p == ')') {
            if (expectOperand || parenCount <= 0) return false;
            parenCount--;
            p++;
            continue;
        }

        /* Factorial operator (postfix) */
        if (*p == '!') {
            if (expectOperand) return false;  /* Can't have ! after operator */
            p++;
            /* After factorial, still not expecting operand (can chain) */
            continue;
        }

        /* Unary minus */
        if (*p == '-' && expectOperand) {
            p++;
            /* Still expecting operand after unary minus */
            continue;
        }

        /* Binary operators */
        if (isOperator(*p)) {
            if (expectOperand) return false;  /* Two operators in a row */
            expectOperand = true;
            p++;
            continue;
        }

        /* Number (decimal, binary, hex) */
        if (isDigitChar(*p) ||
            (*p == '0' && (*(p+1) == 'b' || *(p+1) == 'B' || *(p+1) == 'x' || *(p+1) == 'X'))) {
            if (!expectOperand) return false;

            /* Skip the entire number */
            if (*p == '0' && (*(p+1) == 'b' || *(p+1) == 'B')) {
                p += 2;
                while (*p == '0' || *p == '1') p++;
            } else if (*p == '0' && (*(p+1) == 'x' || *(p+1) == 'X')) {
                p += 2;
                while (isHexDigitChar(*p)) p++;
            } else {
                while (isDigitChar(*p)) p++;
            }

            expectOperand = false;
            continue;
        }

        /* Invalid character */
        return false;
    }

    /* Check final state */
    if (parenCount != 0) return false;       /* Unbalanced parentheses */
    if (expectOperand) return false;         /* Expression ends with operator */

    return true;
}

/* Helper: Tokenize expression into array */
static Token* tokenizeExpression(const char* expr, int* tokenCount) {
    Token* tokens;
    int capacity;
    int count;
    const char* p;
    bool expectOperand;

    capacity = 64;
    tokens = (Token*)malloc(capacity * sizeof(Token));
    if (tokens == NULL) return NULL;

    count = 0;
    p = expr;
    expectOperand = true;

    while (*p != '\0') {
        /* Skip whitespace */
        if (isspace(*p)) {
            p++;
            continue;
        }

        /* Grow array if needed */
        if (count >= capacity - 1) {
            Token* newTokens = (Token*)realloc(tokens, capacity * 2 * sizeof(Token));
            if (newTokens == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            tokens = newTokens;
            capacity *= 2;
        }

        /* Left parenthesis */
        if (*p == '(') {
            tokens[count].type = TOKEN_LPAREN;
            tokens[count].value = (char*)malloc(2);
            if (tokens[count].value == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            tokens[count].value[0] = '(';
            tokens[count].value[1] = '\0';
            count++;
            p++;
            expectOperand = true;
            continue;
        }

        /* Right parenthesis */
        if (*p == ')') {
            tokens[count].type = TOKEN_RPAREN;
            tokens[count].value = (char*)malloc(2);
            if (tokens[count].value == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            tokens[count].value[0] = ')';
            tokens[count].value[1] = '\0';
            count++;
            p++;
            expectOperand = false;
            continue;
        }

        /* Factorial (postfix) */
        if (*p == '!') {
            tokens[count].type = TOKEN_FACTORIAL;
            tokens[count].value = (char*)malloc(2);
            if (tokens[count].value == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            tokens[count].value[0] = '!';
            tokens[count].value[1] = '\0';
            count++;
            p++;
            /* After factorial, still not expecting operand */
            continue;
        }

        /* Unary minus */
        if (*p == '-' && expectOperand) {
            tokens[count].type = TOKEN_UNARY_MINUS;
            tokens[count].value = (char*)malloc(2);
            if (tokens[count].value == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            tokens[count].value[0] = '-';
            tokens[count].value[1] = '\0';
            count++;
            p++;
            /* Still expecting operand */
            continue;
        }

        /* Binary operators */
        if (isOperator(*p)) {
            tokens[count].type = TOKEN_OPERATOR;
            tokens[count].value = (char*)malloc(2);
            if (tokens[count].value == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            tokens[count].value[0] = *p;
            tokens[count].value[1] = '\0';
            count++;
            p++;
            expectOperand = true;
            continue;
        }

        /* Number */
        if (isDigitChar(*p) ||
            (*p == '0' && (*(p+1) == 'b' || *(p+1) == 'B' || *(p+1) == 'x' || *(p+1) == 'X'))) {
            const char* start = p;
            size_t len;

            /* Scan the entire number */
            if (*p == '0' && (*(p+1) == 'b' || *(p+1) == 'B')) {
                p += 2;
                while (*p == '0' || *p == '1') p++;
            } else if (*p == '0' && (*(p+1) == 'x' || *(p+1) == 'X')) {
                p += 2;
                while (isHexDigitChar(*p)) p++;
            } else {
                while (isDigitChar(*p)) p++;
            }

            len = (size_t)(p - start);
            tokens[count].type = TOKEN_NUMBER;
            tokens[count].value = (char*)malloc(len + 1);
            if (tokens[count].value == NULL) {
                int i;
                for (i = 0; i < count; i++) {
                    free(tokens[i].value);
                }
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count].value, start, len);
            tokens[count].value[len] = '\0';
            count++;
            expectOperand = false;
            continue;
        }

        /* Unknown character - should not happen if validated */
        p++;
    }

    *tokenCount = count;
    return tokens;
}

/**
 * @brief Converts infix expression to postfix (Shunting Yard algorithm)
 */
Token* infixToPostfix(const char* expr) {
    Token* infix;
    Token* postfix;
    OperatorStack* opStack;
    int infixCount;
    int postfixCount;
    int i;

    if (expr == NULL) return NULL;

    /* Validate first */
    if (!validateExpression(expr)) return NULL;

    /* Tokenize */
    infix = tokenizeExpression(expr, &infixCount);
    if (infix == NULL) return NULL;

    /* Allocate postfix array (worst case: same size + 1 for NULL terminator) */
    postfix = (Token*)malloc((infixCount + 1) * sizeof(Token));
    if (postfix == NULL) {
        for (i = 0; i < infixCount; i++) {
            free(infix[i].value);
        }
        free(infix);
        return NULL;
    }

    /* Create operator stack */
    opStack = createOperatorStack(infixCount);
    if (opStack == NULL) {
        for (i = 0; i < infixCount; i++) {
            free(infix[i].value);
        }
        free(infix);
        free(postfix);
        return NULL;
    }

    postfixCount = 0;

    /* Shunting Yard algorithm */
    for (i = 0; i < infixCount; i++) {
        Token current = infix[i];

        /* Number: add to output */
        if (current.type == TOKEN_NUMBER) {
            postfix[postfixCount] = current;
            postfixCount++;
        }
        /* Left parenthesis: push to stack */
        else if (current.type == TOKEN_LPAREN) {
            pushOperator(opStack, '(');
            free(current.value);
        }
        /* Right parenthesis: pop until matching left paren */
        else if (current.type == TOKEN_RPAREN) {
            while (!isOperatorStackEmpty(opStack) && peekOperator(opStack) != '(') {
                char op = popOperator(opStack);
                postfix[postfixCount].type = TOKEN_OPERATOR;
                postfix[postfixCount].value = (char*)malloc(2);
                postfix[postfixCount].value[0] = op;
                postfix[postfixCount].value[1] = '\0';
                postfixCount++;
            }
            /* Pop the left parenthesis */
            if (!isOperatorStackEmpty(opStack)) {
                popOperator(opStack);
            }
            free(current.value);
        }
        /* Unary minus: treat as operator with high precedence */
        else if (current.type == TOKEN_UNARY_MINUS) {
            pushOperator(opStack, '~');  /* Use ~ to represent unary minus */
            free(current.value);
        }
        /* Factorial: immediately add to output (postfix operator) */
        else if (current.type == TOKEN_FACTORIAL) {
            postfix[postfixCount] = current;
            postfixCount++;
        }
        /* Binary operator */
        else if (current.type == TOKEN_OPERATOR) {
            char op = current.value[0];
            int prec = getOperatorPrecedence(op);

            /* Pop operators with higher or equal precedence (respect associativity) */
            while (!isOperatorStackEmpty(opStack)) {
                char topOp = peekOperator(opStack);
                int topPrec;

                if (topOp == '(' || topOp == '~') break;

                topPrec = getOperatorPrecedence(topOp);

                /* Right-associative: pop only if strictly greater precedence */
                if (isRightAssociative(op)) {
                    if (topPrec <= prec) break;
                }
                /* Left-associative: pop if greater or equal precedence */
                else {
                    if (topPrec < prec) break;
                }

                topOp = popOperator(opStack);
                postfix[postfixCount].type = TOKEN_OPERATOR;
                postfix[postfixCount].value = (char*)malloc(2);
                postfix[postfixCount].value[0] = topOp;
                postfix[postfixCount].value[1] = '\0';
                postfixCount++;
            }

            pushOperator(opStack, op);
            free(current.value);
        }
    }

    /* Pop remaining operators */
    while (!isOperatorStackEmpty(opStack)) {
        char op = popOperator(opStack);
        if (op != '(') {  /* Ignore any stray parentheses */
            postfix[postfixCount].type = (op == '~') ? TOKEN_UNARY_MINUS : TOKEN_OPERATOR;
            postfix[postfixCount].value = (char*)malloc(2);
            postfix[postfixCount].value[0] = op;
            postfix[postfixCount].value[1] = '\0';
            postfixCount++;
        }
    }

    /* NULL-terminate the array */
    postfix[postfixCount].type = TOKEN_END;
    postfix[postfixCount].value = NULL;

    /* Cleanup */
    destroyOperatorStack(opStack);
    free(infix);

    return postfix;
}

/**
 * @brief Frees memory allocated for token array
 */
void freeTokens(Token* tokens) {
    int i;

    if (tokens == NULL) return;

    for (i = 0; tokens[i].type != TOKEN_END; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}
