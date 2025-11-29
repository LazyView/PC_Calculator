/**
 * @file bignum_ops.c
 * @brief Implementation of basic arithmetic operations
 */

#include "bignum_ops.h"
#include "bignum.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

/* Helper: Remove leading zeros from a digit string */
static void removeLeadingZeros(char* digits) {
    char* p = digits;
    char* dest;

    while (*p == '0' && *(p + 1) != '\0') {
        p++;
    }

    if (p != digits) {
        dest = digits;
        while (*p != '\0') {
            *dest++ = *p++;
        }
        *dest = '\0';
    }
}

/* Helper: Add two unsigned numbers (as digit strings) */
static char* addUnsigned(const char* a, const char* b) {
    size_t lenA, lenB, maxLen;
    char* result;
    int carry, sum, i, j, k;

    lenA = strlen(a);
    lenB = strlen(b);
    maxLen = (lenA > lenB ? lenA : lenB) + 1;

    result = (char*)malloc(maxLen + 1);
    if (result == NULL) return NULL;

    carry = 0;
    i = (int)lenA - 1;
    j = (int)lenB - 1;
    k = (int)maxLen - 1;
    result[k + 1] = '\0';

    while (i >= 0 || j >= 0 || carry) {
        sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result[k--] = (char)((sum % 10) + '0');
        carry = sum / 10;
    }

    /* Shift result to start at position 0 */
    /* After loop, k is at position before first digit, so digits start at k+1 */
    if (k >= 0) {
        memmove(result, result + k + 1, strlen(result + k + 1) + 1);
    }

    removeLeadingZeros(result);
    return result;
}

/* Helper: Subtract unsigned b from unsigned a (assumes a >= b) */
static char* subtractUnsigned(const char* a, const char* b) {
    size_t lenA, lenB;
    char* result;
    int borrow, diff, i, j, k;

    lenA = strlen(a);
    lenB = strlen(b);

    result = (char*)malloc(lenA + 1);
    if (result == NULL) return NULL;

    borrow = 0;
    i = (int)lenA - 1;
    j = (int)lenB - 1;
    k = (int)lenA - 1;
    result[lenA] = '\0';

    while (i >= 0) {
        diff = (a[i--] - '0') - borrow;
        if (j >= 0) diff -= (b[j--] - '0');

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[k--] = (char)(diff + '0');
    }

    removeLeadingZeros(result);
    return result;
}

/**
 * @brief Adds two BigNums
 */
BigNum* add(const BigNum* a, const BigNum* b) {
    BigNum* result;
    char* digits;

    if (a == NULL || b == NULL) return NULL;

    result = (BigNum*)malloc(sizeof(BigNum));
    if (result == NULL) return NULL;

    /* Same signs: add magnitudes, keep sign */
    if (a->isNegative == b->isNegative) {
        digits = addUnsigned(a->digits, b->digits);
        if (digits == NULL) {
            free(result);
            return NULL;
        }
        result->digits = digits;
        result->isNegative = a->isNegative && strcmp(digits, "0") != 0;
    }
    /* Different signs: subtract smaller from larger */
    else {
        if (isLessAbs(a, b)) {
            digits = subtractUnsigned(b->digits, a->digits);
            if (digits == NULL) {
                free(result);
                return NULL;
            }
            result->digits = digits;
            result->isNegative = b->isNegative && strcmp(digits, "0") != 0;
        } else {
            digits = subtractUnsigned(a->digits, b->digits);
            if (digits == NULL) {
                free(result);
                return NULL;
            }
            result->digits = digits;
            result->isNegative = a->isNegative && strcmp(digits, "0") != 0;
        }
    }

    return result;
}

/**
 * @brief Subtracts second BigNum from first
 */
BigNum* subtract(const BigNum* a, const BigNum* b) {
    BigNum* negB;
    BigNum* result;

    if (a == NULL || b == NULL) return NULL;

    /* a - b = a + (-b) */
    negB = negate(b);
    if (negB == NULL) return NULL;

    result = add(a, negB);
    destroyBigNum(negB);

    return result;
}

/**
 * @brief Multiplies two BigNums
 */
BigNum* multiply(const BigNum* a, const BigNum* b) {
    BigNum* result;
    size_t lenA, lenB, lenResult;
    int* temp;
    int i, j;
    char* digits;

    if (a == NULL || b == NULL) return NULL;

    /* Handle zero */
    if (isZero(a) || isZero(b)) {
        return createBigNumZero();
    }

    lenA = strlen(a->digits);
    lenB = strlen(b->digits);
    lenResult = lenA + lenB;

    /* Allocate temporary array for multiplication */
    temp = (int*)calloc(lenResult, sizeof(int));
    if (temp == NULL) return NULL;

    /* Long multiplication */
    for (i = (int)lenA - 1; i >= 0; i--) {
        for (j = (int)lenB - 1; j >= 0; j--) {
            int product = (a->digits[i] - '0') * (b->digits[j] - '0');
            int pos = i + j + 1;

            temp[pos] += product;
            temp[pos - 1] += temp[pos] / 10;
            temp[pos] %= 10;
        }
    }

    /* Convert to string */
    digits = (char*)malloc(lenResult + 1);
    if (digits == NULL) {
        free(temp);
        return NULL;
    }

    for (i = 0; i < (int)lenResult; i++) {
        digits[i] = (char)(temp[i] + '0');
    }
    digits[lenResult] = '\0';
    free(temp);

    removeLeadingZeros(digits);

    result = (BigNum*)malloc(sizeof(BigNum));
    if (result == NULL) {
        free(digits);
        return NULL;
    }

    result->digits = digits;
    result->isNegative = (a->isNegative != b->isNegative) && strcmp(digits, "0") != 0;

    return result;
}

/**
 * @brief Divides first BigNum by second (integer division)
 */
BigNum* divide(const BigNum* a, const BigNum* b) {
    BigNum *quotient, *current, *temp, *one, *absB;
    char* qDigits;
    size_t lenA, i, qLen;
    bool resultNegative;

    if (a == NULL || b == NULL) return NULL;

    /* Division by zero */
    if (isZero(b)) return NULL;

    /* Dividend is zero */
    if (isZero(a)) return createBigNumZero();

    /* Divisor larger than dividend */
    if (isLessAbs(a, b)) return createBigNumZero();

    /* Determine result sign */
    resultNegative = (a->isNegative != b->isNegative);

    /* Create absolute value of b for division loop */
    absB = copyBigNum(b);
    if (absB == NULL) return NULL;
    absB->isNegative = false;

    lenA = strlen(a->digits);
    qDigits = (char*)malloc(lenA + 1);
    if (qDigits == NULL) {
        destroyBigNum(absB);
        return NULL;
    }

    current = createBigNumZero();
    if (current == NULL) {
        free(qDigits);
        destroyBigNum(absB);
        return NULL;
    }

    one = createBigNum("1");
    if (one == NULL) {
        free(qDigits);
        destroyBigNum(current);
        destroyBigNum(absB);
        return NULL;
    }

    qLen = 0;

    /* Long division (working with absolute values) */
    for (i = 0; i < lenA; i++) {
        char digitStr[2];
        BigNum* digit;
        int count;

        /* Bring down next digit */
        digitStr[0] = a->digits[i];
        digitStr[1] = '\0';
        digit = createBigNum(digitStr);
        if (digit == NULL) {
            free(qDigits);
            destroyBigNum(current);
            destroyBigNum(one);
            destroyBigNum(absB);
            return NULL;
        }

        {
            BigNum* ten = createBigNum("10");
            temp = multiply(current, ten);
            destroyBigNum(ten);
            destroyBigNum(current);
            current = add(temp, digit);
            destroyBigNum(temp);
            destroyBigNum(digit);
        }

        /* Count how many times |b| fits into current */
        count = 0;
        while (!isLess(current, absB)) {
            temp = subtract(current, absB);
            destroyBigNum(current);
            current = temp;
            count++;
        }

        qDigits[qLen++] = (char)(count + '0');
    }

    qDigits[qLen] = '\0';
    destroyBigNum(current);
    destroyBigNum(one);
    destroyBigNum(absB);

    removeLeadingZeros(qDigits);

    quotient = (BigNum*)malloc(sizeof(BigNum));
    if (quotient == NULL) {
        free(qDigits);
        return NULL;
    }

    quotient->digits = qDigits;
    quotient->isNegative = resultNegative && strcmp(qDigits, "0") != 0;

    return quotient;
}

/**
 * @brief Computes modulo of first BigNum by second
 * Formula: a % m = a - m * floor(a/m) where floor rounds toward zero
 */
BigNum* modulo(const BigNum* a, const BigNum* b) {
    BigNum *quotient, *product, *remainder;

    if (a == NULL || b == NULL) return NULL;

    /* Division by zero */
    if (isZero(b)) return NULL;

    /* a % b = a - b * (a / b) */
    quotient = divide(a, b);
    if (quotient == NULL) return NULL;

    product = multiply(b, quotient);
    destroyBigNum(quotient);
    if (product == NULL) return NULL;

    remainder = subtract(a, product);
    destroyBigNum(product);

    return remainder;
}

/**
 * @brief Negates a BigNum (changes sign)
 */
BigNum* negate(const BigNum* a) {
    BigNum* result;

    if (a == NULL) return NULL;

    result = copyBigNum(a);
    if (result == NULL) return NULL;

    /* Zero is never negative */
    if (!isZero(result)) {
        result->isNegative = !result->isNegative;
    }

    return result;
}
