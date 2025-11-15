/**
 * @file evaluator.c
 * @brief Implementation of expression evaluation
 */

#include "expression/evaluator.h"
#include "core/bignum_ops.h"
#include "core/bignum_math.h"
#include "conversion/converter.h"
#include <stdlib.h>
#include <string.h>

/* Helper: BigNum stack for evaluation */
typedef struct {
    BigNum** items;
    int top;
    int capacity;
} BigNumStack;

/* Helper: Create BigNum stack */
static BigNumStack* createBigNumStack(int capacity) {
    BigNumStack* stack = (BigNumStack*)malloc(sizeof(BigNumStack));
    if (stack == NULL) return NULL;

    stack->items = (BigNum**)malloc(capacity * sizeof(BigNum*));
    if (stack->items == NULL) {
        free(stack);
        return NULL;
    }

    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

/* Helper: Destroy BigNum stack (frees all BigNums in stack) */
static void destroyBigNumStack(BigNumStack* stack) {
    int i;

    if (stack != NULL) {
        /* Free all remaining BigNums */
        for (i = 0; i <= stack->top; i++) {
            destroyBigNum(stack->items[i]);
        }
        free(stack->items);
        free(stack);
    }
}

/* Helper: Push to BigNum stack */
static bool pushBigNum(BigNumStack* stack, BigNum* num) {
    if (stack->top >= stack->capacity - 1) {
        BigNum** newItems = (BigNum**)realloc(stack->items, stack->capacity * 2 * sizeof(BigNum*));
        if (newItems == NULL) return false;
        stack->items = newItems;
        stack->capacity *= 2;
    }
    stack->items[++stack->top] = num;
    return true;
}

/* Helper: Pop from BigNum stack */
static BigNum* popBigNum(BigNumStack* stack) {
    if (stack->top < 0) return NULL;
    return stack->items[stack->top--];
}

/* Helper: Check if stack is empty */
static bool isBigNumStackEmpty(BigNumStack* stack) {
    return stack->top < 0;
}

/**
 * @brief Evaluates a postfix expression
 */
EvalResult evaluatePostfix(Token* tokens) {
    EvalResult evalResult;
    BigNumStack* stack;
    int i;

    evalResult.result = NULL;
    evalResult.error = EVAL_SUCCESS;

    if (tokens == NULL) {
        evalResult.error = EVAL_ERROR_INVALID_TOKEN;
        return evalResult;
    }

    /* Create evaluation stack */
    stack = createBigNumStack(64);
    if (stack == NULL) {
        evalResult.error = EVAL_ERROR_MEMORY;
        return evalResult;
    }

    /* Process each token */
    for (i = 0; tokens[i].type != TOKEN_END; i++) {
        Token token = tokens[i];

        /* Number: parse and push to stack */
        if (token.type == TOKEN_NUMBER) {
            BigNum* num;

            /* Parse based on format */
            if (isValidBinary(token.value)) {
                num = parseBinary(token.value);
            } else if (isValidHexadecimal(token.value)) {
                num = parseHexadecimal(token.value);
            } else if (isValidDecimal(token.value)) {
                num = parseDecimal(token.value);
            } else {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_INVALID_TOKEN;
                return evalResult;
            }

            if (num == NULL) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }

            if (!pushBigNum(stack, num)) {
                destroyBigNum(num);
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }
        }
        /* Unary minus: pop one operand, negate, push result */
        else if (token.type == TOKEN_UNARY_MINUS) {
            BigNum* operand;
            BigNum* result;

            if (isBigNumStackEmpty(stack)) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_STACK_UNDERFLOW;
                return evalResult;
            }

            operand = popBigNum(stack);
            result = negate(operand);
            destroyBigNum(operand);

            if (result == NULL) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }

            if (!pushBigNum(stack, result)) {
                destroyBigNum(result);
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }
        }
        /* Factorial: pop one operand, compute factorial, push result */
        else if (token.type == TOKEN_FACTORIAL) {
            BigNum* operand;
            BigNum* result;

            if (isBigNumStackEmpty(stack)) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_STACK_UNDERFLOW;
                return evalResult;
            }

            operand = popBigNum(stack);

            /* Check for negative input */
            if (isNegative(operand)) {
                destroyBigNum(operand);
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_NEGATIVE_FACTORIAL;
                return evalResult;
            }

            result = factorial(operand);
            destroyBigNum(operand);

            if (result == NULL) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }

            if (!pushBigNum(stack, result)) {
                destroyBigNum(result);
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }
        }
        /* Binary operator: pop two operands, perform operation, push result */
        else if (token.type == TOKEN_OPERATOR) {
            BigNum* right;
            BigNum* left;
            BigNum* result;
            char op;

            /* Pop operands (right first, then left) */
            if (isBigNumStackEmpty(stack)) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_STACK_UNDERFLOW;
                return evalResult;
            }
            right = popBigNum(stack);

            if (isBigNumStackEmpty(stack)) {
                destroyBigNum(right);
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_STACK_UNDERFLOW;
                return evalResult;
            }
            left = popBigNum(stack);

            op = token.value[0];
            result = NULL;

            /* Perform operation */
            switch (op) {
                case '+':
                    result = add(left, right);
                    break;
                case '-':
                    result = subtract(left, right);
                    break;
                case '*':
                    result = multiply(left, right);
                    break;
                case '/':
                    /* Check for division by zero */
                    if (isZero(right)) {
                        destroyBigNum(left);
                        destroyBigNum(right);
                        destroyBigNumStack(stack);
                        evalResult.error = EVAL_ERROR_DIVISION_BY_ZERO;
                        return evalResult;
                    }
                    result = divide(left, right);
                    break;
                case '%':
                    /* Check for modulo by zero */
                    if (isZero(right)) {
                        destroyBigNum(left);
                        destroyBigNum(right);
                        destroyBigNumStack(stack);
                        evalResult.error = EVAL_ERROR_DIVISION_BY_ZERO;
                        return evalResult;
                    }
                    result = modulo(left, right);
                    break;
                case '^':
                    /* Check for 0^(negative) which is division by zero */
                    if (isZero(left) && isNegative(right)) {
                        destroyBigNum(left);
                        destroyBigNum(right);
                        destroyBigNumStack(stack);
                        evalResult.error = EVAL_ERROR_DIVISION_BY_ZERO;
                        return evalResult;
                    }
                    result = power(left, right);
                    break;
                default:
                    destroyBigNum(left);
                    destroyBigNum(right);
                    destroyBigNumStack(stack);
                    evalResult.error = EVAL_ERROR_INVALID_TOKEN;
                    return evalResult;
            }

            destroyBigNum(left);
            destroyBigNum(right);

            if (result == NULL) {
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }

            if (!pushBigNum(stack, result)) {
                destroyBigNum(result);
                destroyBigNumStack(stack);
                evalResult.error = EVAL_ERROR_MEMORY;
                return evalResult;
            }
        }
        /* Unknown token type */
        else {
            destroyBigNumStack(stack);
            evalResult.error = EVAL_ERROR_INVALID_TOKEN;
            return evalResult;
        }
    }

    /* Result should be exactly one value on stack */
    if (isBigNumStackEmpty(stack)) {
        destroyBigNumStack(stack);
        evalResult.error = EVAL_ERROR_STACK_UNDERFLOW;
        return evalResult;
    }

    evalResult.result = popBigNum(stack);

    /* Stack should now be empty */
    if (!isBigNumStackEmpty(stack)) {
        destroyBigNum(evalResult.result);
        evalResult.result = NULL;
        destroyBigNumStack(stack);
        evalResult.error = EVAL_ERROR_INVALID_TOKEN;
        return evalResult;
    }

    /* Cleanup and return */
    free(stack->items);
    free(stack);

    return evalResult;
}

/**
 * @brief Gets human-readable error message for evaluation error
 */
const char* getEvaluationErrorMessage(EvaluationError error) {
    switch (error) {
        case EVAL_SUCCESS:
            return "Success";
        case EVAL_ERROR_DIVISION_BY_ZERO:
            return "Division by zero!";
        case EVAL_ERROR_NEGATIVE_FACTORIAL:
            return "Input of factorial must not be negative!";
        case EVAL_ERROR_INVALID_TOKEN:
            return "Syntax error!";
        case EVAL_ERROR_STACK_UNDERFLOW:
            return "Syntax error!";
        case EVAL_ERROR_MEMORY:
            return "Memory allocation error!";
        default:
            return "Unknown error!";
    }
}

/**
 * @brief Frees an EvalResult structure
 */
void freeEvalResult(EvalResult* result) {
    if (result != NULL && result->result != NULL) {
        destroyBigNum(result->result);
        result->result = NULL;
    }
}
