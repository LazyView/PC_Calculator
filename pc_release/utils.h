/**
* @file utils.h - header file of utils.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef UTILS
#define UTILS

/**
* This function will receive a string like a function parameter 
* and will call functions to process a string such as remove_all_spaces_from_str and string_to_lower.
* @param str - the string that will be deleted blanks and letters will be replaced with small.
* @retur char* - modified function's parameter str without spaces and with replaced letters.
*/
char* trim_string(char* str);

/**
* This function will receive strings like a function parameters
* and will replace rep by the substring with in the string orig.
* @param orig - original string.
* @param rep - substring which will be replaced.
* @param with - substring with what rep substring will be replaced on.
* @return char* - modified function's parameter orig with the substring rep replaced by the substring with.
*/
char* str_replace(char* orig, char* rep, char* with);

/**
* This funcion will replace all leading zeros from string.
* @example string was "0000123", result of function is "123".
* @param *str - the string from which leading zeros should be removed.
* @return char* - modified function's parameter str without leading zeros.
*/
char* remove_leading_zeros(char* str);

/**
* This function will remove all possible spaces from the string 
* that is passed as function's parameter.
* @param str - the string from which all spaces should be removed.
* @return char* - modified function's parameter str without spaces.
*/
char* remove_all_spaces_from_str(char* str);

/**
* This function will replace all small letters with large.
* @example "HeLpMePlEaSe" -> result : "helpmeplease"
* @param *str - the string in which all letters should be replaced.
* @return char* - modified function's parameter str with replaced letters.
*/
char* string_to_lower(char* str);

/**
* This function will receive a string from the console, 
* rearrange the necessary memory, and then trim the unnecessary
* memory of the string that was allocated
* @return char* - input string, that was read from console.
*/
char* get_input_string();

/**
* This function will read the file line by line.
* @param file_name - the name of the file to be read.
* @param error_code - error code of file reading.
* @param count_of_lines - number of lines in the file.
* @return char** - number of lines in the file.
*/
char** read_file_by_lines(char* file_name, int* error_code, int* count_of_lines);

/**
* This function is a wrapping function of the sprintf function,
* that will convert an int into char.
* @param value - int to convert into a char pointer(c string).
* @param str - conversion result.
*/
void int_to_char(int value, char* str);

/**
* This function reverses the char pointer(с string) contents in reverse order.
* @param str - char pointer(с string) that should be reversed.
* @return char* - reveresed function's parameter str.
*/
char* str_reverse(char* str);

/**
* This function will get the number of digits from the entry int.
* @param temp - entry int.
* @return int - number of digits.
*/
int get_digits_from_int(int temp);

/**
* This function allows you to insert a substring at a certain position of the passed string.
* @param a - the string to insert the substring.
* @param b - substring to insert.
* @param position - the position on which the substring will be inserted.
* @return char* - modified function's parameter a with inserted substring.
*/
char* insert_substring(char* a, char* b, int position);

/**
* This function will return the substring from the string
* that was passed as the first parameter of the function.
* @param string - the string from which we take the substring.
* @param position - the position from which the substring will be taken. 
* @param length - length of substring we want to take. 
* @return *char - result substring.
*/
char* get_substring(char* string, int position, int length);

/**
* This function will remove substring from string.
* @param str - string from which to cut the substring.
* @param substr - substring to be cut.
* @return char* - modified function's parameter str with cut substring.
*/
char* str_remove_substring(char* str, char* substr);

/**
* This function will find the position of hex char in predefined string(more in defines.h), 
* if the passed character is missing in the predefined string, 
* the program will return STATUS_NO_OK(more in defines.h).
* @param c - the char that position you want to find in the string.
* @param int - symbol position.
*/
int find_char_index_in_hex_string(char c);

#endif