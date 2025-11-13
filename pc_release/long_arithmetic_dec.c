/**
* @file long_arithmetic_dec.c
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
#include "defines.h"
#include "utils.h"

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
big_dec* plus_dec(big_dec* a, big_dec* b) {
    char *first, *second;
    big_dec *reverse_a, *reverse_b, *plus_a_b, *reverse_plus_a_b, *minus_b_rev_a,
            *minus_a_rev_b, *result;
    int len, difference, shift, i, remainder;

    if (!a || !b) {
        return NULL;
    }

    if (a->sign == SIGN_NEGATIVE) {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_a = reverse_dec(a);
            reverse_b = reverse_dec(b);
            plus_a_b = plus_dec(reverse_a, reverse_b);
            reverse_plus_a_b = reverse_dec(plus_a_b);
            destroy_bigdec_object(reverse_a);
            destroy_bigdec_object(reverse_b);
            destroy_bigdec_object(plus_a_b);
            return reverse_plus_a_b;
        } 
        else {
            reverse_a = reverse_dec(a);
            minus_b_rev_a = minus_dec(b, reverse_a);
            destroy_bigdec_object(reverse_a);
            return minus_b_rev_a;
        } 
    }
    else {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_b = reverse_dec(b);
            minus_a_rev_b = minus_dec(a, reverse_b);
            destroy_bigdec_object(reverse_b);
            return minus_a_rev_b;
        }
    }

    len = MAX(strlen(a->str_dec), strlen(b->str_dec));

    first = (char*)malloc((len + 2) * sizeof(char));
    if (!first) {
        return NULL;
    }

    second = (char*)malloc((len + 2) * sizeof(char));
    if (!second) {
        return NULL;
    }

    strcpy(first, a->str_dec);
    strcpy(second, b->str_dec);
    first = str_reverse(first);
    second = str_reverse(second);

    difference = abs((int)strlen(b->str_dec) - (int)strlen(a->str_dec));
    shift = MIN(strlen(a->str_dec), strlen(b->str_dec));
    if (strlen(a->str_dec) < strlen(b->str_dec)) {
        for (i = 0; i < difference; i++) {
            first[i + shift] = CHAR_ZERO;
        }
    
        first[shift + difference] = CHAR_END_LINE;
    } else 
    if (strlen(a->str_dec) > strlen(b->str_dec)) {
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

        remainder = (first[i] - CHAR_ZERO) >= ARITHMETIC_BASE_DEC;

        if (remainder) {
            first[i] -= ARITHMETIC_BASE_DEC;
        } 
    }
 
    first = str_reverse(first);
    first = remove_leading_zeros(first);

    free(second);
    result = create_bigdec_object(SIGN_POSITIVE, first);

    return result;
}

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
big_dec* minus_dec(big_dec* a, big_dec* b) {
    char* first, *second;
    big_dec *reverse_b, *plus_a_rev_b, *reverse_a, *plus_rev_a_b,
            *reverse_plus_rev_a_b, *minus_b_a, *rev_minus_b_a, *result;
    int len, difference, shift, i, remainder;

    if (!a || !b) {
        return NULL;
    }

    if (b->sign == SIGN_NEGATIVE) {
        reverse_b = reverse_dec(b);
        plus_a_rev_b = plus_dec(a, reverse_b);
        destroy_bigdec_object(reverse_b);
        return plus_a_rev_b;
    } else
    if (a->sign == SIGN_NEGATIVE) {
        reverse_a = reverse_dec(a);
        plus_rev_a_b = plus_dec(reverse_a, b);
        reverse_plus_rev_a_b = reverse_dec(plus_rev_a_b);
        destroy_bigdec_object(reverse_a);
        destroy_bigdec_object(plus_rev_a_b);

        return reverse_plus_rev_a_b;
    } else
    if (is_less_dec(a, b)) {
        minus_b_a = minus_dec(b, a);
        rev_minus_b_a = reverse_dec(minus_b_a);
        destroy_bigdec_object(minus_b_a);
        return rev_minus_b_a;
    }

    len = MAX(strlen(a->str_dec), strlen(b->str_dec));

    first = (char*)malloc((len + 2) * sizeof(char));
    if (!first) {
        return NULL;
    }
    
    second = (char*)malloc((len + 2) * sizeof(char));
    if (!second) {
        return NULL;
    }

    strcpy(first, a->str_dec);
    strcpy(second, b->str_dec);
    first = str_reverse(first);
    second = str_reverse(second);

    difference = abs((int)strlen(b->str_dec) - (int)strlen(a->str_dec));
    shift = MIN(strlen(a->str_dec), strlen(b->str_dec));
    if (strlen(a->str_dec) < strlen(b->str_dec)) {
        for (i = 0; i < difference; i++) {
            first[i + shift] = CHAR_ZERO;
        }
    
        first[shift + difference] = CHAR_END_LINE;
    } else 
    if (strlen(a->str_dec) > strlen(b->str_dec)) {
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
            first[i] += ARITHMETIC_BASE_DEC;
        }
    }

    first = str_reverse(first);
    first = remove_leading_zeros(first);

    free(second);
    result = create_bigdec_object(SIGN_POSITIVE, first);

    return result;
}

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
big_dec* multiply_dec(big_dec* a, big_dec* b) {
    char *first, *second, *result; 
    big_dec *return_answer;
    int shift, carry, cur;
    unsigned long int i, j, len, first_len, second_len, difference;
    if (!a || !b) {
        return NULL;
    }

    len = MAX(strlen(a->str_dec), strlen(b->str_dec));

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

    strcpy(first, a->str_dec);
    strcpy(second, b->str_dec);
    first = str_reverse(first);
    second = str_reverse(second);

    for (i = 0; i < 2 * (len + 2); i++) {
        result[i] = CHAR_ZERO;
    }
    
    result[2 * (len + 2)] = CHAR_END_LINE;

    difference = abs((int)strlen(b->str_dec) - (int)strlen(a->str_dec));
    shift = MIN(strlen(a->str_dec), strlen(b->str_dec));
    if (strlen(a->str_dec) < strlen(b->str_dec)) {
        for (i = 0; i < difference; i++) {
            first[i + shift] = CHAR_ZERO;
        }
    
        first[shift + difference] = CHAR_END_LINE;
    } else 
    if (strlen(a->str_dec) > strlen(b->str_dec)) {
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

            result[i + j] = (cur % ARITHMETIC_BASE_DEC) + CHAR_ZERO;

            carry = cur / ARITHMETIC_BASE_DEC;
        }
    }

    result = str_reverse(result);
    result = remove_leading_zeros(result);

    free(first);
    free(second);
    return_answer = create_bigdec_object(((a->sign == SIGN_NEGATIVE) == (b->sign == SIGN_NEGATIVE)) ? SIGN_POSITIVE : SIGN_NEGATIVE, result);

    return return_answer;
}

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
big_dec* divide_div_dec(big_dec* a, big_dec* b) {
    char *p_digit_zero, *result, *work_string, *remainder_string,
         *substring, *space_str, *temp_str;
    big_dec *temp, *multiplier, *multiply_result, *minus_result, *return_answer;
    int len, count, digit;
    unsigned long int j, counter;

    if (!a || !b) {
        return NULL;
    }
    
    if (is_zero_dec(b)) { 
        puts("Division by zero!");
        return NULL;
    }
 
    if (is_less_abs_dec(a, b)) {
        p_digit_zero = (char*)malloc(2 * sizeof(char));
        if (!p_digit_zero) {
            return NULL;
        }

    	strcpy(p_digit_zero, STRING_ZERO);
        return create_bigdec_object(SIGN_POSITIVE, p_digit_zero);
    }

    len = MAX(strlen(a->str_dec), strlen(b->str_dec));

    result = (char*)malloc(1 * sizeof(char));
    if (!result) { 
        return NULL;
    }

	strcpy(result, "");

    work_string = (char*)malloc((strlen(a->str_dec) + 1) * sizeof(char));
    if (!work_string) { 
        return NULL;
    }

    strcpy(work_string, a->str_dec);

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
        
        temp = create_bigdec_object(SIGN_POSITIVE, substring);
        if (is_zero_dec(temp)) {
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
        if (!is_less_abs_dec(temp, b)) {
            for (digit = 9; digit > 0; digit--) {
                count = get_digits_from_int(digit);
                temp_str = (char*)malloc((count + 1) * sizeof(char));
                if (!temp_str) { 
                    return NULL;
                }
    
                int_to_char(digit, temp_str);
                temp_str[count] = CHAR_END_LINE;

                multiplier = create_bigdec_object(SIGN_POSITIVE, temp_str);
                multiply_result = multiply_dec(b, multiplier);
                multiply_result->sign = SIGN_POSITIVE;
                if (is_less_abs_dec(multiply_result, temp) || is_equal_abs_dec(multiply_result, temp)) {
                    len = strlen(result) + 2;
                    result = (char*)realloc(result, len);
                    result[len - 2] = digit + CHAR_ZERO;
                    result[len - 1] = CHAR_END_LINE;

                    minus_result = minus_dec(temp, multiply_result);
                    space_str = (char*)malloc((strlen(substring) + 1) * sizeof(char));
                    if (!space_str) { 
                        return NULL;
                    }

                    for (j = 0; j < strlen(substring); j++) {
                        work_string[j] = space_str[j] = CHAR_SPACE;
                    }
                    
                    space_str[strlen(substring)] = CHAR_END_LINE;

                    remainder_string = (char*)realloc(remainder_string, strlen(minus_result->str_dec) + 1);
                    strcpy(remainder_string, minus_result->str_dec);

                    if (!is_zero_dec(minus_result)) {
                        work_string = str_replace(work_string, space_str, minus_result->str_dec);
                    } else {
                        work_string = str_replace(work_string, space_str, "");
                    }
                    
                    free(space_str);

                    counter = 0;

                    destroy_bigdec_object(minus_result);
                    digit = 0;
                }
                
                destroy_bigdec_object(multiply_result);
                destroy_bigdec_object(multiplier);
            }
        } else {
            if (strstr(remainder_string, substring) == NULL) {
                len = strlen(result) + 2;
                result = (char*)realloc(result, len);
                result[len - 2] = CHAR_ZERO;
                result[len - 1] = CHAR_END_LINE;
            }
        }

        destroy_bigdec_object(temp);
        ++counter;
    }

    free(remainder_string);
    free(work_string);

    result = remove_leading_zeros(result);
    return_answer = create_bigdec_object(((a->sign == SIGN_NEGATIVE) == (b->sign == SIGN_NEGATIVE)) ? SIGN_POSITIVE : SIGN_NEGATIVE, result);

    return return_answer;
}

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
big_dec* reverse_dec(big_dec* a) {
    big_dec* b;

    if (!a) { 
        return NULL;
    }

    b = create_bigdec_copy(a);
    if (!b) {
        return NULL;
    }

    if (is_zero_dec(b)) {
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
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of modulo comparing the number a to number b.
*/
bool is_equal_abs_dec(big_dec* a, big_dec* b) {
    big_dec* x = create_bigdec_copy(a);
    big_dec* y = create_bigdec_copy(b);
    bool result;

    x->sign = SIGN_POSITIVE;
    y->sign = SIGN_POSITIVE;

    result = is_equal_dec(x, y);

    destroy_bigdec_object(x);
    destroy_bigdec_object(y);
 
    return result;
}

/** 
* This function compares on equalation of two numbers in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal_dec(big_dec* a, big_dec* b) {
    unsigned long int i;

    if (!a || !b) {
        return false;
    }

    if ( (a->sign == SIGN_NEGATIVE) != (b->sign == SIGN_NEGATIVE) ) {
        return false;
    }
    
    if (is_zero_dec(a) && is_zero_dec(b)) {
        return true;
    }

    if (!strlen(a->str_dec)) {
        if (is_zero_dec(b)) {
            return true;
        }
        else {
            return false;
        } 
    }

    if (!strlen(b->str_dec)) {
        if (is_zero_dec(a)) {
            return true;
        }
        else { 
            return false;
        }
    }
    
    if (strlen(a->str_dec) != strlen(b->str_dec)) {
        return false;
    }

    for (i = 0; i < strlen(a->str_dec); i++) {
        if (a->str_dec[i] != b->str_dec[i]) {
            return false;
        }
    }

    return true;
}

/** 
* This function compare whether the second number modulo is greater than the first number modulo in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of comparing modulo of the number a to the number b
* (is number a modulo less than number b modulo).
*/
bool is_less_abs_dec(big_dec* a, big_dec* b) {
    big_dec *x = create_bigdec_copy(a);
    big_dec *y = create_bigdec_copy(b);
    bool result;

    x->sign = SIGN_POSITIVE;
    y->sign = SIGN_POSITIVE;

    result = is_less_dec(x, y);
    
    destroy_bigdec_object(x);
    destroy_bigdec_object(y);
 
    return result;
}

/** 
* This function compare whether the second number is greater than the first number in long arithmetic,
* long arithmetic means you can compare numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less_dec(big_dec* a, big_dec* b) {
    big_dec *reverse_a, *reverse_b;
    bool res;
    unsigned long int i, len_a, len_b;
    
    if (!a || !b) {
        return false;
    }

    if (is_equal_dec(a, b)) {
        return false;
    }

    if (a->sign == SIGN_NEGATIVE) {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_b = reverse_dec(b);
            reverse_a = reverse_dec(a);
            res = is_less_dec(reverse_b, reverse_a);
            destroy_bigdec_object(reverse_a);
            destroy_bigdec_object(reverse_b);
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
            len_a = strlen(a->str_dec);
            len_b = strlen(b->str_dec);
            if (len_a != len_b) {
                return len_a < len_b;
            }
            else {
                for (i = 0; i < len_a; i++) {
                    if (a->str_dec[i] != b->str_dec[i]) {
                        return (a->str_dec[i] < b->str_dec[i]);
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
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @param len_a - length of the first number(a).
* @param len_b - length of the second number(b).
* @return bool - the result of comparing of the number a to the number b (is number a less than number b).
*/
bool is_less_fixed_length_dec(big_dec* a, big_dec* b, unsigned long int len_a, unsigned long int len_b) {
    big_dec *reverse_a, *reverse_b;
    bool res;
    unsigned long int i;
    
    if (!a || !b) {
        return false;
    }

    if (is_equal_fixed_length_dec(a, b, len_a, len_b)) {
        return false;
    }

    if (a->sign == SIGN_NEGATIVE) {
        if (b->sign == SIGN_NEGATIVE) {
            reverse_b = reverse_dec(b);
            reverse_a = reverse_dec(a);
            res = is_less_fixed_length_dec(reverse_b, reverse_a, len_a, len_b);
            destroy_bigdec_object(reverse_a);
            destroy_bigdec_object(reverse_b);
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
                    if (a->str_dec[i] != b->str_dec[i]) {
                        return (a->str_dec[i] < b->str_dec[i]);
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
* standard data type, numbers are expressed as decimal numbers.
* @param a - first number that is represented by big_dec pointer of decimal number. 
* @param b - second number that is represented by big_dec pointer of decimal number.
* @param len_a - length of the first number(a).
* @param len_b - length of the second number(b).
* @return bool - the result of comparing the number a to number b.
*/
bool is_equal_fixed_length_dec(big_dec* a, big_dec* b, unsigned long int len_a, unsigned long int len_b) {
    unsigned long int i;

    if (!a || !b) {
        return false;
    }

    if ( (a->sign == SIGN_NEGATIVE) != (b->sign == SIGN_NEGATIVE) ) {
        return false;
    }
    
    if (is_zero_fixed_length_dec(a, len_a) && is_zero_fixed_length_dec(b, len_b)) {
        return true;
    }

    if (!len_a) {
        if (is_zero_fixed_length_dec(b, len_b)) {
            return true;
        }
        else {
            return false;
        } 
    }

    if (!len_b) {
        if (is_zero_fixed_length_dec(a, len_a)) {
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
        if (a->str_dec[i] != b->str_dec[i]) {
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
* standard data type, numbers are expressed as decimal numbers.
* @param a - number that should be compared with zero - represented by big_dec pointer of decimal number. 
* @param len - length of the number(a).
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero_fixed_length_dec(big_dec* a, unsigned long int len) {
    if (!a) {
        return false;
    }

    if (!len || (len == 1 && a->str_dec[0] == CHAR_ZERO)) {
        return true;
    }

    return false;
}

/** 
* This function function compares number with zero in long arithmetic,
* long arithmetic means you can compare with zero numbers that are beyond the reach of any available
* standard data type, numbers are expressed as decimal numbers.
* @param a - number that should be compared with zero - represented by big_dec pointer of decimal number. 
* @return bool - the result of comparing the number a with zero.
*/
bool is_zero_dec(big_dec* a) {
    if (!a) {
        return false;
    }

    if (!strlen(a->str_dec) || (strlen(a->str_dec) == 1 && a->str_dec[0] == CHAR_ZERO)) {
        return true;
    }

    return false;
}

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
big_dec* create_bigdec_object(short int sign, char* str_dec) {
    big_dec* temp = (big_dec*)malloc(sizeof(big_dec));
    
    if (!temp) {
        return NULL;
    }

    temp->sign = sign;
    temp->str_dec = str_dec;

    if (is_zero_dec(temp)) {
        temp->sign = SIGN_POSITIVE;
    }

    return temp;
}

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
big_dec* create_bigdec_copy(big_dec* a) {
    char *temp_str;
    big_dec *result; 

    if (!a) {
        return NULL;
    }

    temp_str = (char*)malloc((strlen(a->str_dec) + 1) * sizeof(char));
    if (!temp_str) {
        return NULL;
    }
    
    strcpy(temp_str, a->str_dec);
    result = create_bigdec_object(a->sign, temp_str);

    return result;
}

/**
* <------ It is important to remember that the number is expressed in decimal format. ------>
* 
* 
* This function destroys the number(pointer of big_dec structure that represents
* decimal number) in long arithmetic, long arithmetic means that numbers are beyond the reach
* of any available standard data type, numbers are expressed as decimal numbers.
* @param a - number whose memory must be released(structure memory free). 
*/
void destroy_bigdec_object(big_dec *a) {
    if (a->str_dec) {
        free(a->str_dec);
    }
 
    if (!a) {
        return;
    }
 
    free(a);
    a = NULL;
}