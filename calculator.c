/**
 * @file calculator.c
 * @brief Implementation of calculator application logic
 */

#include "calculator.h"
#include "utils.h"
#include "parser.h"
#include "evaluator.h"
#include "formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Characters allowed in arithmetic expressions */
#define EXPR_CHARS "0123456789abcdefABCDEFxX!^-*/%+() "

/**
 * @brief Checks if input looks like an arithmetic expression
 * @param input The string to check
 * @return true if it contains expression characters, false otherwise
 *
 * An expression must contain at least one:
 * - Operator: + - * / % ^ !
 * - Digit: 0-9
 * - Parenthesis: ( )
 * - Hex/Binary prefix: 0x 0b
 *
 * If it's just letters/underscores without any of these, it's a command.
 */
static bool looksLikeExpression(const char* input) {
    size_t i;
    bool hasOperator;
    bool hasDigit;
    bool hasParens;

    if (input == NULL || *input == '\0') return false;

    hasOperator = false;
    hasDigit = false;
    hasParens = false;

    /* Check for expression-specific characters */
    for (i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        /* Check for operators */
        if (c == '+' || c == '-' || c == '*' || c == '/' ||
            c == '%' || c == '^' || c == '!') {
            hasOperator = true;
        }

        /* Check for digits */
        if (c >= '0' && c <= '9') {
            hasDigit = true;
        }

        /* Check for parentheses */
        if (c == '(' || c == ')') {
            hasParens = true;
        }
    }

    /* An expression must have at least an operator, digit, or parentheses */
    return (hasOperator || hasDigit || hasParens);
}

/**
 * @brief Gets the name of the current output mode
 * @param mode The output mode
 * @return String name of the mode
 */
static const char* getModeName(OutputMode mode) {
    switch (mode) {
        case MODE_DECIMAL:     return "dec";
        case MODE_BINARY:      return "bin";
        case MODE_HEXADECIMAL: return "hex";
        default:               return "dec";
    }
}

/**
 * @brief Initializes calculator state
 */
CalculatorState* initCalculator(void) {
    CalculatorState* state = (CalculatorState*)malloc(sizeof(CalculatorState));
    if (state == NULL) return NULL;

    state->mode = MODE_DECIMAL;  /* Default to decimal mode */
    return state;
}

/**
 * @brief Frees calculator state
 */
void destroyCalculator(CalculatorState* state) {
    free(state);
}

/**
 * @brief Processes a single command or expression
 */
bool processInput(CalculatorState* state, const char* input) {
    char* inputCopy;
    char* trimmed;
    char* lower;
    Token* postfix;
    EvalResult result;
    char* output;

    if (state == NULL || input == NULL) return true;

    /* Make a copy of input to safely modify */
    inputCopy = stringDuplicate(input);
    if (inputCopy == NULL) return true;

    /* Trim and convert to lowercase for command matching */
    trimmed = trimWhitespace(inputCopy);
    if (trimmed == NULL || *trimmed == '\0') {
        free(inputCopy);
        return true;  /* Empty line - continue */
    }

    lower = toLowerCase(trimmed);

    /* Command: quit */
    if (strcmp(lower, "quit") == 0) {
        free(inputCopy);
        return false;
    }

    /* Command: out (show current output format) */
    if (strcmp(lower, "out") == 0) {
        printf("%s\n", getModeName(state->mode));
        free(inputCopy);
        return true;
    }

    /* Command: dec (decimal mode) */
    if (strcmp(lower, "dec") == 0) {
        state->mode = MODE_DECIMAL;
        printf("dec\n");
        free(inputCopy);
        return true;
    }

    /* Command: bin (binary mode) */
    if (strcmp(lower, "bin") == 0) {
        state->mode = MODE_BINARY;
        printf("bin\n");
        free(inputCopy);
        return true;
    }

    /* Command: hex (hexadecimal mode) */
    if (strcmp(lower, "hex") == 0) {
        state->mode = MODE_HEXADECIMAL;
        printf("hex\n");
        free(inputCopy);
        return true;
    }

    /* Check if input looks like an expression (has arithmetic chars) */
    if (!looksLikeExpression(trimmed)) {
        printf("Invalid command \"%s\"!\n", trimmed);
        free(inputCopy);
        return true;
    }

    /* Otherwise, treat as expression */
    /* Parse expression to postfix */
    postfix = infixToPostfix(trimmed);
    if (postfix == NULL) {
        printf("Syntax error!\n");
        free(inputCopy);
        return true;
    }

    /* Evaluate postfix expression */
    result = evaluatePostfix(postfix);
    freeTokens(postfix);

    /* Check for evaluation errors */
    if (result.error != EVAL_SUCCESS) {
        printf("%s\n", getEvaluationErrorMessage(result.error));
        freeEvalResult(&result);
        free(inputCopy);
        return true;
    }

    /* Format and print result based on current mode */
    output = NULL;
    switch (state->mode) {
        case MODE_DECIMAL:
            output = formatDecimal(result.result);
            break;
        case MODE_BINARY:
            output = formatBinary(result.result);
            break;
        case MODE_HEXADECIMAL:
            output = formatHexadecimal(result.result);
            break;
    }

    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    } else {
        printf("Memory allocation error!\n");
    }

    freeEvalResult(&result);
    free(inputCopy);
    return true;
}

/**
 * @brief Runs calculator in interactive mode
 */
void runInteractiveMode(void) {
    CalculatorState* state;
    char* line;
    bool shouldContinue;

    state = initCalculator();
    if (state == NULL) {
        fprintf(stderr, "Memory allocation error!\n");
        return;
    }

    shouldContinue = true;
    while (shouldContinue) {
        /* Read line from stdin */
        line = readLine();
        if (line == NULL) {
            /* EOF or error - exit gracefully */
            break;
        }

        /* Process the input */
        shouldContinue = processInput(state, line);

        free(line);
    }

    destroyCalculator(state);
}

/**
 * @brief Runs calculator in file mode
 */
bool runFileMode(const char* filename) {
    CalculatorState* state;
    char** lines;
    int lineCount;
    int i;
    bool shouldContinue;

    if (filename == NULL) return false;

    /* Read all lines from file */
    lines = readFile(filename, &lineCount);
    if (lines == NULL) {
        printf("Invalid input file!\n");
        return false;
    }

    /* Initialize calculator */
    state = initCalculator();
    if (state == NULL) {
        freeStringArray(lines);
        fprintf(stderr, "Memory allocation error!\n");
        return false;
    }

    /* Process each line */
    shouldContinue = true;
    for (i = 0; i < lineCount && shouldContinue; i++) {
        shouldContinue = processInput(state, lines[i]);
    }

    /* Cleanup */
    destroyCalculator(state);
    freeStringArray(lines);

    return true;
}
