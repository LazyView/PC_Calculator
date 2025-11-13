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

/* TODO: Implement evaluatePostfix */
EvalResult evaluatePostfix(Token* tokens) {
    EvalResult result;
    result.result = NULL;
    result.error = EVAL_ERROR_MEMORY;
    /* Implementation placeholder */
    return result;
}

/* TODO: Implement getEvaluationErrorMessage */
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

/* TODO: Implement freeEvalResult */
void freeEvalResult(EvalResult* result) {
    /* Implementation placeholder */
}
