/**
 * @file converter.c
 * @brief Implementation of input format parsing with two's complement
 */

#include "conversion/converter.h"
#include "core/bignum_ops.h"
#include "app/utils.h"
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

/* Helper: Invert binary string (0→1, 1→0) */
static char* invertBinaryString(const char* binary) {
    char* inverted;
    size_t len;
    size_t i;

    len = strlen(binary);
    inverted = (char*)malloc(len + 1);
    if (inverted == NULL) return NULL;

    for (i = 0; i < len; i++) {
        inverted[i] = (binary[i] == '0') ? '1' : '0';
    }
    inverted[len] = '\0';

    return inverted;
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
 * @brief Parses a binary string to BigNum with two's complement
 */
BigNum* parseBinary(const char* str) {
    const char* digits;
    BigNum* result;
    bool isNegative;

    if (str == NULL || strlen(str) < 3) return NULL;

    /* Check for "0b" prefix */
    if (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) {
        return NULL;
    }

    digits = str + 2;  /* Skip "0b" */

    /* Empty after prefix */
    if (*digits == '\0') return NULL;

    /* Detect sign from leading bit */
    isNegative = (digits[0] == '1');

    if (!isNegative) {
        /* Positive: standard binary conversion */
        result = binaryToDecimal(digits);
    } else {
        /* Negative: two's complement conversion */
        char* inverted;
        BigNum* temp;
        BigNum* one;

        /* Invert all bits */
        inverted = invertBinaryString(digits);
        if (inverted == NULL) return NULL;

        /* Convert inverted to decimal */
        temp = binaryToDecimal(inverted);
        free(inverted);
        if (temp == NULL) return NULL;

        /* Add 1 (completing two's complement) */
        one = createBigNum("1");
        if (one == NULL) {
            destroyBigNum(temp);
            return NULL;
        }

        result = add(temp, one);
        destroyBigNum(temp);
        destroyBigNum(one);
        if (result == NULL) return NULL;

        /* Make negative */
        result->isNegative = true;
    }

    return result;
}

/**
 * @brief Parses a hexadecimal string to BigNum with two's complement
 */
BigNum* parseHexadecimal(const char* str) {
    const char* digits;
    char* binary;
    size_t hexLen;
    size_t binLen;
    size_t i;
    BigNum* result;
    int leadingVal;

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

    /* Detect sign from leading hex digit (>= 8 means negative) */
    leadingVal = hexCharToValue(digits[0]);

    if (leadingVal < 8) {
        /* Positive: standard conversion */
        result = binaryToDecimal(binary);
        free(binary);
    } else {
        /* Negative: two's complement conversion */
        char* inverted;
        BigNum* temp;
        BigNum* one;

        inverted = invertBinaryString(binary);
        free(binary);
        if (inverted == NULL) return NULL;

        temp = binaryToDecimal(inverted);
        free(inverted);
        if (temp == NULL) return NULL;

        one = createBigNum("1");
        if (one == NULL) {
            destroyBigNum(temp);
            return NULL;
        }

        result = add(temp, one);
        destroyBigNum(temp);
        destroyBigNum(one);
        if (result == NULL) return NULL;

        /* Make negative */
        result->isNegative = true;
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
