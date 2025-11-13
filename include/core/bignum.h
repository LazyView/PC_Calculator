/**
 * @file bignum.h
 * @brief Core arbitrary precision number type and basic utilities
 *
 * This module defines the BigNum structure for representing arbitrary
 * precision integers and provides fundamental operations like creation,
 * destruction, copying, and comparison.
 */

#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdbool.h>

/**
 * @brief Structure representing an arbitrary precision integer
 *
 * The number is stored as a decimal string with a separate sign flag.
 * This representation simplifies arithmetic operations and debugging.
 */
typedef struct {
    bool isNegative;  /**< Sign flag: true for negative, false for non-negative */
    char* digits;     /**< Null-terminated string of decimal digits (no sign) */
} BigNum;

/**
 * @brief Creates a new BigNum from a decimal string
 *
 * @param str Decimal string (may include leading minus sign)
 * @return Pointer to newly allocated BigNum, or NULL on error
 */
BigNum* createBigNum(const char* str);

/**
 * @brief Creates a new BigNum initialized to zero
 *
 * @return Pointer to newly allocated BigNum representing 0
 */
BigNum* createBigNumZero(void);

/**
 * @brief Creates a deep copy of a BigNum
 *
 * @param num BigNum to copy
 * @return Pointer to newly allocated copy, or NULL on error
 */
BigNum* copyBigNum(const BigNum* num);

/**
 * @brief Frees memory associated with a BigNum
 *
 * @param num BigNum to destroy (may be NULL)
 */
void destroyBigNum(BigNum* num);

/**
 * @brief Checks if a BigNum is zero
 *
 * @param num BigNum to check
 * @return true if num represents zero, false otherwise
 */
bool isZero(const BigNum* num);

/**
 * @brief Checks if two BigNums are equal
 *
 * @param a First BigNum
 * @param b Second BigNum
 * @return true if a equals b, false otherwise
 */
bool isEqual(const BigNum* a, const BigNum* b);

/**
 * @brief Checks if first BigNum is less than second
 *
 * @param a First BigNum
 * @param b Second BigNum
 * @return true if a < b, false otherwise
 */
bool isLess(const BigNum* a, const BigNum* b);

/**
 * @brief Compares absolute values of two BigNums
 *
 * @param a First BigNum
 * @param b Second BigNum
 * @return true if |a| < |b|, false otherwise
 */
bool isLessAbs(const BigNum* a, const BigNum* b);

/**
 * @brief Checks if a BigNum is negative
 *
 * @param num BigNum to check
 * @return true if num is negative, false otherwise
 */
bool isNegative(const BigNum* num);

#endif /* BIGNUM_H */
