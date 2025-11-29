/**
 * @file converter.c
 * @brief Implementation of input format parsing with two's complement
 */

#include "converter.h"
#include "bignum_ops.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Helper: Convert hex character to value */
static int hexCharToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* Helper: Convert binary string to decimal (positive only) */
static BigNum* binaryToDecimal(const char* binary) {
    BigNum* result;
    BigNum* two;
    BigNum* temp;
    const char* p;

    result = createBigNumZero();
    if (result == NULL) return NULL;

    two = createBigNum("2");
    if (two == NULL) {
        destroyBigNum(result);
        return NULL;
    }

    /* Process each bit */
    for (p = binary; *p != '\0'; p++) {
        /* result = result * 2 */
        temp = multiply(result, two);
        destroyBigNum(result);
        if (temp == NULL) {
            destroyBigNum(two);
            return NULL;
        }
        result = temp;

        /* If bit is 1, add 1 */
        if (*p == '1') {
            BigNum* one = createBigNum("1");
            if (one == NULL) {
                destroyBigNum(result);
                destroyBigNum(two);
                return NULL;
            }
            temp = add(result, one);
            destroyBigNum(one);
            destroyBigNum(result);
            if (temp == NULL) {
                destroyBigNum(two);
                return NULL;
            }
            result = temp;
        }
    }

    destroyBigNum(two);
    return result;
}

/**
 * @brief Parses a decimal string to BigNum
 */
BigNum* parseDecimal(const char* str) {
    if (str == NULL) return NULL;

    /* Decimal parsing is handled directly by createBigNum */
    return createBigNum(str);
}

/**
 * @brief Parses a binary string to BigNum using two's complement
 *
 * Binary literals are interpreted as two's complement numbers:
 * - If MSB (first bit) is 0: positive number (parse as unsigned)
 * - If MSB (first bit) is 1: negative number (two's complement)
 *
 * Examples:
 * - 0b0101 = 5 (MSB=0, positive)
 * - 0b1010 = -6 (MSB=1, two's complement: -8+2=-6)
 * - 0b1111 = -1 (MSB=1, two's complement: -8+4+2+1=-1)
 */
BigNum* parseBinary(const char* str) {
    const char* digits;
    size_t numBits;

    if (str == NULL || strlen(str) < 3) return NULL;

    /* Check for "0b" prefix */
    if (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) {
        return NULL;
    }

    digits = str + 2;  /* Skip "0b" */

    /* Empty after prefix */
    if (*digits == '\0') return NULL;

    numBits = strlen(digits);

    /* Check MSB for two's complement sign */
    if (digits[0] == '1') {
        /* MSB is 1: negative number in two's complement
         * Value = (lower bits as unsigned) - (2^(n-1))
         * where n is the total number of bits
         */
        BigNum* lowerBits;
        BigNum* msbWeight;
        BigNum* two;
        BigNum* result;
        size_t i;

        /* Calculate MSB weight = 2^(numBits-1) */
        two = createBigNum("2");
        if (two == NULL) return NULL;

        msbWeight = createBigNum("1");
        if (msbWeight == NULL) {
            destroyBigNum(two);
            return NULL;
        }

        for (i = 1; i < numBits; i++) {
            BigNum* temp = multiply(msbWeight, two);
            destroyBigNum(msbWeight);
            if (temp == NULL) {
                destroyBigNum(two);
                return NULL;
            }
            msbWeight = temp;
        }
        destroyBigNum(two);

        /* Parse lower bits (all except MSB) as unsigned */
        if (numBits == 1) {
            /* Single bit '1' = -1 */
            lowerBits = createBigNumZero();
        } else {
            lowerBits = binaryToDecimal(digits + 1);
        }
        if (lowerBits == NULL) {
            destroyBigNum(msbWeight);
            return NULL;
        }

        /* Result = lowerBits - msbWeight (negative result) */
        result = subtract(lowerBits, msbWeight);
        destroyBigNum(lowerBits);
        destroyBigNum(msbWeight);

        return result;
    } else {
        /* MSB is 0: positive number, parse as unsigned */
        return binaryToDecimal(digits);
    }
}

/**
 * @brief Parses a hexadecimal string to BigNum using two's complement
 *
 * Hex literals are interpreted as two's complement numbers:
 * - If first hex digit < 8 (0-7): MSB=0, positive number
 * - If first hex digit >= 8 (8-F): MSB=1, negative number
 *
 * Examples:
 * - 0x5 = 5 (first digit=5 < 8, positive)
 * - 0xf = -1 (first digit=15 >= 8, two's complement: 7-8=-1)
 * - 0xa = -6 (first digit=10 >= 8, two's complement: 2-8=-6)
 */
BigNum* parseHexadecimal(const char* str) {
    const char* digits;
    char* binary;
    size_t hexLen;
    size_t binLen;
    size_t i;
    BigNum* result;

    if (str == NULL || strlen(str) < 3) return NULL;

    /* Check for "0x" prefix */
    if (str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
        return NULL;
    }

    digits = str + 2;  /* Skip "0x" */

    /* Empty after prefix */
    if (*digits == '\0') return NULL;

    hexLen = strlen(digits);
    binLen = hexLen * 4;  /* Each hex digit = 4 binary bits */

    /* Allocate binary string */
    binary = (char*)malloc(binLen + 1);
    if (binary == NULL) return NULL;

    /* Convert hex to binary (4 bits per hex digit) */
    for (i = 0; i < hexLen; i++) {
        int val = hexCharToValue(digits[i]);
        if (val < 0) {
            free(binary);
            return NULL;
        }

        /* Convert to 4 binary digits */
        binary[i * 4 + 0] = (val & 8) ? '1' : '0';
        binary[i * 4 + 1] = (val & 4) ? '1' : '0';
        binary[i * 4 + 2] = (val & 2) ? '1' : '0';
        binary[i * 4 + 3] = (val & 1) ? '1' : '0';
    }
    binary[binLen] = '\0';

    /* Use parseBinary which handles two's complement */
    /* Need to add "0b" prefix for parseBinary */
    {
        char* binaryWithPrefix = (char*)malloc(binLen + 3);
        if (binaryWithPrefix == NULL) {
            free(binary);
            return NULL;
        }
        binaryWithPrefix[0] = '0';
        binaryWithPrefix[1] = 'b';
        strcpy(binaryWithPrefix + 2, binary);
        free(binary);

        result = parseBinary(binaryWithPrefix);
        free(binaryWithPrefix);
    }

    return result;
}

/**
 * @brief Validates if a string is a valid decimal number
 */
bool isValidDecimal(const char* str) {
    const char* p;

    if (str == NULL || *str == '\0') return false;

    p = str;

    /* Optional sign */
    if (*p == '-' || *p == '+') {
        p++;
    }

    /* Must have at least one digit */
    if (*p == '\0') return false;

    /* Check all remaining characters are digits */
    while (*p != '\0') {
        if (!isDigitChar(*p)) return false;
        p++;
    }

    return true;
}

/**
 * @brief Validates if a string is a valid binary number
 */
bool isValidBinary(const char* str) {
    const char* p;

    if (str == NULL || strlen(str) < 3) return false;

    /* Check for "0b" prefix */
    if (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) {
        return false;
    }

    p = str + 2;

    /* Must have at least one digit */
    if (*p == '\0') return false;

    /* Check all remaining characters are 0 or 1 */
    while (*p != '\0') {
        if (*p != '0' && *p != '1') return false;
        p++;
    }

    return true;
}

/**
 * @brief Validates if a string is a valid hexadecimal number
 */
bool isValidHexadecimal(const char* str) {
    const char* p;

    if (str == NULL || strlen(str) < 3) return false;

    /* Check for "0x" prefix */
    if (str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
        return false;
    }

    p = str + 2;

    /* Must have at least one digit */
    if (*p == '\0') return false;

    /* Check all remaining characters are hex digits */
    while (*p != '\0') {
        if (!isHexDigitChar(*p)) return false;
        p++;
    }

    return true;
}
