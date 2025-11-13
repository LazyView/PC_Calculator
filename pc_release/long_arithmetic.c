/**
* @file long_arithmetic.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#include "long_arithmetic_dec.h"
#include "long_arithmetic.h"
#include "converter.h"
#include "defines.h"
#include "utils.h"

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
big_int* plus(big_int* a, big_int* b) {
    char *first, *second;
    big_int *reverse_a, *reverse_b, *plus_a_b, *reverse_plus_a_b, *minus_b_rev_a,
            *minus_a_rev_b, *result;
    int len, difference, shift, i, remainder;

    if (!a || !b) {
        return NULL;
    }

    if (a->sign == SIGN_NEGATIVE) {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_a = reverse(a);
            reverse_b = reverse(b);
            plus_a_b = plus(reverse_a, reverse_b);
            reverse_plus_a_b = reverse(plus_a_b);
            destroy_bigint_object(reverse_a);
            destroy_bigint_object(reverse_b);
            destroy_bigint_object(plus_a_b);
            return reverse_plus_a_b;
        } 
        else {
            reverse_a = reverse(a); 
            minus_b_rev_a = minus(b, reverse_a);
            destroy_bigint_object(reverse_a);
            return minus_b_rev_a;
        } 
    }
    else { 
        if (b->sign == SIGN_NEGATIVE) {
            reverse_b = reverse(b);
            minus_a_rev_b = minus(a, reverse_b);
            destroy_bigint_object(reverse_b);
            return minus_a_rev_b;
        }
    }

    len = MAX(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));

    first = (char*)malloc((len + 2) * sizeof(char));
    if (!first) {
        return NULL;
    }

    second = (char*)malloc((len + 2) * sizeof(char));
    if (!second) {
        return NULL;
    }

    strcpy(first, a->str_pseudo_hex);
    strcpy(second, b->str_pseudo_hex);
    first = str_reverse(first);
    second = str_reverse(second);

    difference = abs((int)strlen(b->str_pseudo_hex) - (int)strlen(a->str_pseudo_hex));
    shift = MIN(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));
    if (strlen(a->str_pseudo_hex) < strlen(b->str_pseudo_hex)) {
        for (i = 0; i < difference; i++) {
            first[i + shift] = CHAR_ZERO;
        }
    
        first[shift + difference] = CHAR_END_LINE;
    } else 
    if (strlen(a->str_pseudo_hex) > strlen(b->str_pseudo_hex)) {
        for (i = 0; i < difference; i++) {
            second[i + shift] = CHAR_ZERO;
        }
    
        second[shift + difference] = CHAR_END_LINE;
    }
    
    first[len] = second[len] = CHAR_ZERO;
    first[len + 1] = second[len + 1] = CHAR_END_LINE;
    remainder = 0;
    for (i = 0; i < len || remainder; i++) {
        first[i] += remainder + (i < len ? (second[i] - CHAR_ZERO) : 0);

        remainder = (first[i] - CHAR_ZERO) >= ARITHMETIC_BASE_HEX;

        if (remainder) {
            first[i] -= ARITHMETIC_BASE_HEX;
        } 
    }
 
    first = str_reverse(first);
    first = remove_leading_zeros(first);

    free(second);
    result = create_bigint_object(SIGN_POSITIVE, first);

    return result;
}

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
big_int* minus(big_int* a, big_int* b) {
    char* first, *second;
    big_int *reverse_b, *plus_a_rev_b, *reverse_a, *plus_rev_a_b,
            *reverse_plus_rev_a_b, *minus_b_a, *rev_minus_b_a, *result;
    int len, difference, shift, i, remainder;

    if (!a || !b) {
        return NULL;
    }

    if (b->sign == SIGN_NEGATIVE) {
        reverse_b = reverse(b);
        plus_a_rev_b = plus(a, reverse_b);
        destroy_bigint_object(reverse_b);
        return plus_a_rev_b;
    } else
    if (a->sign == SIGN_NEGATIVE) {
        reverse_a = reverse(a);
        plus_rev_a_b = plus(reverse_a, b);
        reverse_plus_rev_a_b = reverse(plus_rev_a_b);
        destroy_bigint_object(reverse_a);
        destroy_bigint_object(plus_rev_a_b);

        return reverse_plus_rev_a_b;
    } else
    if (is_less(a, b)) {
        minus_b_a = minus(b, a);
        rev_minus_b_a = reverse(minus_b_a);
        destroy_bigint_object(minus_b_a);
        return rev_minus_b_a;
    }

    len = MAX(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));

    first = (char*)malloc((len + 2) * sizeof(char));
    if (!first) {
        return NULL;
    }
    
    second = (char*)malloc((len + 2) * sizeof(char));
    if (!second) {
        return NULL;
    }

    strcpy(first, a->str_pseudo_hex);
    strcpy(second, b->str_pseudo_hex);
    first = str_reverse(first);
    second = str_reverse(second);

    difference = abs((int)strlen(b->str_pseudo_hex) - (int)strlen(a->str_pseudo_hex));
    shift = MIN(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));
    if (strlen(a->str_pseudo_hex) < strlen(b->str_pseudo_hex)) {
        for (i = 0; i < difference; i++) {
            first[i + shift] = CHAR_ZERO;
        }
    
        first[shift + difference] = CHAR_END_LINE;
    } else 
    if (strlen(a->str_pseudo_hex) > strlen(b->str_pseudo_hex)) {
        for (i = 0; i < difference; i++) {
            second[i + shift] = CHAR_ZERO;
        }
    
        second[shift + difference] = CHAR_END_LINE;
    }
    
    first[len] = second[len] = CHAR_ZERO;
    first[len + 1] = second[len + 1] = CHAR_END_LINE;
    
    remainder = 0;
    for (i = 0; i < len || remainder; i++) {
        first[i] -= remainder + (i < len ? (second[i] - CHAR_ZERO) : 0);

        remainder = (first[i] - CHAR_ZERO) < 0;

        if (remainder) {
            first[i] += ARITHMETIC_BASE_HEX;
        }
    }

    first = str_reverse(first);
    first = remove_leading_zeros(first);

    free(second);
    result = create_bigint_object(SIGN_POSITIVE, first);

    return result;
}

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
big_int* multiply(big_int* a, big_int* b) {
    char *first, *second, *result; 
    big_int *return_answer;
    int shift, carry, cur;
    unsigned long int i, j, len, first_len, second_len, difference;
    if (!a || !b) {
        return NULL;
    }

    len = MAX(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));

    first = (char*)malloc((len + 2) * sizeof(char));
    if (!first) {
        return NULL;
    }

    second = (char*)malloc((len + 2) * sizeof(char));
    if (!second) {
        return NULL;
    }
    
    result = (char*)malloc(((2 * (len + 2)) + 1) * sizeof(char));
    if (!result) {
        return NULL;
    }

    strcpy(first, a->str_pseudo_hex);
    strcpy(second, b->str_pseudo_hex);
    first = str_reverse(first);
    second = str_reverse(second);

    for (i = 0; i < 2 * (len + 2); i++) {
        result[i] = CHAR_ZERO;
    }
    
    result[2 * (len + 2)] = CHAR_END_LINE;

    difference = abs((int)strlen(b->str_pseudo_hex) - (int)strlen(a->str_pseudo_hex));
    shift = MIN(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));
    if (strlen(a->str_pseudo_hex) < strlen(b->str_pseudo_hex)) {
        for (i = 0; i < difference; i++) {
            first[i + shift] = CHAR_ZERO;
        }
    
        first[shift + difference] = CHAR_END_LINE;
    } else 
    if (strlen(a->str_pseudo_hex) > strlen(b->str_pseudo_hex)) {
        for (i = 0; i < difference; i++) {
            second[i + shift] = CHAR_ZERO;
        }
    
        second[shift + difference] = CHAR_END_LINE;
    }
    first[len] = second[len] = CHAR_ZERO;
    first[len + 1] = second[len + 1] = CHAR_END_LINE;
    
    first_len = strlen(first);
    second_len = strlen(second);
    for (i = 0; i < first_len; i++) {
        carry = 0;
        for (j = 0; j < second_len || carry; j++) {
            cur = (result[i + j] - CHAR_ZERO) + (first[i] - CHAR_ZERO) * (j < second_len ? (second[j] - CHAR_ZERO) : 0) + carry;

            result[i + j] = (cur % ARITHMETIC_BASE_HEX) + CHAR_ZERO;

            carry = cur / ARITHMETIC_BASE_HEX;
        }
    }

    result = str_reverse(result);
    result = remove_leading_zeros(result);

    free(first);
    free(second);
    return_answer = create_bigint_object(((a->sign == SIGN_NEGATIVE) == (b->sign == SIGN_NEGATIVE)) ? SIGN_POSITIVE : SIGN_NEGATIVE, result);

    return return_answer;
}

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
big_int* divide_div(big_int* a, big_int* b) {
    char *p_digit_zero, *result, *work_string, *remainder_string,
         *substring, *space_str, *temp_str;
    big_int *temp, *multiplier, *multiply_result, *minus_result, *return_answer;
    int len, count, digit;
    unsigned long int j, counter;

    if (!a || !b) {
        return NULL;
    }
    
    if (is_zero(b)) { 
        puts("Division by zero!");
        return NULL;
    }
 
    if (is_less_abs(a, b)) {
        p_digit_zero = (char*)malloc(2 * sizeof(char));
        if (!p_digit_zero) {
            return NULL;
        }

    	strcpy(p_digit_zero, STRING_ZERO);
        return create_bigint_object(SIGN_POSITIVE, p_digit_zero);
    }

    len = MAX(strlen(a->str_pseudo_hex), strlen(b->str_pseudo_hex));

    result = (char*)malloc(1 * sizeof(char));
    if (!result) { 
        return NULL;
    }

	strcpy(result, "");

    work_string = (char*)malloc((strlen(a->str_pseudo_hex) + 1) * sizeof(char));
    if (!work_string) { 
        return NULL;
    }

    strcpy(work_string, a->str_pseudo_hex);

    remainder_string = (char*)malloc(sizeof(char));
    if (!remainder_string) { 
        return NULL;
    }

	strcpy(remainder_string, "");

    counter = 1;
    while (counter - 1 < strlen(work_string)) {
        substring = (char*)malloc((counter + 2) * sizeof(char));
        strncpy(substring, work_string, counter);
        substring[counter] = CHAR_END_LINE;
        
        temp = create_bigint_object(SIGN_POSITIVE, substring);
        if (is_zero(temp)) {
            len = strlen(result) + 2;
            result = (char*)realloc(result, len);
            result[len - 2] = CHAR_ZERO;
            result[len - 1] = CHAR_END_LINE;

            space_str = (char*)malloc((strlen(substring) + 1) * sizeof(char));
            if (!space_str) { 
                return NULL;
            }
    
            for (j = 0; j < strlen(substring); j++) {
                work_string[j] = space_str[j] = CHAR_SPACE;
            }
    
            space_str[strlen(substring)] = CHAR_END_LINE;

            work_string = str_replace(work_string, space_str, "");
            counter = 0;

            free(space_str);
        } else
        if (!is_less_abs(temp, b)) {
            for (digit = 15; digit > 0; digit--) {
                count = 1;
                temp_str = (char*)malloc((count + 1) * sizeof(char));
                if (!temp_str) { 
                    return NULL;
                }
                temp_str[count - 1] = digit + CHAR_ZERO;
                temp_str[count] = CHAR_END_LINE;

                multiplier = create_bigint_object(SIGN_POSITIVE, temp_str);
                multiply_result = multiply(b, multiplier);
                multiply_result->sign = SIGN_POSITIVE;
                if (is_less_abs(multiply_result, temp) || is_equal_abs(multiply_result, temp)) {
                    len = strlen(result) + 2;
                    result = (char*)realloc(result, len);
                    result[len - 2] = digit + CHAR_ZERO;
                    result[len - 1] = CHAR_END_LINE;

                    minus_result = minus(temp, multiply_result);
                    space_str = (char*)malloc((strlen(substring) + 1) * sizeof(char));
                    if (!space_str) { 
                        return NULL;
                    }

                    for (j = 0; j < strlen(substring); j++) {
                        work_string[j] = space_str[j] = CHAR_SPACE;
                    }
                    
                    space_str[strlen(substring)] = CHAR_END_LINE;

                    remainder_string = (char*)realloc(remainder_string, strlen(minus_result->str_pseudo_hex) + 1);
                    strcpy(remainder_string, minus_result->str_pseudo_hex);

                    if (!is_zero(minus_result)) {
                        work_string = str_replace(work_string, space_str, minus_result->str_pseudo_hex);
                    } else {
                        work_string = str_replace(work_string, space_str, "");
                    }
                    
                    free(space_str);

                    counter = 0;

                    destroy_bigint_object(minus_result);
                    digit = 0;
                }
                
                destroy_bigint_object(multiply_result);
                destroy_bigint_object(multiplier);
            }
        } else {
            if (strstr(remainder_string, substring) == NULL) {
                len = strlen(result) + 2;
                result = (char*)realloc(result, len);
                result[len - 2] = CHAR_ZERO;
                result[len - 1] = CHAR_END_LINE;
            }
        }

        destroy_bigint_object(temp);
        ++counter;
    }

    free(remainder_string);
    free(work_string);

    result = remove_leading_zeros(result);
    return_answer = create_bigint_object(((a->sign == SIGN_NEGATIVE) == (b->sign == SIGN_NEGATIVE)) ? SIGN_POSITIVE : SIGN_NEGATIVE, result);

    return return_answer;
}

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
big_int* divide_mod(big_int* a, big_int* b) {
    big_int *divide_result, *multiply_result, *result;
    
    if (!a || !b) { 
        return NULL;
    }

    if (is_zero(b)) { 
        puts("Division by zero!");
        return NULL;
    }

    divide_result = divide_div(a, b);
    multiply_result = multiply(divide_result, b);

    result = minus(a, multiply_result);

    destroy_bigint_object(divide_result);
    destroy_bigint_object(multiply_result);
    
    return result;
}

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
big_int* prod_tree(big_int* left, big_int* right) {
    char *p_digit_one, *p_digit_two;
    big_int *result, *one, *two, *minus_result, *plus_result, *middle, *middle_plus_one, *left_rec, *right_rec;

    p_digit_one = (char*)malloc(2 * sizeof(char));
    if (!p_digit_one) {
        return NULL;
    }
    strcpy(p_digit_one, STRING_ONE);
    one = create_bigint_object(SIGN_POSITIVE, p_digit_one);

    if (is_less(right, left)) {
        return one;
    }

    p_digit_two = (char*)malloc(2 * sizeof(char));
    if (!p_digit_two) {
        destroy_bigint_object(one);
        return NULL;
    }
	strcpy(p_digit_two, BASE_BIN);
    two = create_bigint_object(SIGN_POSITIVE, p_digit_two);

    if (is_equal(left, right)) {
        destroy_bigint_object(one);
        destroy_bigint_object(two);
        return create_bigint_copy(left);
    }

    minus_result = minus(right, left);
    if (is_equal(minus_result, one)) {
        destroy_bigint_object(one);
        destroy_bigint_object(two);
        destroy_bigint_object(minus_result);
        return multiply(left, right);
    }
    destroy_bigint_object(minus_result);

    plus_result = plus(left, right);
    middle = divide_div(plus_result, two);
    middle_plus_one = plus(middle, one);

    destroy_bigint_object(one);
    destroy_bigint_object(two);
    destroy_bigint_object(plus_result);

    left_rec = prod_tree(left, middle);
    right_rec = prod_tree(middle_plus_one, right);
    destroy_bigint_object(middle);
    destroy_bigint_object(middle_plus_one);

    result = multiply(left_rec, right_rec);

    destroy_bigint_object(left_rec);
    destroy_bigint_object(right_rec);

    return result;
}

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
big_int* factorial(big_int* a) {
    char *p_digit_one, *p_digit_two;
    big_int *result, *one, *two;
    
    if (a->sign == SIGN_NEGATIVE && !is_zero(a)) {
        puts("Input of factorial must not be negative!");
        return NULL;
    }

    p_digit_one = (char*)malloc(2 * sizeof(char));
    if (!p_digit_one) {
        return NULL;
    }
    strcpy(p_digit_one, STRING_ONE);
    one = create_bigint_object(SIGN_POSITIVE, p_digit_one);

    if (is_zero(a)) {
        return one;
    }

    p_digit_two = (char*)malloc(2 * sizeof(char));
    if (!p_digit_two) {
        destroy_bigint_object(one);
        return NULL;
    }
	strcpy(p_digit_two, BASE_BIN);
    two = create_bigint_object(SIGN_POSITIVE, p_digit_two);

    if (is_equal(a, one) || is_equal(a, two)) {
        destroy_bigint_object(one);
        destroy_bigint_object(two);
        return create_bigint_copy(a);
    }

    result = prod_tree(two, a);
    destroy_bigint_object(one);
    destroy_bigint_object(two);
    return result;
}

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
big_int* power(big_int* a, big_int* b) {
    char *p_digit_zero, *p_digit_one, *p_digit_two;
    big_int *zero, *temp, *b_copy, *two, *result,
            *result_copy, *temp_copy_1, *temp_copy,
            *b_copy_copy;

    if (!a || !b) {
        return NULL;
    }

    if (is_zero(a) && b->sign == SIGN_NEGATIVE) {
        /* puts("You cannot raise zero to a negative power!"); */
        puts("Syntax error!");
        return NULL;
    }
    
    if (is_zero(a) && is_zero(b)) {
        /* puts("You cannot raise zero to the power of zero!"); */
        puts("Syntax error!");
        return NULL;
    }
    
    if (is_zero(a) || b->sign == SIGN_NEGATIVE) {
        p_digit_zero = (char*)malloc(2 * sizeof(char));
        if (!p_digit_zero) {
            return NULL;
        }
	
    	strcpy(p_digit_zero, STRING_ZERO);
        zero = create_bigint_object(SIGN_POSITIVE, p_digit_zero);
    
        return zero;
    }

    temp = create_bigint_copy(a);
    b_copy = create_bigint_copy(b);

    p_digit_two = (char*)malloc(2 * sizeof(char));
    if (!p_digit_two) {
        return NULL;
    }
	
    strcpy(p_digit_two, BASE_BIN);
    two = create_bigint_object(SIGN_POSITIVE, p_digit_two);

    p_digit_one = (char*)malloc(2 * sizeof(char));
    if (!p_digit_one) {
        return NULL;
    }
	
    strcpy(p_digit_one, STRING_ONE);
    result = create_bigint_object(SIGN_POSITIVE, p_digit_one);

    while (!is_zero(b_copy)) {
        if (is_odd(b_copy)) {
            result_copy = multiply(result, temp);
            destroy_bigint_object(result);
            result = create_bigint_copy(result_copy);
            destroy_bigint_object(result_copy);
        }

        temp_copy_1 = create_bigint_copy(temp);
        temp_copy = multiply(temp, temp_copy_1);
        destroy_bigint_object(temp);
        temp = create_bigint_copy(temp_copy);
        destroy_bigint_object(temp_copy);
        destroy_bigint_object(temp_copy_1);

        b_copy_copy = divide_div(b_copy, two);
        destroy_bigint_object(b_copy);
        b_copy = create_bigint_copy(b_copy_copy);
        destroy_bigint_object(b_copy_copy);
    }

    destroy_bigint_object(b_copy);
    destroy_bigint_object(two);
    destroy_bigint_object(temp);
    
    return result;
}

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
big_int* reverse(big_int* a) {
    big_int* b;

    if (!a) { 
        return NULL;
    }

    b = create_bigint_copy(a);
    if (!b) {
        return NULL;
    }

    if (is_zero(b)) {
        return b;
    }

    if (a->sign == SIGN_POSITIVE) {
        b->sign = SIGN_NEGATIVE;
        return b;
    }

    b->sign = SIGN_POSITIVE;
 
    return b;
}

/**
* This function compares on equalation of two numbers modulo in long arithmetic,
* long arithmetic means you can compare modulo of numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of modulo comparing the number a to number b.
*/
bool is_equal_abs(big_int* a, big_int* b) {
    big_int* x = create_bigint_copy(a);
    big_int* y = create_bigint_copy(b);
    bool result;

    x->sign = SIGN_POSITIVE;
    y->sign = SIGN_POSITIVE;

    result = is_equal(x, y);

    destroy_bigint_object(x);
    destroy_bigint_object(y);
 
    return result;
}

/**
* This function compares on equalation of two numbers in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal(big_int* a, big_int* b) {
    unsigned long int i;

    if (!a || !b) {
        return false;
    }

    if ( (a->sign == SIGN_NEGATIVE) != (b->sign == SIGN_NEGATIVE) ) {
        return false;
    }
    
    if (is_zero(a) && is_zero(b)) {
        return true;
    }

    if (!strlen(a->str_pseudo_hex)) {
        if (is_zero(b)) {
            return true;
        }
        else {
            return false;
        } 
    }

    if (!strlen(b->str_pseudo_hex)) {
        if (is_zero(a)) {
            return true;
        }
        else { 
            return false;
        }
    }
    
    if (strlen(a->str_pseudo_hex) != strlen(b->str_pseudo_hex)) {
        return false;
    }

    for (i = 0; i < strlen(a->str_pseudo_hex); i++) {
        if (a->str_pseudo_hex[i] != b->str_pseudo_hex[i]) {
            return false;
        }
    }

    return true;
}

/**
* This function compare whether the second number modulo is greater than the first number modulo in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of comparing modulo of the number a to the number b
* (is number a modulo less than number b modulo).
*/
bool is_less_abs(big_int* a, big_int* b) {
    big_int *x = create_bigint_copy(a);
    big_int *y = create_bigint_copy(b);
    bool result;

    x->sign = SIGN_POSITIVE;
    y->sign = SIGN_POSITIVE;

    result = is_less(x, y);
    
    destroy_bigint_object(x);
    destroy_bigint_object(y);
 
    return result;
}

/**
* This function compare whether the second number is greater than the first number in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - first number that is represented by big_int pointer of pseudo hexadecimal number. 
* @param b - second number that is represented by big_int pointer of pseudo hexadecimal number.
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less(big_int* a, big_int* b) {
    big_int *reverse_a, *reverse_b;
    bool res;
    unsigned long int i, len_a, len_b;
    
    if (!a || !b) {
        return false;
    }

    if (is_equal(a, b)) {
        return false;
    }

    if (a->sign == SIGN_NEGATIVE) {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_b = reverse(b);
            reverse_a = reverse(a);
            res = is_less(reverse_b, reverse_a);
            destroy_bigint_object(reverse_a);
            destroy_bigint_object(reverse_b);
            return res;
        } 
        else {
            return true;
        }
    }
    else {
        if (b->sign == SIGN_NEGATIVE) {
            return false;
        }
        else {
            len_a = strlen(a->str_pseudo_hex);
            len_b = strlen(b->str_pseudo_hex);
            if (len_a != len_b) {
                return len_a < len_b;
            }
            else {
                for (i = 0; i < len_a; i++) {
                    if (a->str_pseudo_hex[i] != b->str_pseudo_hex[i]) {
                        return (a->str_pseudo_hex[i] < b->str_pseudo_hex[i]);
                    }
                }
                return false;
            }
        }
    }
}

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
bool is_less_fixed_length(big_int* a, big_int* b, unsigned long int len_a, unsigned long int len_b) {
    big_int *reverse_a, *reverse_b;
    bool res;
    unsigned long int i;
    
    if (!a || !b) {
        return false;
    }

    if (is_equal_fixed_length(a, b, len_a, len_b)) {
        return false;
    }

    if (a->sign == SIGN_NEGATIVE) {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_b = reverse(b);
            reverse_a = reverse(a);
            res = is_less_fixed_length(reverse_b, reverse_a, len_a, len_b);
            destroy_bigint_object(reverse_a);
            destroy_bigint_object(reverse_b);
            return res;
        } 
        else {
            return true;
        }
    }
    else {
        if (b->sign == SIGN_NEGATIVE) {
            return false;
        }
        else {
            if (len_a != len_b) {
                return len_a < len_b;
            }
            else {
                for (i = 0; i < len_a; i++) {
                    if (a->str_pseudo_hex[i] != b->str_pseudo_hex[i]) {
                        return (a->str_pseudo_hex[i] < b->str_pseudo_hex[i]);
                    }
                }
                return false;
            }
        }
    }
}

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
bool is_equal_fixed_length(big_int* a, big_int* b, unsigned long int len_a, unsigned long int len_b) {
    unsigned long int i;

    if (!a || !b) {
        return false;
    }

    if ( (a->sign == SIGN_NEGATIVE) != (b->sign == SIGN_NEGATIVE) ) {
        return false;
    }
    
    if (is_zero_fixed_length(a, len_a) && is_zero_fixed_length(b, len_b)) {
        return true;
    }

    if (!len_a) {
        if (is_zero_fixed_length(b, len_b)) {
            return true;
        }
        else {
            return false;
        } 
    }

    if (!len_b) {
        if (is_zero_fixed_length(a, len_a)) {
            return true;
        }
        else { 
            return false;
        }
    }
    
    if (len_a != len_b) {
        return false;
    }

    for (i = 0; i < len_a; i++) {
        if (a->str_pseudo_hex[i] != b->str_pseudo_hex[i]) {
            return false;
        }
    }

    return true;
}

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
bool is_zero_fixed_length(big_int* a, unsigned long int len) {
    if (!a) {
        return false;
    }

    if (!len || (len == 1 && a->str_pseudo_hex[0] == CHAR_ZERO)) {
        return true;
    }

    return false;
}

/**
* This function function compares number with zero in long arithmetic,
* long arithmetic means you can compare with zero numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number that should be compared with zero - represented by big_int pointer of pseudo hexadecimal number. 
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero(big_int* a) {
    if (!a) {
        return false;
    }

    if (!strlen(a->str_pseudo_hex) || (strlen(a->str_pseudo_hex) == 1 && a->str_pseudo_hex[0] == CHAR_ZERO)) {
        return true;
    }

    return false;
}

/**
* This function evaluates a number in long arithmetic by determining whether it is odd or not,
* long arithmetic means you can evaluate numbers that are beyond the reach of any available
* standard data type, numbers are expressed as pseudo hexadecimal numbers.
* @param a - number that should be compared with zero - represented by big_int pointer of pseudo hexadecimal number. 
* @return bool - the result of the evaluation of a.
*/
bool is_odd(big_int* a) {
    if (!a) {
        return false;
    }

    if (is_zero(a)) {
        return false;
    }

    if ((a->str_pseudo_hex[strlen(a->str_pseudo_hex) - 1] - CHAR_ZERO) & 1) {
        return true;
    }

    return false;
}

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
big_int* create_bigint_object(short int sign, char* str_hex) {
    big_int* temp = (big_int*)malloc(sizeof(big_int));
    
    if (!temp) {
        return NULL;
    }

    temp->sign = sign;
    temp->str_pseudo_hex = str_hex;

    if (is_zero(temp)) {
        temp->sign = SIGN_POSITIVE;
    }

    return temp;
}

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
big_int* create_bigint_copy(big_int* a) {
    char *temp_str;
    big_int *result; 

    if (!a) {
        return NULL;
    }

    temp_str = (char*)malloc((strlen(a->str_pseudo_hex) + 1) * sizeof(char));
    if (!temp_str) {
        return NULL;
    }
    
    strcpy(temp_str, a->str_pseudo_hex);
    result = create_bigint_object(a->sign, temp_str);

    return result;
}

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
void destroy_bigint_object(big_int *a) {
    if (a->str_pseudo_hex) {
        free(a->str_pseudo_hex);
    }
 
    if (!a) {
        return;
    }
 
    free(a);
    a = NULL;
}