/**
 * @file bignum_math.h
 * @brief Advanced mathematical operations for arbitrary precision numbers
 *
 * This module provides advanced mathematical operations such as
 * exponentiation and factorial for BigNum types.
 */

#ifndef BIGNUM_MATH_H
#define BIGNUM_MATH_H

#include "bignum.h"

/**
 * @brief Raises a BigNum to a power
 *
 * Computes base^exponent using efficient algorithms.
 * Exponent must be non-negative.
 *
 * @param base Base number
 * @param exponent Exponent (must be non-negative)
 * @return Pointer to newly allocated BigNum containing base^exponent, or NULL on error
 */
BigNum* power(const BigNum* base, const BigNum* exponent);

/**
 * @brief Computes factorial of a BigNum
 *
 * Computes n! for non-negative integer n.
 * Input must be non-negative.
 *
 * @param n Number to compute factorial of (must be non-negative)
 * @return Pointer to newly allocated BigNum containing n!, or NULL on error or negative input
 */
BigNum* factorial(const BigNum* n);

#endif /* BIGNUM_MATH_H */
