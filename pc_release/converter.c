/**
* @file converter.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "long_arithmetic.h"
#include "long_arithmetic_dec.h"
#include "defines.h"
#include "utils.h"

/**
* @details This function will convert char pointer(c string) of binary number to big_dec pointer
* of sign decimal number.
* @param number - char pointer(c string) of number in binary format (the number must be verified
* to match the binary format with the third-party function).
* @return big_dec* - pointer of big_dec structure which represents sign decimal number.
*/
big_dec* bin_to_dec(char* number) {
    char *zero_str, *two_str, *power_result_str;
    big_dec *big_number, *two_object, *power_result, *big_number_copy, 
            *power_result_copy;
    int i;
    unsigned long int number_len;

    if (!number) {
        return NULL;
    }

    number_len = strlen(number);

    zero_str = (char*)malloc(2 * sizeof(char));
    if (!zero_str) {
        return NULL;
    }

    two_str = (char*)malloc(2 * sizeof(char));
    if (!two_str) {
        free(zero_str);
        return NULL;
    }

    power_result_str = (char*)malloc(2 * sizeof(char));
    if (!power_result_str) {
        free(zero_str);
        free(two_str);
        return NULL;
    }
    
    strcpy(zero_str, STRING_ZERO);
    strcpy(two_str, BASE_BIN);
    strcpy(power_result_str, STRING_ONE);
    
    big_number = create_bigdec_object(SIGN_POSITIVE, zero_str);
    two_object = create_bigdec_object(SIGN_POSITIVE, two_str);
    power_result = create_bigdec_object(POSITION_SIGN, power_result_str);

    for (i = number_len - 1; i > POSITION_SIGN; i--) {
        if (number[i] != CHAR_ZERO) {
            big_number_copy = plus_dec(big_number, power_result);
            destroy_bigdec_object(big_number);
            big_number = create_bigdec_copy(big_number_copy);
            destroy_bigdec_object(big_number_copy);
        }

        power_result_copy = plus_dec(power_result, power_result);
        destroy_bigdec_object(power_result);
        power_result = create_bigdec_copy(power_result_copy);
        destroy_bigdec_object(power_result_copy);
    }

    if (POSITION_SIGN == number_len - 1 && number[POSITION_SIGN] == CHAR_ONE) {
        big_number_copy = minus_dec(big_number, power_result);
        destroy_bigdec_object(big_number);
        big_number = create_bigdec_copy(big_number_copy);
        destroy_bigdec_object(big_number_copy); 
    } else 
    if (number[POSITION_SIGN] == CHAR_ONE) {
        big_number_copy = minus_dec(big_number, power_result);
        destroy_bigdec_object(big_number);
        big_number = create_bigdec_copy(big_number_copy);
        destroy_bigdec_object(big_number_copy);
    }

    destroy_bigdec_object(power_result);
    destroy_bigdec_object(two_object);

    return big_number;
}

/**
* @details This function will convert char pointer(c string) of hexadecimal number to big_dec pointer
* of sign decimal number.
* @param number - char pointer(c string) of number in hexadecimal format (the number must be verified
* to match the hexadecimal format with the third-party function).
* @return big_dec* - pointer of big_dec structure which represents sign decimal number.
*/
big_dec* hex_to_dec(char* number) {
    char *sixteen_str, *zero_str, *one_str;
    big_dec *sixteen_object, *big_number, *current_power, *big_result_digit, 
             *big_number_copy, *current_power_copy, *big_result_digit_copy;
    unsigned long int i, j, result_digit; 
    
    if (!number) {
        return NULL;
    }

    sixteen_str = (char*)malloc(3 * sizeof(char));
    if (!sixteen_str) {
        return NULL;
    }
    
    strcpy(sixteen_str, BASE_HEX);
    sixteen_object = create_bigdec_object(SIGN_POSITIVE, sixteen_str);

    zero_str = (char*)malloc(2 * sizeof(char));
    if (!zero_str) {
        destroy_bigdec_object(sixteen_object);
        return NULL;
    }
    
    strcpy(zero_str, STRING_ZERO);
    big_number = create_bigdec_object(SIGN_POSITIVE, zero_str);

    one_str = (char*)malloc(2 * sizeof(char));
    if (!one_str) {
        destroy_bigdec_object(sixteen_object);
        destroy_bigdec_object(big_number);
        return NULL;
    }
    
    strcpy(one_str, STRING_ONE);
    current_power = create_bigdec_object(SIGN_POSITIVE, one_str);

    for (i = strlen(number) - 1; i > POSITION_SIGN; i--) {
        result_digit = 0;
        for (j = 0; j < HEX_LENGTH_SYMBOLS_ALLOWED; j++) {
            if (number[i] == SYMBOLS_HEX_ALLOWED[j]) {
                result_digit = j;
                break;
            }
        }

        /*
        * We need to assign zero to this variable.
        * The most quickest decision is to use 'minus' function with one variable
        * (and this variable has short length as a perfect case).
        */
        big_result_digit = minus_dec(sixteen_object, sixteen_object);
        for (j = 0; j < result_digit; j++) {
            big_result_digit_copy = plus_dec(big_result_digit, current_power);
            destroy_bigdec_object(big_result_digit);
            big_result_digit = create_bigdec_copy(big_result_digit_copy);
            destroy_bigdec_object(big_result_digit_copy);
        }

        big_number_copy = plus_dec(big_number, big_result_digit);
        destroy_bigdec_object(big_number);
        big_number = create_bigdec_copy(big_number_copy);
        destroy_bigdec_object(big_number_copy);

        destroy_bigdec_object(big_result_digit);

        for (j = 0; j < HEX_FIGURE_LENGTH_IN_BINARY; j++) {
            current_power_copy = plus_dec(current_power, current_power);
            destroy_bigdec_object(current_power);
            current_power = create_bigdec_copy(current_power_copy);
            destroy_bigdec_object(current_power_copy);
        }
    }

    if (number[POSITION_SIGN] != CHAR_ZERO) {
        big_number_copy = minus_dec(big_number, current_power);
        destroy_bigdec_object(big_number);
        big_number = create_bigdec_copy(big_number_copy);
        destroy_bigdec_object(big_number_copy);
    }

    destroy_bigdec_object(sixteen_object);
    destroy_bigdec_object(current_power);

    return big_number;
}

/**
* @details This function will convert big_dec pointer of sign decimal number
* to char pointer(c string) of binary number. 
* @param a - pointer of big_dec structure which represents sign decimal number. 
* @return char* - number in binary format.
*/
char* dec_to_bin(big_dec* a) {
    char res_char;
    char *result_str, *one_str, *temp;
    big_dec *big_number, *abs_number, *current_power, *current_power_copy, *big_number_copy;
    big_dec **powers;
    int len;
    unsigned long int power_number, size, real_size, i, number_power_len;

    if (!a) {
        return NULL;
    }

    result_str = (char*)malloc(4 * sizeof(char));
    if (!result_str) { 
        return NULL;
    }

    power_number = 1;

    if (a->sign == SIGN_POSITIVE) {
        strcpy(result_str, PREFIX_BIN_POSITIVE);
        big_number = create_bigdec_copy(a);
    } else {
        strcpy(result_str, PREFIX_BIN_NEGATIVE);
        
        abs_number = create_bigdec_copy(a);
        abs_number->sign = SIGN_POSITIVE;

        one_str = (char*)malloc(2 * sizeof(char));
        if (!one_str) {
            free(result_str);
            destroy_bigdec_object(abs_number);
            return NULL;
        }
        strcpy(one_str, STRING_ONE);
        current_power = create_bigdec_object(SIGN_POSITIVE, one_str);

        number_power_len = strlen(abs_number->str_dec);
        while (is_less_fixed_length_dec(current_power, abs_number, strlen(current_power->str_dec), number_power_len)) {
            ++power_number;

            current_power_copy = plus_dec(current_power, current_power);
            destroy_bigdec_object(current_power);
            current_power = create_bigdec_copy(current_power_copy);
            destroy_bigdec_object(current_power_copy);
        }

        big_number = minus_dec(current_power, abs_number);

        destroy_bigdec_object(abs_number);
        destroy_bigdec_object(current_power);
    }

    one_str = (char*)malloc(2 * sizeof(char));
    if (!one_str) {
        free(result_str);
        destroy_bigdec_object(big_number);
        return NULL;
    }
    strcpy(one_str, STRING_ONE);
    current_power = create_bigdec_object(SIGN_POSITIVE, one_str);

    real_size = ALLOCATE_SIZE;
    size = 0;
    powers = (big_dec**)malloc(real_size * sizeof(big_dec*));
    if (!powers) {
        free(result_str);
        destroy_bigdec_object(big_number);
        destroy_bigdec_object(current_power);
        return NULL;
    }

    number_power_len = strlen(big_number->str_dec);
    while (!is_less_fixed_length_dec(big_number, current_power, number_power_len, strlen(current_power->str_dec))) {
        ++size;
        if (size > real_size) {
            real_size += ALLOCATE_SIZE;
            powers = (big_dec**)realloc(powers, real_size * sizeof(big_dec*));
            if (!powers) {
                free(result_str);
                destroy_bigdec_object(big_number);
                destroy_bigdec_object(current_power);
                return NULL;
            }
        }
        powers[size - 1] = create_bigdec_copy(current_power);

        current_power_copy = plus_dec(current_power, current_power);
        destroy_bigdec_object(current_power);
        current_power = create_bigdec_copy(current_power_copy);
        destroy_bigdec_object(current_power_copy);
    }

    /*
    * We know size of the result string, because we have already found the neccessary amount of powers
    */
    result_str = (char*)realloc(result_str, (a->sign == SIGN_NEGATIVE ? power_number : size) + strlen(result_str) + 1);
    if (!result_str) {
        destroy_bigdec_object(big_number);
        destroy_bigdec_object(current_power);
        for (i = 0; i < size; i++) {
            destroy_bigdec_object(powers[i]);
        }
        free(powers);
        return NULL;
    }

    for (i = size; i > 0; i--) {
        res_char = CHAR_ZERO;
        if (!is_less_dec(big_number, powers[i - 1])) { 
            big_number_copy = minus_dec(big_number, powers[i - 1]);
            destroy_bigdec_object(big_number);
            big_number = create_bigdec_copy(big_number_copy);
            destroy_bigdec_object(big_number_copy);

            res_char = CHAR_ONE;
        }

        len = strlen(result_str) + 2;
        result_str[len - 2] = res_char;
        result_str[len - 1] = CHAR_END_LINE;
    }

    destroy_bigdec_object(current_power);
    destroy_bigdec_object(big_number);
    for (i = 0; i < size; i++) {
        destroy_bigdec_object(powers[i]);
    }
    free(powers);

    if (a->sign == SIGN_NEGATIVE) {
        temp = (char*)malloc(2 * sizeof(char));
		strcpy(temp, STRING_ZERO);
        while (strlen(result_str) - 2 < power_number) {    
            result_str = insert_substring(result_str, temp, HEX_FIGURE_LENGTH_IN_BINARY);
        }
        free(temp);
    }

    return result_str;
}

/**
* @details This function will convert big_dec pointer of sign decimal number
* to char pointer(c string) of hexadecimal number 
* @param a - pointer of big_dec structure which represents sign decimal number. 
* @return char* - number in hexadecimal format.
*/
char* dec_to_hex(big_dec* a) {
    char *result_str, *sixteen_str, *one_str, *temp;
    big_dec *big_number, *sixteen_object, *abs_number, *current_power, *current_power_copy, *big_number_copy;
    big_dec **powers, **sums;
    int size, i, result_digit, len, sixteen_plus_counter;
    unsigned long int power_number, j;
    
    if (!a) {
        return NULL;
    }

    result_str = (char*)malloc(HEX_FIGURE_LENGTH_IN_BINARY * sizeof(char));
    if (!result_str) { 
        return NULL;
    }

    power_number = 1;

    sixteen_str = (char*)malloc(3 * sizeof(char));
    if (!sixteen_str) {
        free(result_str);
        return NULL;
    }
    strcpy(sixteen_str, BASE_HEX);
    sixteen_object = create_bigdec_object(SIGN_POSITIVE, sixteen_str);

    if (a->sign == SIGN_POSITIVE) {
        strcpy(result_str, PREFIX_HEX_POSITIVE);
        big_number = create_bigdec_copy(a);
    } else {
        strcpy(result_str, PREFIX_HEX_NEGATIVE);
        
        abs_number = create_bigdec_copy(a);
        abs_number->sign = SIGN_POSITIVE;

        one_str = (char*)malloc(2 * sizeof(char));
        if (!one_str) {
            free(result_str);
            destroy_bigdec_object(sixteen_object);
            destroy_bigdec_object(abs_number);
            return NULL;
        }
        strcpy(one_str, STRING_ONE);
        current_power = create_bigdec_object(SIGN_POSITIVE, one_str);

        while (is_less_dec(current_power, abs_number)) {
            ++power_number;

            for (sixteen_plus_counter = 0; sixteen_plus_counter < HEX_FIGURE_LENGTH_IN_BINARY; sixteen_plus_counter++) {
                current_power_copy = plus_dec(current_power, current_power);
                destroy_bigdec_object(current_power);
                current_power = create_bigdec_copy(current_power_copy);
                destroy_bigdec_object(current_power_copy);
            }
        }

        big_number = minus_dec(current_power, abs_number);

        destroy_bigdec_object(abs_number);
        destroy_bigdec_object(current_power);
    }

    one_str = (char*)malloc(2 * sizeof(char));
    if (!one_str) {
        free(result_str);
        destroy_bigdec_object(sixteen_object);
        destroy_bigdec_object(big_number);
        return NULL;
    }
    strcpy(one_str, STRING_ONE);
    current_power = create_bigdec_object(SIGN_POSITIVE, one_str);

    size = 0;
    powers = (big_dec**)malloc(size * sizeof(big_dec*));

    while (!is_less_dec(big_number, current_power)) {
        ++size;
        powers = (big_dec**)realloc(powers, size * sizeof(big_dec*));
        powers[size - 1] = create_bigdec_copy(current_power);

        for (sixteen_plus_counter = 0; sixteen_plus_counter < HEX_FIGURE_LENGTH_IN_BINARY; sixteen_plus_counter++) {
            current_power_copy = plus_dec(current_power, current_power);
            destroy_bigdec_object(current_power);
            current_power = create_bigdec_copy(current_power_copy);
            destroy_bigdec_object(current_power_copy);
        }
    }

    /*
    * We know size of the result string, because we have already found the neccessary amount of powers
    */
    result_str = (char*)realloc(result_str, size + strlen(result_str) + 1);
    if (!result_str) {
        destroy_bigdec_object(big_number);
        destroy_bigdec_object(current_power);
        destroy_bigdec_object(sixteen_object);
        for (i = 0; i < size; i++) {
            destroy_bigdec_object(powers[i]);
        }
        free(powers);
        return NULL;
    }

    len = strlen(result_str);
    for (i = size - 1; i >= 0; i--) {
        sums = (big_dec**)malloc(HEX_LENGTH_SYMBOLS_ALLOWED * sizeof(big_dec*));
        sums[0] = minus_dec(sixteen_object, sixteen_object);

        for (result_digit = 1; result_digit < HEX_LENGTH_SYMBOLS_ALLOWED; result_digit++) {
            sums[result_digit] = plus_dec(sums[result_digit - 1], powers[i]);
        }

        for (result_digit = 15; result_digit >= 0; result_digit--) {
            if (!is_less_dec(big_number, sums[result_digit])) {
                break;
            }
        }

        big_number_copy = minus_dec(big_number, sums[result_digit]);
        destroy_bigdec_object(big_number);
        big_number = create_bigdec_copy(big_number_copy);
        destroy_bigdec_object(big_number_copy);

        len = strlen(result_str) + 2;
        result_str[len - 2] = SYMBOLS_HEX_ALLOWED[result_digit];
        result_str[len - 1] = CHAR_END_LINE;

        for (result_digit = 0; result_digit < HEX_LENGTH_SYMBOLS_ALLOWED; result_digit++) {
            destroy_bigdec_object(sums[result_digit]);
        }
        free(sums);
    }

    destroy_bigdec_object(sixteen_object);
    destroy_bigdec_object(current_power);
    destroy_bigdec_object(big_number);
    for (i = 0; i < size; i++) {
        destroy_bigdec_object(powers[i]);
    }
    free(powers);

    if (a->sign == SIGN_NEGATIVE) {
        while (strlen(result_str) - 2 < power_number) {
            temp = (char*)malloc(2 * sizeof(char));
            if (!temp) {
                free(result_str);
                return NULL;
            }
			strcpy(temp, STRING_ZERO);
            result_str = insert_substring(result_str, temp, HEX_FIGURE_LENGTH_IN_BINARY);
            free(temp);
        }
    }

    /* more about HEX_FIGURE_AS_SIGN_DIVIDER_DIVIDER in defines.h*/
    if (strlen(result_str) - 1 > POSITION_SIGN && 
       ((result_str[POSITION_SIGN] == CHAR_ZERO && find_char_index_in_hex_string(result_str[POSITION_SIGN + 1]) < HEX_FIGURE_AS_SIGN_DIVIDER) || 
        (result_str[POSITION_SIGN] == CHAR_F && find_char_index_in_hex_string(result_str[POSITION_SIGN + 1]) >= HEX_FIGURE_AS_SIGN_DIVIDER))) {
        len = strlen(result_str);
        for (j = POSITION_SIGN; j < strlen(result_str); j++) {
            result_str[j] = result_str[j + 1];
        }
        result_str = (char*)realloc(result_str, len);
        if (!result_str) {
            return NULL;
        }
    }

    return result_str;
}

/**
* @details This function will convert big_int pointer of sign pseudo hexadecimal number(more in long_arithmetic.h)
* to char pointer(c string) of hexadecimal number. 
* @param a - pointer of big_int structure which represents
* sign pseudo hexadecimal number(more in long_arithmetic.h). 
* @return char* - number in hexadecimal format.
*/
char* big_to_hex(big_int* a) {
    char *result_str, *one_str;
    unsigned long int i, len, start_position, power_number, sixteen_plus_counter;
    big_int *big_number, *abs_number, *current_power, *current_power_copy;

    if (!a) {
        return NULL;
    }

    result_str = (char*)malloc((strlen(a->str_pseudo_hex) + 3 + 1) * sizeof(char));
    if (!result_str) {
        return NULL;
    }

    if (a->sign == SIGN_POSITIVE) {
        strcpy(result_str, PREFIX_HEX_POSITIVE);

        big_number = create_bigint_copy(a);
    } else {
        strcpy(result_str, PREFIX_HEX_NEGATIVE);

        abs_number = create_bigint_copy(a);
        abs_number->sign = SIGN_POSITIVE;

        one_str = (char*)malloc(2 * sizeof(char));
        if (!one_str) {
            free(result_str);
            destroy_bigint_object(abs_number);
            return NULL;
        }
        strcpy(one_str, STRING_ONE);
        current_power = create_bigint_object(SIGN_POSITIVE, one_str);

        while (is_less(current_power, abs_number)) {
            ++power_number;

            for (sixteen_plus_counter = 0; sixteen_plus_counter < HEX_FIGURE_LENGTH_IN_BINARY; sixteen_plus_counter++) {
                current_power_copy = plus(current_power, current_power);
                destroy_bigint_object(current_power);
                current_power = create_bigint_copy(current_power_copy);
                destroy_bigint_object(current_power_copy);
            }
        }

        big_number = minus(current_power, abs_number);

        destroy_bigint_object(abs_number);
        destroy_bigint_object(current_power);
    }

    strcat(result_str, big_number->str_pseudo_hex);

    len = strlen(result_str);
    start_position = POSITION_SIGN;
    if (len - 1 > POSITION_SIGN && (result_str[POSITION_SIGN] == CHAR_ZERO || result_str[POSITION_SIGN] == CHAR_F)) {
        ++start_position;
    }

    for (i = start_position; i < len; i++) {
        result_str[i] = SYMBOLS_HEX_ALLOWED[result_str[i] - CHAR_ZERO];
    }

    /* more about HEX_FIGURE_AS_SIGN_DIVIDER_DIVIDER in defines.h*/
    if (strlen(result_str) - 1 > POSITION_SIGN && 
       ((result_str[POSITION_SIGN] == CHAR_ZERO && find_char_index_in_hex_string(result_str[POSITION_SIGN + 1]) < HEX_FIGURE_AS_SIGN_DIVIDER) || 
        (result_str[POSITION_SIGN] == CHAR_F && find_char_index_in_hex_string(result_str[POSITION_SIGN + 1]) >= HEX_FIGURE_AS_SIGN_DIVIDER))) { 
        len = strlen(result_str);
        for (i = POSITION_SIGN; i < strlen(result_str); i++) {
            result_str[i] = result_str[i + 1];
        }
        result_str = (char*)realloc(result_str, len);
        if (!result_str) {
            return NULL;
        }
    }

    if (a->sign == SIGN_NEGATIVE && !strcmp(a->str_pseudo_hex, STRING_ONE)) {
        len = strlen(result_str);
        result_str[len - 1] = CHAR_END_LINE;

        result_str = (char*)realloc(result_str, len);
        if (!result_str) {
            return NULL;
        }
    }

    destroy_bigint_object(big_number);

    return result_str;
}

/**
* @details This function will convert char pointer(c string) of pseudohexadecimal number to binary number.
* @param number - char pointer(c string) of number in pseudohexadecimal format.
* @return char* - char pointer(c string) which represents binary number.
*/
char* abs_hex_to_bin(char *number) {
    char res_char;
    char *bin_str, *result_str;
    unsigned long int i;

    bin_str = (char*)malloc(((strlen(number) * HEX_FIGURE_LENGTH_IN_BINARY) + 1) * sizeof(char));
    if (!bin_str) {
        return NULL;
    }
    strcpy(bin_str, STRING_EMPTY);

    for (i = 0; i < strlen(number); i++) {
        res_char = number[i] - CHAR_ZERO;
        switch (res_char) {
            case  0: strcat(bin_str, HEX_TO_BIN_ZERO); break;
            case  1: strcat(bin_str, HEX_TO_BIN_ONE); break;
            case  2: strcat(bin_str, HEX_TO_BIN_TWO); break;
            case  3: strcat(bin_str, HEX_TO_BIN_THREE); break;
            case  4: strcat(bin_str, HEX_TO_BIN_FOUR); break;
            case  5: strcat(bin_str, HEX_TO_BIN_FIVE); break;
            case  6: strcat(bin_str, HEX_TO_BIN_SIX); break;
            case  7: strcat(bin_str, HEX_TO_BIN_SEVEN); break;
            case  8: strcat(bin_str, HEX_TO_BIN_EIGHT); break;
            case  9: strcat(bin_str, HEX_TO_BIN_NINE); break;
            case 10: strcat(bin_str, HEX_TO_BIN_A); break;
            case 11: strcat(bin_str, HEX_TO_BIN_B); break;
            case 12: strcat(bin_str, HEX_TO_BIN_C); break;
            case 13: strcat(bin_str, HEX_TO_BIN_D); break;
            case 14: strcat(bin_str, HEX_TO_BIN_E); break;
            case 15: strcat(bin_str, HEX_TO_BIN_F); break;
        }
    }
    bin_str[strlen(number) * HEX_FIGURE_LENGTH_IN_BINARY] = CHAR_END_LINE;
    bin_str = remove_leading_zeros(bin_str);

    result_str = (char*)malloc((strlen(bin_str) + 3 + 1) * sizeof(char));
    if (!result_str) {
        return NULL;
    }
    strcpy(result_str, PREFIX_BIN);
    strcat(result_str, STRING_ZERO);
    strcat(result_str, bin_str);
    free(bin_str);

    return result_str;
}

/**
* @details This function will convert big_int pointer of sign pseudo hexadecimal number(more in long_arithmetic.h)
* to char pointer(c string) of binary number. 
* @param a - pointer of big_int structure which represents
* sign pseudo hexadecimal number(more in long_arithmetic.h). 
* @return char* - number in binary format.
*/
char* big_to_bin(big_int* a) {
    char *number_str, *one_str, *temp_zero, *temp_bin;
    unsigned long int power_number, len;
    big_int *current_power_copy, *current_power, *abs_number, *big_number;

    if (!a) {
        return NULL;
    }

    number_str = abs_hex_to_bin(a->str_pseudo_hex);

    if (a->sign == SIGN_POSITIVE) {
        if ((a->sign == SIGN_NEGATIVE && !strcmp(a->str_pseudo_hex, STRING_ONE)) || (a->sign == SIGN_POSITIVE && !strcmp(a->str_pseudo_hex, STRING_ZERO))) {
            len = strlen(number_str);
            number_str[len - 1] = CHAR_END_LINE;

            number_str = (char*)realloc(number_str, len);
            if (!number_str) {
                return NULL;
            }
        }

        return number_str;
    } else {
        free(number_str);
        abs_number = create_bigint_copy(a);
        abs_number->sign = SIGN_POSITIVE;

        one_str = (char*)malloc(2 * sizeof(char));
        if (!one_str) {
            destroy_bigint_object(abs_number);
            return NULL;
        }
        strcpy(one_str, STRING_ONE);
        current_power = create_bigint_object(SIGN_POSITIVE, one_str);

        power_number = 1;
        while (is_less(current_power, abs_number)) {
            ++power_number;

            current_power_copy = plus(current_power, current_power);
            destroy_bigint_object(current_power);
            current_power = create_bigint_copy(current_power_copy);
            destroy_bigint_object(current_power_copy);
        }

        big_number = minus(current_power, abs_number);

        destroy_bigint_object(abs_number);
        destroy_bigint_object(current_power);

        temp_bin = abs_hex_to_bin(big_number->str_pseudo_hex);
        temp_bin[POSITION_SIGN] = CHAR_ONE;

        temp_zero = (char*)malloc(2 * sizeof(char));
		strcpy(temp_zero, STRING_ZERO);
        while (strlen(temp_bin) - 2 < power_number) {
            temp_bin = (char*)realloc(temp_bin, (strlen(temp_bin) + 2) * sizeof(char));
            temp_bin = insert_substring(temp_bin, temp_zero, HEX_FIGURE_LENGTH_IN_BINARY);
        }
        free(temp_zero);

        if ((a->sign == SIGN_NEGATIVE && !strcmp(a->str_pseudo_hex, STRING_ONE)) || (a->sign == SIGN_POSITIVE && !strcmp(a->str_pseudo_hex, STRING_ZERO))) {
            len = strlen(temp_bin);
            temp_bin[len - 1] = CHAR_END_LINE;

            temp_bin = (char*)realloc(temp_bin, len);
            if (!temp_bin) {
                return NULL;
            }
        }

        destroy_bigint_object(big_number);

        return temp_bin;
    }
}

/**
* @details This function will convert big_int pointer of sign pseudo hexadecimal number(more in long_arithmetic.h)
* to big_dec pointer of sign decimal number.
* @param a - pointer of big_int structure which represents
* sign pseudo hexadecimal number(more in long_arithmetic.h). 
* @return big_dec* - pointer of big_dec structure which represents sign decimal number.
*/
big_dec* big_to_dec(big_int* a) {
    char* temp_hex;
    big_dec *b;

    temp_hex = big_to_hex(a);
    
    /* more about HEX_FIGURE_AS_SIGN_DIVIDER_DIVIDER in defines.h*/
    if (temp_hex[POSITION_SIGN] != CHAR_ZERO && temp_hex[POSITION_SIGN] != CHAR_F) {
        temp_hex = (char*)realloc(temp_hex, (strlen(temp_hex) + 2) * sizeof(char));
        if (temp_hex[POSITION_SIGN] - CHAR_ZERO < HEX_FIGURE_AS_SIGN_DIVIDER) { 
            temp_hex = str_replace(temp_hex, PREFIX_HEX, PREFIX_HEX_POSITIVE);
        } else {
            temp_hex = str_replace(temp_hex, PREFIX_HEX, PREFIX_HEX_NEGATIVE);
        }
    }

    b = hex_to_dec(temp_hex);
    b->sign = a->sign;
    free(temp_hex);

    return b;
}

/**
* @details This function will convert big_dec pointer of sign decimal number
* to big_int pointer of sign pseudo hexadecimal number(more in long_arithmetic.h).
* @param a - pointer of big_dec structure which represents sign decimal number.
* @return big_int* - pointer of big_int structure which represents
* sign pseudo hexadecimal number(more in long_arithmetic.h). 
*/
big_int* dec_to_big(big_dec* a) {
    char* temp_hex, *result_str;
    big_dec* abs_a;
    big_int* b;
    unsigned long i, start_position, len_temp_hex;

    if (!a) {
        return NULL;
    }

    abs_a = create_bigdec_copy(a);
    abs_a->sign = SIGN_POSITIVE;

    temp_hex = dec_to_hex(abs_a);
    destroy_bigdec_object(abs_a);

    len_temp_hex = strlen(temp_hex);

    start_position = POSITION_SIGN;
    if (len_temp_hex - 1 > POSITION_SIGN && (temp_hex[POSITION_SIGN] == CHAR_ZERO || temp_hex[POSITION_SIGN] == CHAR_F)) {
        ++start_position;
    }

    result_str = (char*)malloc((len_temp_hex - start_position + 1) * sizeof(char));
    for (i = start_position; i < len_temp_hex; i++) {
        result_str[i - start_position] = find_char_index_in_hex_string(temp_hex[i]) + CHAR_ZERO;
    }
    result_str[len_temp_hex - start_position] = CHAR_END_LINE;
    free(temp_hex);
    
    b = create_bigint_object(a->sign, result_str);
    
    return b;
}

/*
* @details This function will convert char pointer(c string) of hexadecimal number
* to big_int pointer of sign pseudo hexadecimal number(more in long_arithmetic.h).
* @param a - char pointer(c string) of number in hexadecimal format (the number must be verified
* to match the hexadecimal format with the third-party function).
* @return big_int* - pointer of big_int structure which represents
* sign pseudo hexadecimal number(more in long_arithmetic.h). 
*/
big_int* hex_to_big(char* number) {
    char *result_str, *sixteen_str, *temp_str;
    big_int *b, *b_copy, *sixteen_object, *current_power, *power_grade;
    big_dec *sixteen_dec;
    unsigned long i, start_position, len_number, count, power_grade_int;

    if (!number) {
        return NULL;
    }

    len_number = strlen(number);

    start_position = POSITION_SIGN;
    if (len_number - 1 > POSITION_SIGN && (number[POSITION_SIGN] == CHAR_ZERO || number[POSITION_SIGN] == CHAR_F)) {
        ++start_position;
    }

    result_str = (char*)malloc((len_number - start_position + 1) * sizeof(char));
    for (i = start_position; i < len_number; i++) {
        result_str[i - start_position] = find_char_index_in_hex_string(number[i]) + CHAR_ZERO;
    }
    result_str[len_number - start_position] = CHAR_END_LINE;

    b = create_bigint_object(SIGN_POSITIVE, result_str);

    if (number[POSITION_SIGN] == CHAR_F) {
        sixteen_str = (char*)malloc(3 * sizeof(char));
        if (!sixteen_str) {
            free(result_str);
            return NULL;
        }
        strcpy(sixteen_str, BASE_HEX);
        sixteen_dec = create_bigdec_object(SIGN_POSITIVE, sixteen_str);
        sixteen_object = dec_to_big(sixteen_dec);

        power_grade_int = strlen(number) - strlen(PREFIX_HEX_NEGATIVE);
        count = get_digits_from_int(power_grade_int);
        temp_str = (char*)malloc((count + 1) * sizeof(char));
        if (!temp_str) { 
            return NULL;
        }
        int_to_char(power_grade_int, temp_str);
        temp_str[count] = CHAR_END_LINE;
        power_grade = create_bigint_object(SIGN_POSITIVE, temp_str);

        current_power = power(sixteen_object, power_grade);

        b_copy = minus(b, current_power);
        destroy_bigint_object(b);
        b = create_bigint_copy(b_copy);
        destroy_bigint_object(b_copy);

        destroy_bigint_object(sixteen_object);
        destroy_bigint_object(power_grade);
        destroy_bigdec_object(sixteen_dec);
    }

    return b;
}

/**
* @details This function will convert char pointer(c string) of binary number 
* to char pointer(c string) of hexadecimal number. 
* @param number - char pointer(c string) of number in binary format (the number must be verified
* to match the binary format with the third-party function).
* @return char* - number in hexadecimal format.
*/
char* bin_to_hex(char* number) {
    char *result_str, *temp, *number_copy;
    int result_digit;
    unsigned long int i, result_len, number_len, additional_count;

    if (!number) {
        return NULL;
    }

    number_len = strlen(number);

    number_copy = (char*)malloc((number_len + 1) * sizeof(char));
    if (!number_copy) {
        return NULL;
    }
    strcpy(number_copy, number);

    additional_count = ((number_len - 2) / HEX_FIGURE_LENGTH_IN_BINARY) - !((number_len - 2) % HEX_FIGURE_LENGTH_IN_BINARY) + 1;
    result_str = (char*)malloc((additional_count + HEX_FIGURE_LENGTH_IN_BINARY) * sizeof(char));
    if (!result_str) {
        free(number_copy);
        return NULL;
    }

    if (number[POSITION_SIGN] == CHAR_ZERO) {
        strcpy(result_str, PREFIX_HEX_POSITIVE);
    } else {
        strcpy(result_str, PREFIX_HEX_NEGATIVE);
    }

    additional_count = !((number_len - 2) % HEX_FIGURE_LENGTH_IN_BINARY) ? 0 : (HEX_FIGURE_LENGTH_IN_BINARY - ((number_len - 2) % HEX_FIGURE_LENGTH_IN_BINARY));
    number_copy = (char*)realloc(number_copy, (additional_count + strlen(number_copy) + 1) * sizeof(char));
    if (!number_copy) {
        free(result_str);
        return NULL;
    }
    temp = (char*)malloc(2 * sizeof(char));
    if (!temp) {
        free(result_str);
        free(number_copy);
        return NULL;
    }
    for (i = 0; i < additional_count; i++) {
        
        if (number[POSITION_SIGN] == CHAR_ONE) {
		    strcpy(temp, STRING_ONE);
        } else {
            strcpy(temp, STRING_ZERO);
        }
        
        number_copy = insert_substring(number_copy, temp, 3);
    }
    free(temp);

    for (i = POSITION_SIGN; i < strlen(number_copy); i += HEX_FIGURE_LENGTH_IN_BINARY) {
        result_digit = 0;
        if (number_copy[i] == CHAR_ONE) {
            result_digit += BIN_THIRD_DEGREE_TO_DEC;
        }
        if (number_copy[i + 1] == CHAR_ONE) {
            result_digit += BIN_SECOND_DEGREE_TO_DEC;
        }
        if (number_copy[i + 2] == CHAR_ONE) {
            result_digit += BIN_FIRST_DEGREE_TO_DEC;
        }
        if (number_copy[i + 3] == CHAR_ONE) {
            result_digit += BIN_ZERO_DEGREE_TO_DEC;
        }

        result_len = strlen(result_str) + 2;
        result_str[result_len - 2] = SYMBOLS_HEX_ALLOWED[result_digit];
        result_str[result_len - 1] = CHAR_END_LINE;
    }

    /* more about HEX_FIGURE_AS_SIGN_DIVIDER_DIVIDER in defines.h*/
    if (strlen(result_str) - 1 > POSITION_SIGN && 
       ((result_str[POSITION_SIGN] == CHAR_ZERO && find_char_index_in_hex_string(result_str[POSITION_SIGN + 1]) < HEX_FIGURE_AS_SIGN_DIVIDER) || 
        (result_str[POSITION_SIGN] == CHAR_F && find_char_index_in_hex_string(result_str[POSITION_SIGN + 1]) >= HEX_FIGURE_AS_SIGN_DIVIDER))) { 
        result_len = strlen(result_str);
        for (i = POSITION_SIGN; i < strlen(result_str); i++) {
            result_str[i] = result_str[i + 1];
        }
        
        result_str = (char*)realloc(result_str, result_len);
        if (!result_str) {
            free(result_str);
            free(number_copy);
            return NULL;
        }
    }

    free(number_copy);
    
    return result_str;
}

/**
* @details This function prepares big_int for display in the specified format.
* @param a - pointer to big_int structure which represents sign pseudo hexadecimal number(more in long_arithmetic.h).
* @param mode - variable that indicates in which format the number will be prepared for displaying.
* Predefined modes:
*   MODE_DEC  -> char 'd'
*   MODE_BIN  -> char 'b'
*   MODE_HEX  -> char 'h'
*   MODE_QUIT -> char 'q'
* in other cases error will occured.
* @return char* - prepared number for displaying.
*/
char* to_string(big_int* a, char mode) {
    char *result_str;
    big_dec *b;
    int len;

    if (!a || !mode) {
        return NULL;
    }

    if (mode == MODE_DEC) {
        b = big_to_dec(a);

        len = strlen(b->str_dec) + 1;
        if (a->sign == SIGN_NEGATIVE && !is_zero(a)) {
            ++len;
        }
        result_str = (char*)malloc(len * sizeof(char));
        if (!result_str) {
            destroy_bigdec_object(b);
            return NULL;
        }
		strcpy(result_str, STRING_EMPTY);

        if (a->sign == SIGN_NEGATIVE && !is_zero(a)) {
            strcat(result_str, STRING_MINUS);
        }
        strcat(result_str, b->str_dec);

        destroy_bigdec_object(b);
        return result_str;
    } 
    if (mode == MODE_BIN) {
        return big_to_bin(a);
    } 
    if (mode == MODE_HEX) {
        return big_to_hex(a);
    } 

    return NULL;
}
