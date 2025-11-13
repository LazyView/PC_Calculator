/**
 * @file parser.c
 * @brief Implementation of expression parsing
 */

#include "expression/parser.h"
#include "app/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* TODO: Implement validateExpression */
bool validateExpression(const char* expr) {
    /* Implementation placeholder */
    return false;
}

/* TODO: Implement infixToPostfix */
Token* infixToPostfix(const char* expr) {
    /* Implementation placeholder */
    return NULL;
}

/* TODO: Implement freeTokens */
void freeTokens(Token* tokens) {
    /* Implementation placeholder */
}

/* TODO: Implement getOperatorPrecedence */
int getOperatorPrecedence(char op) {
    /* Implementation placeholder */
    return -1;
}

/* TODO: Implement isRightAssociative */
bool isRightAssociative(char op) {
    /* Implementation placeholder */
    return false;
}
