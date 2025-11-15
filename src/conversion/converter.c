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
    BigNum* msbPower;
    BigNum* temp;
    const char* p;

    if (str == NULL || strlen(str) < 3) return NULL;

    /* Check for "0b" prefix */
    if (str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) {
        return NULL;
    }

    digits = str + 2;  /* Skip "0b" */

    /* Empty after prefix */
    if (*digits == '\0') return NULL;

    /* Two's complement interpretation: first bit is sign bit (MSB) */
    /* If MSB is '1', the number is negative */

    if (digits[0] == '1') {
        /* Negative number in two's complement */
        /* Parse remaining bits as unsigned */
        result = binaryToDecimal(digits + 1);
        if (result == NULL) return NULL;

        /* Calculate 2^(n-1) where n is total number of bits */
        /* This is the weight of the MSB (negative in two's complement) */
        msbPower = createBigNum("1");
        if (msbPower == NULL) {
            destroyBigNum(result);
            return NULL;
        }

        /* Multiply by 2 for each bit position (n-1 times) */
        for (p = digits + 1; *p != '\0'; p++) {
            BigNum* two = createBigNum("2");
            if (two == NULL) {
                destroyBigNum(result);
                destroyBigNum(msbPower);
                return NULL;
            }
            temp = multiply(msbPower, two);
            destroyBigNum(two);
            destroyBigNum(msbPower);
            if (temp == NULL) {
                destroyBigNum(result);
                return NULL;
            }
            msbPower = temp;
        }

        /* Subtract MSB power: result = (lower bits) - 2^(n-1) */
        temp = subtract(result, msbPower);
        destroyBigNum(result);
        destroyBigNum(msbPower);

        return temp;
    } else {
        /* Positive number (MSB is '0') - parse as unsigned */
        result = binaryToDecimal(digits);
        return result;
    }
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
    int firstDigitValue;
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

    /* Two's complement interpretation based on first hex digit */
    /* If first digit >= 8, the MSB is 1, so number is negative */
    firstDigitValue = hexCharToValue(digits[0]);

    if (firstDigitValue >= 8) {
        /* Negative number - MSB is 1 */
        /* Parse binary using two's complement (delegate to parseBinary) */
        /* Create temporary string with "0b" prefix */
        char* tempBinary = (char*)malloc(binLen + 3);
        if (tempBinary == NULL) {
            free(binary);
            return NULL;
        }
        tempBinary[0] = '0';
        tempBinary[1] = 'b';
        strcpy(tempBinary + 2, binary);
        free(binary);

        /* Use parseBinary which now handles two's complement */
        result = parseBinary(tempBinary);
        free(tempBinary);
    } else {
        /* Positive number - parse as unsigned */
        result = binaryToDecimal(binary);
        free(binary);
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
