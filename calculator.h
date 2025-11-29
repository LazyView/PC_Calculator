/**
 * @file calculator.h
 * @brief Main calculator application logic
 *
 * This module provides the main application interface including
 * command processing, mode management, and both interactive and
 * file-based operation modes.
 */

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdbool.h>

/**
 * @brief Output format modes
 */
typedef enum {
    MODE_DECIMAL,      /**< Decimal output mode */
    MODE_BINARY,       /**< Binary output mode */
    MODE_HEXADECIMAL   /**< Hexadecimal output mode */
} OutputMode;

/**
 * @brief Calculator state
 */
typedef struct {
    OutputMode mode;   /**< Current output mode */
} CalculatorState;

/**
 * @brief Initializes calculator state
 *
 * @return Pointer to newly allocated calculator state, or NULL on error
 */
CalculatorState* initCalculator(void);

/**
 * @brief Frees calculator state
 *
 * @param state Calculator state to free
 */
void destroyCalculator(CalculatorState* state);

/**
 * @brief Processes a single command or expression
 *
 * Handles commands: "dec", "bin", "hex", "quit"
 * Evaluates arithmetic expressions and prints results.
 *
 * @param state Calculator state
 * @param input Input string (command or expression)
 * @return true to continue, false to quit
 */
bool processInput(CalculatorState* state, const char* input);

/**
 * @brief Runs calculator in interactive mode
 *
 * Reads input from stdin and processes commands/expressions.
 */
void runInteractiveMode(void);

/**
 * @brief Runs calculator in file mode
 *
 * Reads and processes commands/expressions from a file.
 *
 * @param filename Path to input file
 * @return true on success, false on error
 */
bool runFileMode(const char* filename);

#endif /* CALCULATOR_H */
