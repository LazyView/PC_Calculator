/**
 * @file formatter.h
 * @brief Output formatting and conversion from BigNum
 *
 * This module handles conversion of BigNum to different output formats
 * (binary, hexadecimal, decimal) with proper formatting including
 * two's complement for negative numbers in binary/hexadecimal.
 */

#ifndef FORMATTER_H
#define FORMATTER_H

#include "bignum.h"

/**
 * @brief Formats a BigNum as a decimal string
 *
 * Produces minimal representation (no unnecessary leading zeros).
 * Negative numbers have a leading minus sign.
 *
 * @param num BigNum to format
 * @return Newly allocated string containing decimal representation, or NULL on error
 */
char* formatDecimal(const BigNum* num);

/**
 * @brief Formats a BigNum as a binary string with "0b" prefix
 *
 * Uses two's complement representation for negative numbers.
 * Produces minimal representation (no unnecessary leading bits).
 *
 * @param num BigNum to format
 * @return Newly allocated string containing binary representation, or NULL on error
 */
char* formatBinary(const BigNum* num);

/**
 * @brief Formats a BigNum as a hexadecimal string with "0x" prefix
 *
 * Uses two's complement representation for negative numbers.
 * Produces minimal representation (no unnecessary leading digits).
 * Uses lowercase letters a-f.
 *
 * @param num BigNum to format
 * @return Newly allocated string containing hexadecimal representation, or NULL on error
 */
char* formatHexadecimal(const BigNum* num);

#endif /* FORMATTER_H */
