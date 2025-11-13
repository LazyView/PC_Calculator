/**
* @file converter.h - header file of converter.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @details connecting this file, it is important to check 
* that the header files long_arithmetic.h and long_arithmetic_dec.h are connected also,
* as this file uses big_int and big_dec structures.
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef CONVERTER
#define CONVERTER

/**
* @details This function will convert char pointer(c string) of binary number to big_dec pointer
* of decimal number.
* @param number - char pointer(c string) of number in binary format (the number must be verified
* to match the binary format with the third-party function).
* @return big_dec* - pointer of big_dec structure that represents decimal number.
*/
big_dec* bin_to_dec(char* number);

/**
* @details This function will convert char pointer(c string) of hexadecimal number to big_dec pointer
* of decimal number.
* @param number - char pointer(c string) of number in hexadecimal format (the number must be verified
* to match the hexadecimal format with the third-party function).
* @return big_dec* - pointer of big_dec structure that represents decimal number.
*/
big_dec* hex_to_dec(char* number);

/**
* @details This function will convert big_dec pointer of decimal number
* to char pointer(c string) of binary number. 
* @param a - pointer of big_dec structure that represents decimal number. 
* @return char* - number in binary format.
*/
char* dec_to_bin(big_dec* a);

/**
* @details This function will convert big_dec pointer of decimal number
* to char pointer(c string) of hexadecimal number 
* @param a - pointer of big_dec structure that represents decimal number. 
* @return char* - number in hexadecimal format.
*/
char* dec_to_hex(big_dec* a);

/**
* @details This function will convert big_int pointer of pseudo hexadecimal number(more in long_arithmetic.h)
* to char pointer(c string) of hexadecimal number. 
* @param a - pointer of big_int structure that represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
* @return char* - number in hexadecimal format.
*/
char* big_to_hex(big_int* a);

/**
* @details This function will convert big_int pointer of pseudo hexadecimal number(more in long_arithmetic.h)
* to char pointer(c string) of binary number. 
* @param a - pointer of big_int structure that represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
* @return char* - number in binary format.
*/
char* big_to_bin(big_int* a);

/**
* @details This function will convert big_int pointer of pseudo hexadecimal number(more in long_arithmetic.h)
* to big_dec pointer of decimal number.
* @param a - pointer of big_int structure that represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
* @return big_dec* - pointer of big_dec structure that represents decimal number.
*/
big_dec* big_to_dec(big_int* a);

/**
* @details This function will convert big_dec pointer of decimal number
* to big_int pointer of pseudo hexadecimal number(more in long_arithmetic.h).
* @param a - pointer of big_dec structure that represents decimal number.
* @return big_int* - pointer of big_int structure that represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
*/
big_int* dec_to_big(big_dec* a);

/**
* @details This function will convert char pointer(c string) of hexadecimal number
* to big_int pointer of pseudo hexadecimal number(more in long_arithmetic.h).
* @param a - char pointer(c string) of number in hexadecimal format (the number must be verified
* to match the hexadecimal format with the third-party function).
* @return big_int* - pointer of big_int structure that represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
*/
big_int* hex_to_big(char* number);

/**
* @details This function will convert char pointer(c string) of binary number 
* to char pointer(c string) of hexadecimal number. 
* @param number - char pointer(c string) of number in binary format (the number must be verified
* to match the binary format with the third-party function).
* @return char* - number in hexadecimal format.
*/
char* bin_to_hex(char* number);

/**
* @details This function will convert char pointer(c string) of pseudohexadecimal number to binary number.
* @param number - char pointer(c string) of number in pseudohexadecimal format.
* @return char* - char pointer(c string) which represents binary number.
*/
char* abs_hex_to_bin(char *number);

/**
* @details This function prepares big_int for display in the specified format.
* @param a - pointer to big_int structure that represents pseudo hexadecimal number(more in long_arithmetic.h).
* @param mode - variable that indicates in that format the number will be prepared for displaying.
* Predefined modes:
*   MODE_DEC  -> char 'd'
*   MODE_BIN  -> char 'b'
*   MODE_HEX  -> char 'h'
*   MODE_QUIT -> char 'q'
* in other cases error will occured.
* @return char* - prepared number for displaying.
*/
char* to_string(big_int* a, char mode);

#endif