/**
 * @file utils.c
 * @brief Implementation of utility functions
 */

#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define INITIAL_BUFFER_SIZE 256
#define MAX_LINE_LENGTH 10000

/* Helper function to check if character is whitespace */
static bool isWhitespaceChar(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/**
 * @brief Removes leading and trailing whitespace from a string
 */
char* trimWhitespace(char* str) {
    char* end;

    if (str == NULL) {
        return NULL;
    }

    /* Trim leading whitespace */
    while (isWhitespaceChar(*str)) {
        str++;
    }

    /* All spaces? */
    if (*str == '\0') {
        return str;
    }

    /* Trim trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isWhitespaceChar(*end)) {
        end--;
    }

    /* Write new null terminator */
    *(end + 1) = '\0';

    return str;
}

/**
 * @brief Converts string to lowercase
 */
char* toLowerCase(char* str) {
    char* p;

    if (str == NULL) {
        return NULL;
    }

    for (p = str; *p != '\0'; p++) {
        *p = (char)tolower((unsigned char)*p);
    }

    return str;
}

/**
 * @brief Removes all whitespace from a string
 */
char* removeWhitespace(const char* str) {
    char* result;
    char* dest;
    const char* src;

    if (str == NULL) {
        return NULL;
    }

    result = (char*)malloc(strlen(str) + 1);
    if (result == NULL) {
        return NULL;
    }

    dest = result;
    src = str;

    while (*src != '\0') {
        if (!isWhitespaceChar(*src)) {
            *dest++ = *src;
        }
        src++;
    }

    *dest = '\0';
    return result;
}

/**
 * @brief Reads a line from stdin
 */
char* readLine(void) {
    char* buffer;
    size_t bufferSize;
    size_t length;
    int ch;

    bufferSize = INITIAL_BUFFER_SIZE;
    buffer = (char*)malloc(bufferSize);
    if (buffer == NULL) {
        return NULL;
    }

    length = 0;

    /* Read characters until newline or EOF */
    while ((ch = getchar()) != EOF && ch != '\n') {
        /* Grow buffer if needed */
        if (length + 1 >= bufferSize) {
            char* newBuffer;
            bufferSize *= 2;

            /* Check for maximum line length */
            if (bufferSize > MAX_LINE_LENGTH) {
                free(buffer);
                return NULL;
            }

            newBuffer = (char*)realloc(buffer, bufferSize);
            if (newBuffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }

        buffer[length++] = (char)ch;
    }

    /* Handle EOF with no input */
    if (ch == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    /* Null-terminate */
    buffer[length] = '\0';

    /* Shrink buffer to actual size */
    if (length + 1 < bufferSize) {
        char* newBuffer = (char*)realloc(buffer, length + 1);
        if (newBuffer != NULL) {
            buffer = newBuffer;
        }
    }

    return buffer;
}

/**
 * @brief Reads all lines from a file
 */
char** readFile(const char* filename, int* lineCount) {
    FILE* file;
    char** lines;
    int capacity;
    int count;
    char buffer[MAX_LINE_LENGTH];

    if (filename == NULL || lineCount == NULL) {
        return NULL;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        *lineCount = 0;
        return NULL;
    }

    /* Initial capacity */
    capacity = 16;
    lines = (char**)malloc(capacity * sizeof(char*));
    if (lines == NULL) {
        fclose(file);
        *lineCount = 0;
        return NULL;
    }

    count = 0;

    /* Read lines */
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        size_t len;
        char* line;

        /* Remove newline characters */
        len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[len - 1] = '\0';
            len--;
        }
        if (len > 0 && (buffer[len - 1] == '\r')) {
            buffer[len - 1] = '\0';
        }

        /* Grow array if needed */
        if (count >= capacity) {
            char** newLines;
            capacity *= 2;
            newLines = (char**)realloc(lines, capacity * sizeof(char*));
            if (newLines == NULL) {
                /* Free already allocated lines */
                while (count > 0) {
                    free(lines[--count]);
                }
                free(lines);
                fclose(file);
                *lineCount = 0;
                return NULL;
            }
            lines = newLines;
        }

        /* Duplicate line */
        line = stringDuplicate(buffer);
        if (line == NULL) {
            /* Free already allocated lines */
            while (count > 0) {
                free(lines[--count]);
            }
            free(lines);
            fclose(file);
            *lineCount = 0;
            return NULL;
        }

        lines[count++] = line;
    }

    fclose(file);

    /* Add NULL terminator */
    if (count >= capacity) {
        char** newLines = (char**)realloc(lines, (count + 1) * sizeof(char*));
        if (newLines != NULL) {
            lines = newLines;
        }
    }

    if (lines != NULL) {
        lines[count] = NULL;
    }

    *lineCount = count;
    return lines;
}

/**
 * @brief Frees an array of strings
 */
void freeStringArray(char** lines) {
    int i;

    if (lines == NULL) {
        return;
    }

    for (i = 0; lines[i] != NULL; i++) {
        free(lines[i]);
    }

    free(lines);
}

/**
 * @brief Checks if a character is a digit
 */
bool isDigitChar(char c) {
    return c >= '0' && c <= '9';
}

/**
 * @brief Checks if a character is a hexadecimal digit
 */
bool isHexDigitChar(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

/**
 * @brief Duplicates a string
 */
char* stringDuplicate(const char* str) {
    char* duplicate;
    size_t len;

    if (str == NULL) {
        return NULL;
    }

    len = strlen(str);
    duplicate = (char*)malloc(len + 1);
    if (duplicate == NULL) {
        return NULL;
    }

    memcpy(duplicate, str, len + 1);
    return duplicate;
}

/**
 * @brief Reverses a string in place
 */
char* reverseString(char* str) {
    char* start;
    char* end;
    char temp;

    if (str == NULL || *str == '\0') {
        return str;
    }

    start = str;
    end = str + strlen(str) - 1;

    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    return str;
}
