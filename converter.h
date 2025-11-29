/**
 * @file converter.h
 * @brief Input format parsing and conversion to BigNum
 *
 * This module handles parsing of numbers in different formats (binary,
 * hexadecimal, decimal) and converts them to BigNum representation.
 * Supports two's complement interpretation for binary and hexadecimal.
 */

#ifndef CONVERTER_H
#define CONVERTER_H

#include "bignum.h"
#include <stdbool.h>

/**
 * @brief Parses a decimal string to BigNum
 *
 * Accepts standard decimal notation with optional leading minus sign.
 *
 * @param str Decimal string (e.g., "-123", "456")
 * @return Pointer to newly allocated BigNum, or NULL on error
 */
BigNum* parseDecimal(const char* str);

/**
 * @brief Parses a binary string to BigNum
 *
 * Accepts binary strings with "0b" prefix (e.g., "0b1010").
 * Interprets using two's complement for negative numbers.
 *
 * @param str Binary string with "0b" prefix
 * @return Pointer to newly allocated BigNum, or NULL on error
 */
BigNum* parseBinary(const char* str);

/**
 * @brief Parses a hexadecimal string to BigNum
 *
 * Accepts hexadecimal strings with "0x" prefix (e.g., "0xFF").
 * Interprets using two's complement for negative numbers.
 * Accepts both uppercase and lowercase letters.
 *
 * @param str Hexadecimal string with "0x" prefix
 * @return Pointer to newly allocated BigNum, or NULL on error
 */
BigNum* parseHexadecimal(const char* str);

/**
 * @brief Validates if a string is a valid decimal number
 *
 * @param str String to validate
 * @return true if valid decimal, false otherwise
 */
bool isValidDecimal(const char* str);

/**
 * @brief Validates if a string is a valid binary number
 *
 * @param str String to validate (should have "0b" prefix)
 * @return true if valid binary, false otherwise
 */
bool isValidBinary(const char* str);

/**
 * @brief Validates if a string is a valid hexadecimal number
 *
 * @param str String to validate (should have "0x" prefix)
 * @return true if valid hexadecimal, false otherwise
 */
bool isValidHexadecimal(const char* str);

#endif /* CONVERTER_H */
