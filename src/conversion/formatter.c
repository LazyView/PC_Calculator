/**
 * @file formatter.c
 * @brief Implementation of output formatting with two's complement
 */

#include "../../include/conversion/formatter.h"
#include "../../include/core/bignum_ops.h"
#include "../../include/app/utils.h"
#include <stdlib.h>
#include <string.h>

/* Helper: Convert BigNum to binary string (positive only) */
static char* decimalToBinary(const BigNum* num) {
    BigNum* current;
    BigNum* two;
    BigNum* quotient;
    BigNum* remainder;
    char* binary;
    char* reversed;
    size_t capacity;
    size_t length;

    if (isZero(num)) {
        binary = (char*)malloc(2);
        if (binary) {
            binary[0] = '0';
            binary[1] = '\0';
        }
        return binary;
    }

    capacity = 256;
    binary = (char*)malloc(capacity);
    if (binary == NULL) return NULL;

    current = copyBigNum(num);
    if (current == NULL) {
        free(binary);
        return NULL;
    }

    two = createBigNum("2");
    if (two == NULL) {
        free(binary);
        destroyBigNum(current);
        return NULL;
    }

    length = 0;

    /* Repeatedly divide by 2, collecting remainders */
    while (!isZero(current)) {
        /* Grow buffer if needed */
        if (length + 1 >= capacity) {
            char* newBinary;
            capacity *= 2;
            newBinary = (char*)realloc(binary, capacity);
            if (newBinary == NULL) {
                free(binary);
                destroyBigNum(current);
                destroyBigNum(two);
                return NULL;
            }
            binary = newBinary;
        }

        /* Get remainder (0 or 1) */
        remainder = modulo(current, two);
        if (remainder == NULL) {
            free(binary);
            destroyBigNum(current);
            destroyBigNum(two);
            return NULL;
        }

        binary[length++] = isZero(remainder) ? '0' : '1';
        destroyBigNum(remainder);

        /* Divide by 2 */
        quotient = divide(current, two);
        destroyBigNum(current);
        if (quotient == NULL) {
            free(binary);
            destroyBigNum(two);
            return NULL;
        }
        current = quotient;
    }

    binary[length] = '\0';
    destroyBigNum(current);
    destroyBigNum(two);

    /* Reverse (we built it backwards) */
    reversed = stringDuplicate(binary);
    if (reversed) {
        reverseString(reversed);
    }
    free(binary);

    return reversed;
}

/* Helper: Invert binary string and add 1 (two's complement) */
static char* applyTwosComplement(const char* binary) {
    char* inverted;
    size_t len;
    size_t i;
    int carry;

    len = strlen(binary);
    inverted = (char*)malloc(len + 2);  /* Extra space for possible overflow */
    if (inverted == NULL) return NULL;

    /* Invert all bits */
    for (i = 0; i < len; i++) {
        inverted[i] = (binary[i] == '0') ? '1' : '0';
    }
    inverted[len] = '\0';

    /* Add 1 */
    carry = 1;
    for (i = len; i > 0 && carry; i--) {
        if (inverted[i - 1] == '0') {
            inverted[i - 1] = '1';
            carry = 0;
        } else {
            inverted[i - 1] = '0';
        }
    }

    /* If still have carry, prepend 1 */
    if (carry) {
        memmove(inverted + 1, inverted, len + 1);
        inverted[0] = '1';
    }

    return inverted;
}

/**
 * @brief Formats a BigNum as a decimal string
 */
char* formatDecimal(const BigNum* num) {
    char* result;
    size_t len;

    if (num == NULL || num->digits == NULL) return NULL;

    len = strlen(num->digits);

    /* Allocate: digits + possible minus + null */
    result = (char*)malloc(len + 2);
    if (result == NULL) return NULL;

    if (num->isNegative && strcmp(num->digits, "0") != 0) {
        result[0] = '-';
        strcpy(result + 1, num->digits);
    } else {
        strcpy(result, num->digits);
    }

    return result;
}

/**
 * @brief Formats a BigNum as a binary string with "0b" prefix
 */
char* formatBinary(const BigNum* num) {
    char* binary;
    char* withPrefix;
    size_t len;
    char leadingBit;

    if (num == NULL) return NULL;

    /* Handle zero specially */
    if (isZero(num)) {
        withPrefix = (char*)malloc(4);
        if (withPrefix) {
            strcpy(withPrefix, "0b0");
        }
        return withPrefix;
    }

    if (!num->isNegative) {
        /* Positive: standard conversion */
        BigNum* abs = copyBigNum(num);
        abs->isNegative = false;

        binary = decimalToBinary(abs);
        destroyBigNum(abs);
        if (binary == NULL) return NULL;

        /* Ensure leading bit is 0 (indicates positive) */
        leadingBit = binary[0];
        if (leadingBit == '1') {
            /* Prepend a 0 */
            len = strlen(binary);
            withPrefix = (char*)malloc(len + 4);  /* "0b" + "0" + binary + null */
            if (withPrefix) {
                strcpy(withPrefix, "0b0");
                strcat(withPrefix, binary);
            }
            free(binary);
        } else {
            /* Already has leading 0 */
            withPrefix = (char*)malloc(strlen(binary) + 3);
            if (withPrefix) {
                strcpy(withPrefix, "0b");
                strcat(withPrefix, binary);
            }
            free(binary);
        }
    } else {
        /* Negative: two's complement */
        BigNum* abs = copyBigNum(num);
        char* complement;

        abs->isNegative = false;

        binary = decimalToBinary(abs);
        destroyBigNum(abs);
        if (binary == NULL) return NULL;

        /* Apply two's complement */
        complement = applyTwosComplement(binary);
        free(binary);
        if (complement == NULL) return NULL;

        /* Ensure MSB is 1 for negative numbers in two's complement */
        if (complement[0] == '0') {
            /* Need to prepend 1 to indicate negative */
            size_t len = strlen(complement);
            char* newComp = (char*)malloc(len + 2);
            if (newComp) {
                newComp[0] = '1';
                strcpy(newComp + 1, complement);
                free(complement);
                complement = newComp;
            }
        } else {
            /* MSB is already 1, remove redundant leading 1s */
            /* Can remove '1' bits while next bit is also '1' */
            while (complement[0] == '1' && complement[1] == '1') {
                memmove(complement, complement + 1, strlen(complement));
            }
        }

        withPrefix = (char*)malloc(strlen(complement) + 3);
        if (withPrefix) {
            strcpy(withPrefix, "0b");
            strcat(withPrefix, complement);
        }
        free(complement);
    }

    return withPrefix;
}

/**
 * @brief Formats a BigNum as a hexadecimal string with "0x" prefix
 */
char* formatHexadecimal(const BigNum* num) {
    char* binary;
    char* hex;
    size_t binLen;
    size_t hexLen;
    size_t i, j;
    int val;
    const char hexDigits[] = "0123456789abcdef";
    char* withPrefix;

    if (num == NULL) return NULL;

    /* Handle zero specially */
    if (isZero(num)) {
        withPrefix = (char*)malloc(4);
        if (withPrefix) {
            strcpy(withPrefix, "0x0");
        }
        return withPrefix;
    }

    /* Get binary representation first */
    if (!num->isNegative) {
        /* Positive */
        BigNum* abs = copyBigNum(num);
        abs->isNegative = false;

        binary = decimalToBinary(abs);
        destroyBigNum(abs);
        if (binary == NULL) return NULL;

        /* Ensure leading bit is 0 (pad to multiple of 4) */
        binLen = strlen(binary);
        while (binLen % 4 != 0 || binary[0] == '1') {
            char* newBinary = (char*)malloc(binLen + 2);
            if (newBinary == NULL) {
                free(binary);
                return NULL;
            }
            newBinary[0] = '0';
            strcpy(newBinary + 1, binary);
            free(binary);
            binary = newBinary;
            binLen++;
        }
    } else {
        /* Negative: two's complement */
        BigNum* abs = copyBigNum(num);
        char* complement;

        abs->isNegative = false;

        binary = decimalToBinary(abs);
        destroyBigNum(abs);
        if (binary == NULL) return NULL;

        /* Ensure at least one leading 0 so two's complement gives leading 1 */
        binLen = strlen(binary);
        if (binary[0] == '1') {
            char* newBinary = (char*)malloc(binLen + 2);
            if (newBinary == NULL) {
                free(binary);
                return NULL;
            }
            newBinary[0] = '0';
            strcpy(newBinary + 1, binary);
            free(binary);
            binary = newBinary;
            binLen++;
        }

        /* Pad to multiple of 4 */
        while (binLen % 4 != 0) {
            char* newBinary = (char*)malloc(binLen + 2);
            if (newBinary == NULL) {
                free(binary);
                return NULL;
            }
            newBinary[0] = '0';
            strcpy(newBinary + 1, binary);
            free(binary);
            binary = newBinary;
            binLen++;
        }

        /* Apply two's complement */
        complement = applyTwosComplement(binary);
        free(binary);
        if (complement == NULL) return NULL;

        binary = complement;
        binLen = strlen(binary);
    }

    /* Convert binary to hex (4 bits = 1 hex digit) */
    hexLen = binLen / 4;
    hex = (char*)malloc(hexLen + 1);
    if (hex == NULL) {
        free(binary);
        return NULL;
    }

    for (i = 0, j = 0; i < binLen; i += 4, j++) {
        val = ((binary[i] - '0') << 3) |
              ((binary[i + 1] - '0') << 2) |
              ((binary[i + 2] - '0') << 1) |
              (binary[i + 3] - '0');
        hex[j] = hexDigits[val];
    }
    hex[hexLen] = '\0';
    free(binary);

    /* Remove unnecessary leading digits (but preserve sign indication) */
    if (!num->isNegative) {
        /* Positive: remove leading 0s but keep one leading 0-7 digit */
        while (hex[0] == '0' && hex[1] != '\0' && hex[1] < '8') {
            memmove(hex, hex + 1, strlen(hex));
        }
    } else {
        /* Negative: remove leading Fs but keep one leading 8-F digit */
        while (hex[0] == 'f' && hex[1] != '\0' && hex[1] >= '8') {
            memmove(hex, hex + 1, strlen(hex));
        }
    }

    withPrefix = (char*)malloc(strlen(hex) + 3);
    if (withPrefix) {
        strcpy(withPrefix, "0x");
        strcat(withPrefix, hex);
    }
    free(hex);

    return withPrefix;
}
