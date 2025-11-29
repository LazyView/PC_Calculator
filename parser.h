/**
 * @file parser.h
 * @brief Expression parsing and validation
 *
 * This module handles parsing of mathematical expressions, including
 * tokenization, syntax validation, and conversion from infix to postfix
 * notation using the Shunting Yard algorithm.
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

/**
 * @brief Token types for expression parsing
 */
typedef enum {
    TOKEN_NUMBER,       /**< Numeric literal */
    TOKEN_OPERATOR,     /**< Binary operator: +, -, *, /, %, ^ */
    TOKEN_UNARY_MINUS,  /**< Unary minus operator */
    TOKEN_FACTORIAL,    /**< Factorial operator: ! */
    TOKEN_LPAREN,       /**< Left parenthesis */
    TOKEN_RPAREN,       /**< Right parenthesis */
    TOKEN_END           /**< End of expression */
} TokenType;

/**
 * @brief Represents a single token in an expression
 */
typedef struct {
    TokenType type;     /**< Type of token */
    char* value;        /**< String value (for numbers and operators) */
} Token;

/**
 * @brief Validates an arithmetic expression
 *
 * Checks for:
 * - Valid characters
 * - Balanced parentheses
 * - Valid operator placement
 * - Valid number formats
 *
 * @param expr Expression string to validate
 * @return true if expression is valid, false otherwise
 */
bool validateExpression(const char* expr);

/**
 * @brief Converts infix expression to postfix (Shunting Yard algorithm)
 *
 * @param expr Infix expression string
 * @return Array of tokens in postfix order (NULL-terminated), or NULL on error
 */
Token* infixToPostfix(const char* expr);

/**
 * @brief Frees memory allocated for token array
 *
 * @param tokens Token array to free (NULL-terminated)
 */
void freeTokens(Token* tokens);

/**
 * @brief Gets operator precedence
 *
 * @param op Operator character
 * @return Precedence level (higher number = higher precedence), or -1 if invalid
 */
int getOperatorPrecedence(char op);

/**
 * @brief Checks if operator is right-associative
 *
 * @param op Operator character
 * @return true if right-associative (^), false otherwise
 */
bool isRightAssociative(char op);

#endif /* PARSER_H */
