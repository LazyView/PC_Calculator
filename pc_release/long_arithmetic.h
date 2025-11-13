/**
* @file long_arithmetic.h - header file of long_arithmetic.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @details connecting this file, it is important to check 
* that the header file stdbool.h is connected also,
* as this file uses bool.
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef LONG_ARITHMETIC
#define LONG_ARITHMETIC

/**
* Structure describing a pseudo hexadecimal number
* pseudo hexadecimal number. This structure is represented by
* numbers from 0 to 9 like in normal hexadecimal number and ASCII symbols that
* are letters in the normal hexadecimal number.
* normal hex: 'A' -> pseudo hex: ':'
* normal hex: 'B' -> pseudo hex: ';'
* normal hex: 'C' -> pseudo hex: '<'
* normal hex: 'D' -> pseudo hex: '='
* normal hex: 'E' -> pseudo hex: '>'
* normal hex: 'F' -> pseudo hex: '?'
* (more in dokumentation).
*/
typedef struct {
    short int sign;       /* sign of pseudo hexadecimal number */
    char* str_pseudo_hex; /* pseudo number represented by char pointer(c string) */
} big_int;

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is addition in long arithmetic, long arithmetic means you can add numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers, the addition itself is the usual addition in column, as in secondary school.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return big_int* - the result of adding a and b. 
*/
big_int* plus(big_int* a, big_int* b);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is subtraction in long arithmetic, long arithmetic means you can subtract numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers, the subtraction itself is the usual subtraction in column, as in secondary school.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return big_int* - the result of substracting b from a.
*/
big_int* minus(big_int* a, big_int* b);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is multiplication in long arithmetic, long arithmetic means you can multiply numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers, the multiplication itself is the usual multiplication in column, as in secondary school.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return big_int* - the result of multiplying a by b.
*/
big_int* multiply(big_int* a, big_int* b);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is division in long arithmetic, long arithmetic means you can divide numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers, the division itself is the usual division in column, as in secondary school.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return big_int* - the result of dividing a by b.
*/
big_int* divide_div(big_int* a, big_int* b);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is modulo division in long arithmetic, long arithmetic means you can divide numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return big_int* - the result of finding the remainder of division a by b
*/
big_int* divide_mod(big_int* a, big_int* b);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is a tree for finding factorial in long arithmetic, means you can find factorial of a number
* that are beyond the reach of any available standard data type quickly, using tree for segregation of duties. Numbers are expressed as
* pseudo hexadecimal numbers.
* @param left - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param right - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @return big_int* - the result of finding the multiplication of a given range of numbers.
*/
big_int* prod_tree(big_int* left, big_int* right);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is factorial in long arithmetic, long arithmetic means you can find factorial of a number
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @return big_int* - the result of finding the factorial of a given number.
*/
big_int* factorial(big_int* a);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function is exponentiation in long arithmetic, long arithmetic means you can exponentiate numbers
* that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return big_int* - the result of raising the number a to the power b.
*/
big_int* power(big_int* a, big_int* b);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function changes the number sign in long arithmetic, long arithmetic means you can change the sign 
* of numbers that are beyond the reach of any available standard data type, numbers are expressed as
* pseudo hexadecimal numbers.
* @param a - number that sign should be changed - represented by big_int pointer of pseudo hexadecimal number. 
* @return big_int* - number with reversed sign.
*/
big_int* reverse(big_int* a);

/**
* This function compares on equalation of two numbers modulo in long arithmetic,
* long arithmetic means you can compare modulo of numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of modulo comparing the number a to number b.
*/
bool is_equal_abs(big_int* a, big_int* b);

/**
* This function compares on equalation of two numbers in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal(big_int* a, big_int* b);

/** 
* This function compare whether the second number modulo is greater than the first number modulo in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of comparing modulo of the number a to the number b
* (is number a modulo less than number b modulo).
*/
bool is_less_abs(big_int* a, big_int* b);

/** 
* This function compare whether the second number is greater than the first number in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less(big_int* a, big_int* b);

/** 
* <----  Is necessary to speed up the program, if the length of the number is constant,  ---->
*             but it must be compared, then passing the length of the number,
*                   how can the parameter speed up the program
*
* This function compare whether the second number is greater than the first number in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @param len_a - length of the first number(a).
* @param len_b - length of the second number(b).
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less_fixed_length(big_int* a, big_int* b, unsigned long int len_a, unsigned long int len_b);

/** 
* <----  Is necessary to speed up the program, if the length of the number is constant,  ---->
*             but it must be compared, then passing the length of the number,
*                   how can the parameter speed up the program
*
* This function compares on equalation of two numbers in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @param len_a - length of the first number(a).
* @param len_b - length of the second number(b).
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal_fixed_length(big_int* a, big_int* b, unsigned long int len_a, unsigned long int len_b);

/** 
* <----  Is necessary to speed up the program, if the length of the number is constant,  ---->
*             but it must be compared, then passing the length of the number,
*                   how can the parameter speed up the program
* 
* This function function compares number with zero in long arithmetic,
* long arithmetic means you can compare with zero numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number that should be compared with zero - represented by big_int pointer of pseudo hexadecimal number. 
* @param len - length of the number(a).
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero_fixed_length(big_int* a, unsigned long int len);

/** 
* This function function compares number with zero in long arithmetic,
* long arithmetic means you can compare with zero numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number that should be compared with zero - represented by big_int pointer of pseudo hexadecimal number. 
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero(big_int* a);

/** 
* This function evaluates a number in long arithmetic by determining whether it is odd or not,
* long arithmetic means you can evaluate numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number that should be compared with zero - represented by big_int pointer of pseudo hexadecimal number. 
* @return bool - the result of the evaluation of a.
*/
bool is_odd(big_int* a);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function creates the number(pointer of big_int structure that represents
* pseudo hexadecimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param sign - a short int value, that represents sign, value is predefined(more in defines.h). 
* @param str_hex - a string that contains number in a pseudo hexadecimal format.
* @return big_int* - created number of big_int structure pointer.
*/
big_int* create_bigint_object(short int sign, char* str_hex);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function creates the copy of number(pointer of big_int structure that represents
* pseudo hexadecimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number that copy should be created, number is represented by big_int structure pointer. 
* @return big_int* - number a copy, also represented as pseudo hexadecimal number.
*/
big_int* create_bigint_copy(big_int* a);

/**
* <------ It is important to remember that the number is expressed in pseudo hexadecimal number. ------>
* <------          needs to be converted using functions from the converter.h                    ------>
* 
* 
* This function destroys the number(pointer of big_int structure that represents
* pseudo hexadecimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number whose memory must be released(structure memory free). 
*/
void destroy_bigint_object(big_int *a);

#endif