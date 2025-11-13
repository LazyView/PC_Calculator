/**
 * @file bignum_math.c
 * @brief Implementation of advanced mathematical operations
 */

#include "core/bignum_math.h"
#include "core/bignum_ops.h"
#include "core/bignum.h"
#include <stdlib.h>

/**
 * @brief Raises a BigNum to a power
 * Uses binary exponentiation for efficiency
 */
BigNum* power(const BigNum* base, const BigNum* exponent) {
    BigNum *result, *currentBase, *currentExp, *two, *temp, *remainder;
    bool isOdd;

    if (base == NULL || exponent == NULL) {
        return NULL;
    }

    /* Exponent must be non-negative */
    if (isNegative(exponent)) {
        return NULL;
    }

    /* Special case: base^0 = 1 */
    if (isZero(exponent)) {
        return createBigNum("1");
    }

    /* Special case: 0^n = 0 (for n > 0) */
    if (isZero(base)) {
        return createBigNumZero();
    }

    /* Special case: base^1 = base */
    {
        BigNum* one = createBigNum("1");
        if (one != NULL && isEqual(exponent, one)) {
            destroyBigNum(one);
            return copyBigNum(base);
        }
        if (one != NULL) {
            destroyBigNum(one);
        }
    }

    /* Initialize result to 1 */
    result = createBigNum("1");
    if (result == NULL) {
        return NULL;
    }

    /* Copy base and exponent for manipulation */
    currentBase = copyBigNum(base);
    if (currentBase == NULL) {
        destroyBigNum(result);
        return NULL;
    }

    currentExp = copyBigNum(exponent);
    if (currentExp == NULL) {
        destroyBigNum(result);
        destroyBigNum(currentBase);
        return NULL;
    }

    two = createBigNum("2");
    if (two == NULL) {
        destroyBigNum(result);
        destroyBigNum(currentBase);
        destroyBigNum(currentExp);
        return NULL;
    }

    /* Binary exponentiation: base^exp */
    while (!isZero(currentExp)) {
        /* Check if exponent is odd (exp % 2 == 1) */
        remainder = modulo(currentExp, two);
        isOdd = !isZero(remainder);
        destroyBigNum(remainder);

        if (isOdd) {
            /* result *= currentBase */
            temp = multiply(result, currentBase);
            destroyBigNum(result);
            result = temp;
            if (result == NULL) {
                destroyBigNum(currentBase);
                destroyBigNum(currentExp);
                destroyBigNum(two);
                return NULL;
            }
        }

        /* currentBase *= currentBase (square) */
        temp = multiply(currentBase, currentBase);
        destroyBigNum(currentBase);
        currentBase = temp;
        if (currentBase == NULL) {
            destroyBigNum(result);
            destroyBigNum(currentExp);
            destroyBigNum(two);
            return NULL;
        }

        /* currentExp /= 2 */
        temp = divide(currentExp, two);
        destroyBigNum(currentExp);
        currentExp = temp;
        if (currentExp == NULL) {
            destroyBigNum(result);
            destroyBigNum(currentBase);
            destroyBigNum(two);
            return NULL;
        }
    }

    destroyBigNum(currentBase);
    destroyBigNum(currentExp);
    destroyBigNum(two);

    return result;
}

/**
 * @brief Computes factorial of a BigNum
 * Uses iterative multiplication
 */
BigNum* factorial(const BigNum* n) {
    BigNum *result, *current, *one, *temp;

    if (n == NULL) {
        return NULL;
    }

    /* Factorial of negative number is undefined */
    if (isNegative(n)) {
        return NULL;
    }

    /* Special case: 0! = 1 and 1! = 1 */
    if (isZero(n)) {
        return createBigNum("1");
    }

    /* Initialize result to 1 */
    result = createBigNum("1");
    if (result == NULL) {
        return NULL;
    }

    /* Create counter starting at 2 */
    current = createBigNum("2");
    if (current == NULL) {
        destroyBigNum(result);
        return NULL;
    }

    /* Create increment value */
    one = createBigNum("1");
    if (one == NULL) {
        destroyBigNum(result);
        destroyBigNum(current);
        return NULL;
    }

    /* Multiply result by each number from 2 to n */
    while (!isLess(n, current)) {
        /* result *= current */
        temp = multiply(result, current);
        destroyBigNum(result);
        result = temp;
        if (result == NULL) {
            destroyBigNum(current);
            destroyBigNum(one);
            return NULL;
        }

        /* current++ */
        temp = add(current, one);
        destroyBigNum(current);
        current = temp;
        if (current == NULL) {
            destroyBigNum(result);
            destroyBigNum(one);
            return NULL;
        }
    }

    destroyBigNum(current);
    destroyBigNum(one);

    return result;
}
