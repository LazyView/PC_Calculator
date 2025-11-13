/**
* @file utils.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "defines.h"

/**
* This function will receive a string like a function parameter 
* and will call functions to process a string such as remove_all_spaces_from_str and string_to_lower.
* @param str - the string that will be deleted blanks and letters will be replaced with small.
* @return char* - modified function's parameter str without spaces and with replaced letters.
*/
char* trim_string(char* str) {
    if (!str) {
        return NULL;
    }

    str = remove_all_spaces_from_str(str);
    str = string_to_lower(str);
    
    return str;
}

/**
* This function will receive strings like a function parameters
* and will replace rep by the substring with in the string orig.
* @param orig - original string.
* @param rep - substring which will be replaced.
* @param with - substring with what rep substring will be replaced on.
* @return char* - modified function's parameter orig with the substring rep replaced by the substring with.
*/
char* str_replace(char* orig, char* rep, char* with) {
    char* str;
    int rep_len, with_len, tail_len;
    
    if (!(str = strstr(orig, rep))) {
        return NULL;
    }

    rep_len = strlen(rep);
    with_len = strlen(with);
    tail_len = strlen(str + rep_len);

    memmove(str + with_len, str + rep_len, tail_len + 1);
    memcpy(str, with, with_len);

    return str;
}

/**
* This funcion will replace all leading zeros from string.
* @example string was "0000123", result of function is "123".
* @param *str - the string from which leading zeros should be removed.
* @return char* - modified function's parameter str without leading zeros.
*/
char* remove_leading_zeros(char* str) {
    int non_space_count;
    unsigned long int i, index, str_len;
    
    if (!str) {
        return NULL;
    }

    str_len = strlen(str);
    index = 0;
    for (i = 0; i < str_len; i++) {
        if (str[i] == CHAR_ZERO) {
            ++index;
        } else {
            break;
        }
    }
    
    if (!index) {
        return str;
    }
    
    if (index == str_len) {
        --index;
    }

    non_space_count = 0;
    for (i = index; i < str_len; i++) {
        str[non_space_count] = str[i];
        ++non_space_count;
    }
    
    str = (char*)realloc(str, (non_space_count + 1) * sizeof(char));
    if (!str) { 
        return NULL;
    }

    str[non_space_count] = CHAR_END_LINE;

    return str;
}

/**
* This function will remove all possible spaces from the string 
* that is passed as function's parameter.
* @param str - the string from which all spaces should be removed.
* @return char* - modified function's parameter str without spaces.
*/
char* remove_all_spaces_from_str(char* str) {
    unsigned long int non_space_count, i;
    
    if (!str) {
        return NULL;
    }

    non_space_count = 0;
    for (i = 0; str[i] != CHAR_END_LINE; i++) {
        if (str[i] != CHAR_SPACE && str[i] != CHAR_TAB) {
            str[non_space_count] = str[i];
            non_space_count++;
        }    
    }
    
    if (strlen(str) == non_space_count) {
        return str;
    }

    str = (char*)realloc(str, (non_space_count + 1) * sizeof(char));
    if (!str) {
        return NULL;
    }
    str[non_space_count] = CHAR_END_LINE;

    return str;
}

/**
* This function will replace all small letters with large.
* @example "HeLpMePlEaSe" -> result : "helpmeplease"
* @param *str - the string in which all letters should be replaced.
* @return char* - modified function's parameter str with replaced letters.
*/
char* string_to_lower(char* str) {
    int i;

    if (!str) {
        return NULL;
    } 

    for (i = 0; str[i] != CHAR_END_LINE; i++) {
        if (str[i] >= CHAR_A && str[i] <= CHAR_Z) {
            str[i] -= CHAR_TO_LOWER_DIFFERENCE;
        }    
    }

    return str;
}

/**
* This function will receive a string from the console, 
* rearrange the necessary memory, and then trim the unnecessary
* memory of the string that was allocated
* @return char* - input string, that was read from console.
*/
char* get_input_string() {
	int current_size = DEFAULT_STRING_SIZE;
    int counter;

	char* my_str = (char*)malloc(DEFAULT_STRING_SIZE * sizeof(char));
    if (!my_str) {
        return NULL;
    }

	counter = 0;
	while ((my_str[counter++] = (char)getchar()) != CHAR_NEW_LINE) {
        if (counter < current_size) {
            my_str = (char*)realloc(my_str, current_size + DEFAULT_STRING_SIZE);
            if (!my_str) {
                free(my_str);
                return NULL;
            }

            current_size += DEFAULT_STRING_SIZE;
        }
    }

    my_str[--counter] = CHAR_END_LINE;
	my_str = (char*)realloc(my_str, counter + 1);
    if (!my_str) {
        free(my_str);
        return NULL;
    }

	return my_str;
}

/**
* This function will read the file line by line.
* @param file_name - the name of the file to be read.
* @param error_code - error code of file reading.
* @param count_of_lines - number of lines in the file.
* @return char** - number of lines in the file.
*/
char** read_file_by_lines(char* file_name, int* error_code, int* count_of_lines) {
    char **lines;
    FILE* file;
    char ch, previous_char;
    int i, current_line, counter, initialize_counter;
    
    *error_code = STATUS_NOT_OK;

    if (!file_name || !error_code) {
        return NULL;
    }

    file = fopen(file_name, "rb");
    if (!file) {
        return NULL;
    }

    *count_of_lines = 0;
    ch = 0;
    previous_char = ch;
    while (!feof(file)) {
        previous_char = ch;
        ch = fgetc(file);
        if (ch == CHAR_NEW_LINE || ch == EOF) {
            ++(*count_of_lines);
        }
    }

    
    if ((previous_char == CHAR_NEW_LINE && ch == EOF) || (!previous_char && ch == EOF)) {
        --(*count_of_lines);
    }

    fclose(file);
    if (!(*count_of_lines)) {
        return NULL;
    }

    file = fopen(file_name, "rb");

    if (!file) {
        return NULL;
    }

    lines = malloc((*count_of_lines) * sizeof(char*));
    for (i = 0; i < (*count_of_lines); i++) {
        lines[i] = malloc((DEFAULT_STRING_SIZE + 1) * sizeof(char));
    }

    current_line = 0;
    do {
        initialize_counter = -1;
        while (++initialize_counter < DEFAULT_STRING_SIZE) {
            lines[current_line][initialize_counter] = CHAR_ZERO;
        }
        lines[current_line][DEFAULT_STRING_SIZE] = CHAR_END_LINE;

        fgets(lines[current_line], DEFAULT_STRING_SIZE, file);
        
        counter = 0;
        
        while (lines[current_line][counter++] != CHAR_NEW_LINE && counter < DEFAULT_STRING_SIZE);

        if (counter > 1 && lines[current_line][counter - 2] != CHAR_CARRIAGE_RETURN) {
            ++counter;
        }

        lines[current_line] = (char*)realloc(lines[current_line], --counter);    
    
        if (!lines[current_line]) {
            for (i = 0; i < *count_of_lines; i++) {
                free(lines[i]);
            }

            free(lines);
            fclose(file);
            
            return NULL;
        }
        lines[current_line][--counter] = CHAR_END_LINE;

        ++current_line;
    } while (current_line < (*count_of_lines));

    fclose(file);

    *error_code = STATUS_OK;
    
    return lines;
}

/**
* This function is a wrapping function of the sprintf function,
* that will convert an int into char.
* @param value - int to convert into a char pointer(c string).
* @param str - conversion result.
*/
void int_to_char(int value, char* str) {
    if (!str) {
        return;
    }

    sprintf(str, "%d", value);
}

/**
* This function reverses the char pointer(с string) contents in reverse order.
* @param str - char pointer(с string) that should be reversed.
* @return char* - reveresed function's parameter str.
*/
char* str_reverse(char* str) {
    int len, i, left, right;

    if (!str) {
        return NULL;
    }

    len = strlen(str), left = 0, right = len - 1;
    for (i = left; i < right; i++) {
        char temp = str[i];
        str[i] = str[right];
        str[right] = temp;
        right--;
    }

    return str;
}

/**
* This function will get the number of digits from the entry int.
* @param temp - entry int.
* @return int - number of digits.
*/
int get_digits_from_int(int temp) {
    int counter;

    if (!temp) {
        return 1;
    }

    counter = 0;
    while (temp) {
        temp /= 10;
        ++counter;
    }
    
    return counter;
}

/**
* This function allows you to insert a substring at a certain position of the passed string.
* @param a - the string to insert the substring.
* @param b - substring to insert.
* @param position - the position on which the substring will be inserted.
* @return char* - modified function's parameter a with inserted substring.
*/
char* insert_substring(char* a, char* b, int position) {
    int length = strlen(a);
    char *f, *e;
    
    f = get_substring(a, 1, position - 1);      
    e = get_substring(a, position, length - position + 1);
 
    strcpy(a, "");
    strcat(a, f);
    free(f);
    strcat(a, b);
    strcat(a, e);
    free(e);
 
    return a;
}

/**
* This function will return the substring from the string
* that was passed as the first parameter of the function.
* @param string - the string from which we take the substring.
* @param position - the position from which the substring will be taken. 
* @param length - length of substring we want to take. 
* @return *char - result substring.
*/
char* get_substring(char* string, int position, int length) {
    char* pointer = (char*)malloc((length + 1) * sizeof(char));
    int i;
   
    if (!pointer) {
        return NULL;
    }
 
    for (i = 0; i < length; i++) {
        pointer[i] = *((string+position-1) + i);
    }
    
    pointer[i] = CHAR_END_LINE;
 
    return pointer;
}

/**
* This function will remove substring from string.
* @param str - string from which to cut the substring.
* @param substr - substring to be cut.
* @return char* - modified function's parameter str with cut substring.
*/
char* str_remove_substring(char *str, char* substr) {
    char *p, *q, *r;
    int len;

    if (*substr && (q = r = strstr(str, substr)) != NULL) {
        len = strlen(substr);
        while ((r = strstr(p = r + len, substr)) != NULL) {
            memmove(q, p, r - p);
            q += r - p;
        }
        memmove(q, p, strlen(p) + 1);
    }
    
    return str;
}

/**
* This function will find the position of hex char in predefined string(more in defines.h), 
* if the passed character is missing in the predefined string, 
* the program will return STATUS_NO_OK(more in defines.h).
* @param c - the char that position you want to find in the string.
* @return int - symbol position.
*/
int find_char_index_in_hex_string(char c) {
    int i;

    for (i = 0; i < HEX_LENGTH_SYMBOLS_ALLOWED; i++) {
        if (SYMBOLS_HEX_ALLOWED[i] == c) {
            return i;
        }
    }

    return STATUS_NOT_OK;
}