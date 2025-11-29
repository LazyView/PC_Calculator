/**
 * @file bignum.c
 * @brief Implementation of core BigNum operations
 */

#include "../../include/core/bignum.h"
#include "../../include/app/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Helper function to remove leading zeros from a string of digits */
static char* removeLeadingZeros(char* digits) {
    char* p = digits;

    /* Skip leading zeros */
    while (*p == '0' && *(p + 1) != '\0') {
        p++;
    }

    /* Move result to beginning if needed */
    if (p != digits) {
        char* src = p;
        char* dest = digits;
        while (*src != '\0') {
            *dest++ = *src++;
        }
        *dest = '\0';
    }

    return digits;
}

/**
 * @brief Creates a new BigNum from a decimal string
 */
BigNum* createBigNum(const char* str) {
    BigNum* num;
    const char* p;
    char* digitsStart;
    size_t len;
    bool negative;

    if (str == NULL || *str == '\0') {
        return NULL;
    }

    /* Allocate BigNum structure */
    num = (BigNum*)malloc(sizeof(BigNum));
    if (num == NULL) {
        return NULL;
    }

    /* Skip whitespace */
    p = str;
    while (isspace((unsigned char)*p)) {
        p++;
    }

    /* Check for sign */
    negative = false;
    if (*p == '-') {
        negative = true;
        p++;
    } else if (*p == '+') {
        p++;
    }

    /* Validate and count digits */
    digitsStart = (char*)p;
    len = 0;
    while (*p != '\0') {
        if (!isDigitChar(*p)) {
            free(num);
            return NULL;
        }
        len++;
        p++;
    }

    /* Empty digits */
    if (len == 0) {
        free(num);
        return NULL;
    }

    /* Allocate and copy digits */
    num->digits = (char*)malloc(len + 1);
    if (num->digits == NULL) {
        free(num);
        return NULL;
    }

    memcpy(num->digits, digitsStart, len);
    num->digits[len] = '\0';

    /* Remove leading zeros */
    removeLeadingZeros(num->digits);

    /* Zero is never negative */
    if (strcmp(num->digits, "0") == 0) {
        num->isNegative = false;
    } else {
        num->isNegative = negative;
    }

    return num;
}

/**
 * @brief Creates a new BigNum initialized to zero
 */
BigNum* createBigNumZero(void) {
    BigNum* num;

    num = (BigNum*)malloc(sizeof(BigNum));
    if (num == NULL) {
        return NULL;
    }

    num->digits = (char*)malloc(2);
    if (num->digits == NULL) {
        free(num);
        return NULL;
    }

    num->digits[0] = '0';
    num->digits[1] = '\0';
    num->isNegative = false;

    return num;
}

/**
 * @brief Creates a deep copy of a BigNum
 */
BigNum* copyBigNum(const BigNum* num) {
    BigNum* copy;

    if (num == NULL) {
        return NULL;
    }

    copy = (BigNum*)malloc(sizeof(BigNum));
    if (copy == NULL) {
        return NULL;
    }

    copy->digits = stringDuplicate(num->digits);
    if (copy->digits == NULL) {
        free(copy);
        return NULL;
    }

    copy->isNegative = num->isNegative;

    return copy;
}

/**
 * @brief Frees memory associated with a BigNum
 */
void destroyBigNum(BigNum* num) {
    if (num == NULL) {
        return;
    }

    if (num->digits != NULL) {
        free(num->digits);
    }

    free(num);
}

/**
 * @brief Checks if a BigNum is zero
 */
bool isZero(const BigNum* num) {
    if (num == NULL || num->digits == NULL) {
        return false;
    }

    return strcmp(num->digits, "0") == 0;
}

/**
 * @brief Checks if two BigNums are equal
 */
bool isEqual(const BigNum* a, const BigNum* b) {
    if (a == NULL || b == NULL) {
        return false;
    }

    /* Same sign and same digits */
    if (a->isNegative != b->isNegative) {
        return false;
    }

    return strcmp(a->digits, b->digits) == 0;
}

/**
 * @brief Compares absolute values of two BigNums
 * @return true if |a| < |b|, false otherwise
 */
bool isLessAbs(const BigNum* a, const BigNum* b) {
    size_t lenA, lenB;

    if (a == NULL || b == NULL) {
        return false;
    }

    lenA = strlen(a->digits);
    lenB = strlen(b->digits);

    /* Compare lengths first */
    if (lenA < lenB) {
        return true;
    }
    if (lenA > lenB) {
        return false;
    }

    /* Same length - compare lexicographically */
    return strcmp(a->digits, b->digits) < 0;
}

/**
 * @brief Checks if first BigNum is less than second
 */
bool isLess(const BigNum* a, const BigNum* b) {
    if (a == NULL || b == NULL) {
        return false;
    }

    /* Handle different signs */
    if (a->isNegative && !b->isNegative) {
        return true;  /* negative < positive */
    }
    if (!a->isNegative && b->isNegative) {
        return false;  /* positive >= negative */
    }

    /* Same sign */
    if (a->isNegative) {
        /* Both negative: -5 < -3, so compare opposite */
        return isLessAbs(b, a);
    } else {
        /* Both positive */
        return isLessAbs(a, b);
    }
}

/**
 * @brief Checks if a BigNum is negative
 */
bool isNegative(const BigNum* num) {
    if (num == NULL) {
        return false;
    }

    return num->isNegative && !isZero(num);
}
