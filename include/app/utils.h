/**
 * @file utils.h
 * @brief Utility functions for string manipulation and I/O
 *
 * This module provides helper functions for string processing,
 * file I/O, and other common operations.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * @brief Removes leading and trailing whitespace from a string
 *
 * @param str String to trim (modified in place)
 * @return Pointer to trimmed string (same as input)
 */
char* trimWhitespace(char* str);

/**
 * @brief Converts string to lowercase
 *
 * @param str String to convert (modified in place)
 * @return Pointer to converted string (same as input)
 */
char* toLowerCase(char* str);

/**
 * @brief Removes all whitespace from a string
 *
 * @param str String to process
 * @return Newly allocated string without whitespace, or NULL on error
 */
char* removeWhitespace(const char* str);

/**
 * @brief Reads a line from stdin
 *
 * Dynamically allocates memory for the line.
 *
 * @return Newly allocated string containing the line, or NULL on EOF or error
 */
char* readLine(void);

/**
 * @brief Reads all lines from a file
 *
 * @param filename Path to file
 * @param lineCount Output parameter for number of lines read
 * @return Array of strings (NULL-terminated), or NULL on error
 */
char** readFile(const char* filename, int* lineCount);

/**
 * @brief Frees an array of strings
 *
 * @param lines Array of strings (NULL-terminated)
 */
void freeStringArray(char** lines);

/**
 * @brief Checks if a character is a digit
 *
 * @param c Character to check
 * @return true if digit, false otherwise
 */
bool isDigitChar(char c);

/**
 * @brief Checks if a character is a hexadecimal digit
 *
 * @param c Character to check
 * @return true if hex digit (0-9, a-f, A-F), false otherwise
 */
bool isHexDigitChar(char c);

/**
 * @brief Duplicates a string
 *
 * @param str String to duplicate
 * @return Newly allocated copy of string, or NULL on error
 */
char* stringDuplicate(const char* str);

/**
 * @brief Reverses a string in place
 *
 * @param str String to reverse (modified in place)
 * @return Pointer to reversed string (same as input)
 */
char* reverseString(char* str);

#endif /* UTILS_H */
