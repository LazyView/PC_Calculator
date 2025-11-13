/**
 * @file evaluator.h
 * @brief Expression evaluation
 *
 * This module handles evaluation of postfix expressions, managing
 * the evaluation stack and performing operations using BigNum arithmetic.
 */

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "core/bignum.h"
#include "parser.h"
#include <stdbool.h>

/**
 * @brief Error codes for expression evaluation
 */
typedef enum {
    EVAL_SUCCESS = 0,              /**< Evaluation succeeded */
    EVAL_ERROR_DIVISION_BY_ZERO,   /**< Division by zero attempted */
    EVAL_ERROR_NEGATIVE_FACTORIAL, /**< Factorial of negative number */
    EVAL_ERROR_INVALID_TOKEN,      /**< Invalid token encountered */
    EVAL_ERROR_STACK_UNDERFLOW,    /**< Not enough operands */
    EVAL_ERROR_MEMORY              /**< Memory allocation failed */
} EvaluationError;

/**
 * @brief Evaluation result containing result and error code
 */
typedef struct {
    BigNum* result;           /**< Result of evaluation (NULL on error) */
    EvaluationError error;    /**< Error code */
} EvalResult;

/**
 * @brief Evaluates a postfix expression
 *
 * @param tokens Array of tokens in postfix order (NULL-terminated)
 * @return EvalResult containing result and error code
 */
EvalResult evaluatePostfix(Token* tokens);

/**
 * @brief Gets human-readable error message for evaluation error
 *
 * @param error Error code
 * @return Error message string (static, do not free)
 */
const char* getEvaluationErrorMessage(EvaluationError error);

/**
 * @brief Frees an EvalResult structure
 *
 * @param result EvalResult to free
 */
void freeEvalResult(EvalResult* result);

#endif /* EVALUATOR_H */
