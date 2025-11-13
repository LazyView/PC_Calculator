/**
 * @file bignum_ops.h
 * @brief Basic arithmetic operations for arbitrary precision numbers
 *
 * This module provides fundamental arithmetic operations: addition,
 * subtraction, multiplication, division, and modulo for BigNum types.
 */

#ifndef BIGNUM_OPS_H
#define BIGNUM_OPS_H

#include "bignum.h"

/**
 * @brief Adds two BigNums
 *
 * @param a First operand
 * @param b Second operand
 * @return Pointer to newly allocated BigNum containing a + b, or NULL on error
 */
BigNum* add(const BigNum* a, const BigNum* b);

/**
 * @brief Subtracts second BigNum from first
 *
 * @param a First operand (minuend)
 * @param b Second operand (subtrahend)
 * @return Pointer to newly allocated BigNum containing a - b, or NULL on error
 */
BigNum* subtract(const BigNum* a, const BigNum* b);

/**
 * @brief Multiplies two BigNums
 *
 * @param a First operand
 * @param b Second operand
 * @return Pointer to newly allocated BigNum containing a * b, or NULL on error
 */
BigNum* multiply(const BigNum* a, const BigNum* b);

/**
 * @brief Divides first BigNum by second (integer division)
 *
 * @param a Dividend
 * @param b Divisor
 * @return Pointer to newly allocated BigNum containing a / b, or NULL on error or division by zero
 */
BigNum* divide(const BigNum* a, const BigNum* b);

/**
 * @brief Computes modulo of first BigNum by second
 *
 * @param a Dividend
 * @param b Divisor
 * @return Pointer to newly allocated BigNum containing a % b, or NULL on error or division by zero
 */
BigNum* modulo(const BigNum* a, const BigNum* b);

/**
 * @brief Negates a BigNum (changes sign)
 *
 * @param a BigNum to negate
 * @return Pointer to newly allocated BigNum containing -a, or NULL on error
 */
BigNum* negate(const BigNum* a);

#endif /* BIGNUM_OPS_H */
