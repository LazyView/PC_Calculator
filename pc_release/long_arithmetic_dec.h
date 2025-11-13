/**
* @file long_arithmetic_deс.h - header file of long_arithmetic_dec.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @details connecting this file, it is important to check 
* that the header file stdbool.h is connected also,
* as this file uses bool.
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef LONG_ARITHMETIC_DEC
#define LONG_ARITHMETIC_DEC

/**
* Structure describing a decimal number.
* (more in dokumentation).
*/
typedef struct {
    short int sign; /* sign of decimal number */
    char* str_dec;  /* decimal number represented by char pointer(c string) */
} big_dec;

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function is addition in long arithmetic, long arithmetic means you can add numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* decimal numbers, the addition itself is the usual addition in column, as in secondary school.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return big_dec* - the result of adding a and b. 
*/
big_dec* plus_dec(big_dec* a, big_dec* b);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function is subtraction in long arithmetic, long arithmetic means you can subtract numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* decimal numbers, the subtraction itself is the usual subtraction in column, as in secondary school.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return big_dec* - the result of substracting b from a.
*/
big_dec* minus_dec(big_dec* a, big_dec* b);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function is multiplication in long arithmetic, long arithmetic means you can multiply numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* decimal numbers, the multiplication itself is the usual multiplication in column, as in secondary school.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return big_dec* - the result of multiplying a by b.
*/
big_dec* multiply_dec(big_dec* a, big_dec* b);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function is division in long arithmetic, long arithmetic means you can divide numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* decimal numbers, the division itself is the usual division in column, as in secondary school.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return big_dec* - the result of dividing a by b.
*/
big_dec* divide_div_dec(big_dec* a, big_dec* b);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function changes the number sign in long arithmetic, long arithmetic means you can change the sign 
* of numbers that are beyond the reach of any available standard data type, numbers are expressed as
* decimal numbers.
* @param a - number that sign should be changed - represented by big_dec pointer of decimal number. 
* @return big_dec* - number with reversed sign.
*/
big_dec* reverse_dec(big_dec* a);

/**
* This function compares on equalation of two numbers modulo in long arithmetic,
* long arithmetic means you can compare modulo of numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of modulo comparing the number a to number b.
*/
bool is_equal_abs_dec(big_dec* a, big_dec* b);

/**
* This function compares on equalation of two numbers in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal_dec(big_dec* a, big_dec* b);

/**
* This function compare whether the second number modulo is greater than the first number modulo in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of comparing modulo of the number a to the number b
* (is number a modulo less than number b modulo).
*/
bool is_less_abs_dec(big_dec* a, big_dec* b);

/**
* This function compare whether the second number is greater than the first number in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less_dec(big_dec* a, big_dec* b);

/**
* <----  Is necessary to speed up the program, if the length of the number is constant,  ---->
*             but it must be compared, then passing the length of the number,
*                   how can the parameter speed up the program
*
* This function compare whether the second number is greater than the first number in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @param len_a - length of the first number(a).
* @param len_b - length of the second number(b).
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less_fixed_length_dec(big_dec* a, big_dec* b, unsigned long int len_a, unsigned long int len_b);

/**
* <----  Is necessary to speed up the program, if the length of the number is constant,  ---->
*             but it must be compared, then passing the length of the number,
*                   how can the parameter speed up the program
*
* This function compares on equalation of two numbers in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @param len_a - length of the first number(a).
* @param len_b - length of the second number(b).
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal_fixed_length_dec(big_dec* a, big_dec* b, unsigned long int len_a, unsigned long int len_b);

/**
* <----  Is necessary to speed up the program, if the length of the number is constant,  ---->
*             but it must be compared, then passing the length of the number,
*                   how can the parameter speed up the program
* 
* This function function compares number with zero in long arithmetic,
* long arithmetic means you can compare with zero numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - number that should be compared with zero - represented by big_dec pointer of decimal number. 
* @param len - length of the number(a).
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero_fixed_length_dec(big_dec* a, unsigned long int len);

/**
* This function function compares number with zero in long arithmetic,
* long arithmetic means you can compare with zero numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - number that should be compared with zero - represented by big_dec pointer of decimal number. 
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero_dec(big_dec* a);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
*
* 
* This function creates the number(pointer of big_dec structure that represents
* decimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as decimal numbers.
* @param sign - a short int value, that represents sign, value is predefined(more in defines.h). 
* @param str_hex - a string that contains number in a decimal format.
* @return big_dec* - created number of big_dec structure pointer.
*/
big_dec* create_bigdec_object(short int sign, char* str_dec);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
*
* 
* This function creates the copy of number(pointer of big_dec structure that represents
* decimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as decimal numbers.
* @param a - number that copy should be created, number is represented by big_dec structure pointer. 
* @return big_dec* - number a copy, also represented as decimal number.
*/
big_dec* create_bigdec_copy(big_dec* a);

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function destroys the number(pointer of big_dec structure that represents
* decimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as decimal numbers.
* @param a - number whose memory must be released(structure memory free). 
*/
void destroy_bigdec_object(big_dec *a);

#endif