/**
* @file expression.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"
#include "long_arithmetic.h"
#include "long_arithmetic_dec.h"
#include "utils.h"
#include "converter.h"

/**
* @details This function checks whether the input is an arithmetical expression or not.
* @param input - char pointer ("c string") of the entry string from which the desired operation will be executed.
* @return bool - true will be returned if this char pointer is an arithmetical expression, false if not.
*/
bool is_it_an_arithmetical_expression(char* input) {
    bool flag;
    unsigned long int i, j, unknown_symbol_counter;

    if (!input) {
        return false;
    }

    unknown_symbol_counter = 0;
    for (i = 0; input[i] != CHAR_END_LINE; i++) {
        if (strchr(SYMBOLS_ALLOWED, input[i]) == NULL) {
            ++unknown_symbol_counter;
        }
    }
    if (unknown_symbol_counter == strlen(input)) {
        return false;
    }

    /*
    * Letters check
    */
    for (i = 0; i < strlen(input); i++) {
        if (strchr(SYMBOLS_LETTERS_ALLOWED, input[i]) != NULL && (!i || !(input[i - 1] == PREFIX_BIN[0] && input[i] == PREFIX_BIN[1]))) {
            flag = false;
            for (j = i; j > 0; j--) {
                if (input[j - 1] == PREFIX_HEX[0] && input[j] == PREFIX_HEX[1]) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                return false;
            }
        }
    }

    return true;
}

/**
* @details This function will check for the correctness of the arithmetical expression record,
* specifically the correctly scaled brackets, the correctness of the numbers in binary, 
* hexadecimal and decimal numerals.
* @param input - char pointer ("c string") of arithmetical expression, 
* this parameter should be checked beforehand by is_it_an_arithmetical_expression function.
* @return bool - true will be returned if this arithmetical expression is correct, false if not.
*/
bool is_the_expression_correct(char* input) {
    int brackets_count;
    unsigned long int i, j;
    bool flag;

    if (!input) {
        return false;
    }
    
    if (!strlen(input)) {
        return false;
    }
    

    /*
    * HEX number check
    */
    for (i = 0; i < strlen(input) - 1; i++) {
        if (input[i] == PREFIX_HEX[0] && input[i + 1] == PREFIX_HEX[1]) {
            for (j = i + 2; strchr(SYMBOLS_OPERANDS, input[j]) == NULL && input[j] != CHAR_END_LINE; j++) {
                if (strchr(SYMBOLS_HEX_ALLOWED, input[j]) == NULL) {
                    return false;
                }
            }
        }
    }

    /*
    * BIN number check
    */
    for (i = 0; i < strlen(input) - 1; i++) {
        if ((!i || strchr(SYMBOLS_OPERANDS, input[i - 1]) != NULL) && input[i] == PREFIX_BIN[0] && input[i + 1] == PREFIX_BIN[1]) {
            for (j = i + 2; strchr(SYMBOLS_OPERANDS, input[j]) == NULL && input[j] != CHAR_END_LINE; j++) {
                if (strchr(SYMBOLS_BIN_ALLOWED, input[j]) == NULL) {
                    return false;
                }
            }
        }
    }

    /*
    * Correct brackets expression check
    */
    brackets_count = 0;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == SYMBOLS_BRACKETS[0]) {
            ++brackets_count;
        } else 
        if (input[i] == SYMBOLS_BRACKETS[1]) {
            --brackets_count;
            if (brackets_count < 0) {
                return false;
            }
        }
    }
    if (brackets_count) {
        return false;
    }

    /*
    * Empty brackets expression check
    */
    for (i = 0; i < strlen(input) - 1; i++) {
        if (input[i] == SYMBOLS_BRACKETS[0] && input[i + 1] == SYMBOLS_BRACKETS[1]) {
            return false;
        }
    }

    /*
    * Letters check
    */
    for (i = 0; i < strlen(input); i++) {
        if (strchr(SYMBOLS_LETTERS_ALLOWED, input[i]) != NULL && (!i || !(input[i - 1] == PREFIX_BIN[0] && input[i] == PREFIX_BIN[1]))) {
            flag = false;
            for (j = i; j > 0; j--) {
                if (input[j - 1] == PREFIX_HEX[0] && input[j] == PREFIX_HEX[1]) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                return false;
            }
        }
    }

    /*
    * Factorial before number check
    */
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == OPERAND_FACTORIAL) {
            if (!i || (strchr(SYMBOLS_OPERANDS, input[i - 1]) != NULL && input[i - 1] != SYMBOLS_BRACKETS[1] && input[i - 1] != OPERAND_FACTORIAL)) {
                return false;
            }
        }
    }

    return true;
}

/**
* @details This function will identify all unary minuses in the char pointer ("c string") 
* and replace them with char '_', and move them by number (or bracket expression),
* this is necessary for correct operation of the stack of operations. 
* @example '-1' will be changed on '1_'.
* @example '-(1+1)' will be changed on '(1+1)_'.  
* @param input - char pointer ("c string") of the entry string from which the desired operation will be executed.
* @return char* - char pointer ("c string") with modified unary minuses.
*/
char* replace_unary_minus(char* input) {
    char *temp, *space_str;
    int number_count, brackets_count;
    unsigned long int i, j, k, p, minuses_count;
    bool is_there_any_bracket_flag;
    if (!input) {
        return NULL;
    }
    
    for (i = 0; i < strlen(input) - 1; i++) {
        if ((!i || input[i - 1] == OPERAND_UNARY_MINUS || (strchr(SYMBOLS_OPERANDS, input[i - 1]) != NULL && input[i - 1] != SYMBOLS_BRACKETS[1])) && 
             input[i] == OPERAND_MINUS && (!i || input[i - 1] != OPERAND_FACTORIAL) &&
            (strchr(SYMBOLS_DEC_ALLOWED, input[i + 1]) != NULL || input[i + 1] == OPERAND_MINUS || input[i + 1] == SYMBOLS_BRACKETS[0])) {
                input[i] = OPERAND_UNARY_MINUS;
        }
    }

    for (i = 0; i < strlen(input); i++) {
        if (input[i] == OPERAND_UNARY_MINUS) {
            j = 0;
            minuses_count = 0; 
            for (j = i; j < strlen(input); j++) {
                if (input[j] == OPERAND_UNARY_MINUS) {
                    ++minuses_count;
                } else {
                    break;
                }
            }

            number_count = 0;
            brackets_count = 0;
            k = j;
            is_there_any_bracket_flag = (input[k] == SYMBOLS_BRACKETS[0]);
            while (k < strlen(input)) {
                ++number_count;
                
                if (is_there_any_bracket_flag) {
                    if (input[k] == SYMBOLS_BRACKETS[0]) {
                        ++brackets_count;
                    } else 
                    if (input[k] == SYMBOLS_BRACKETS[1]) {
                        --brackets_count;
                    }
                    
                    if (!brackets_count) {
                        ++k;
                        break;
                    }
                } else {
                    if (strchr(SYMBOLS_OPERANDS, input[k]) != NULL) {
                        --number_count;
                        break;
                    }
                }

                ++k;
            }

            while (k < strlen(input) && input[k] == OPERAND_FACTORIAL) {
                ++number_count;
                ++k;
            }

            for (p = i; p < strlen(input); p++) {
                if (input[p] == OPERAND_UNARY_MINUS) {
                    input[p] = CHAR_SPACE;
                } else {
                    break;
                }
            }

            space_str = (char*)malloc((minuses_count + 1) * sizeof(char));
            if (!space_str) { 
                return NULL;
            }
            
            for (p = 0; p < minuses_count; p++) {
                space_str[p] = CHAR_SPACE;
            }
            
            space_str[minuses_count] = CHAR_END_LINE;

            input = str_remove_substring(input, space_str);
            free(space_str);

            temp = (char*)malloc(2 * sizeof(char));
			strcpy(temp, "_");
            for (p = 0; p < minuses_count; p++) {
                input = insert_substring(input, temp, i + number_count + 1);
            }
            
            free(temp);

            i += number_count + minuses_count;
        }
    }

    return input;
}

/**
* @details This function returns a predefined operand that is passed as a function parameter.
* @param c - operand which priority the function should return.
* @return int - operand priority, which was predefined.
* Predefined priorities:
* operand '-' -> priority '0';
* operand '+' -> priority '0';
* operand '%' -> priority '1';
* operand '/' -> priority '2';
* operand '*' -> priority '2';
* operand '^' -> priority '3';
* operand '_' -> priority '4';
* operand '!' -> priotiry '5';
* other operands -> priority '-1'; - will be returned if parameter is undefined.
*/
int get_operand_priority(char c) {
    switch (c) {
        case OPERAND_MINUS:       return PRECEDENCE_ZERO;
        case OPERAND_PLUS:        return PRECEDENCE_ZERO;
        case OPERAND_MOD:         return PRECEDENCE_ONE;
        case OPERAND_DIV:         return PRECEDENCE_TWO;
        case OPERAND_MULTIPLY:    return PRECEDENCE_TWO;
        case OPERAND_POWER:       return PRECEDENCE_THREE;
        case OPERAND_UNARY_MINUS: return PRECEDENCE_FOUR;
        case OPERAND_FACTORIAL:   return PRECEDENCE_FIVE;
        default:                   return PRECEDENCE_MINUS;
    }
}

/**
* @details This function will clear (free) the memory of the number's stack which contains pointers to big_int. 
* @param number_stack - stack of numbers, number presented as pointer of big_int structure, which represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
* @param number_stack_size - stack length.
* @return big_int** - NULL, stack will be cleared (memory free).
*/
big_int** clear_number_stack(big_int** number_stack, int number_stack_size) {
    int i;

    if (number_stack) {
        for (i = 0; i < number_stack_size; i++) {
            if (number_stack[i]) {
                destroy_bigint_object(number_stack[i]);
            }
        }

        free(number_stack);
        number_stack = NULL;
    }

    return NULL;
}

/**
* @details This function will clear (free) the memory of the stack 
* that contains mathematical operands such as addition, subtraction and so on...
* @param operand_stack - stack that contains operands, only 
* predefined mathematical operands(more in defines.h) are allowed.
* @return char* - NULL, stack will be cleared (memory free).
*/
char* clear_operand_stack(char* operand_stack) {
    if (operand_stack) {
        free(operand_stack);
        operand_stack = NULL;
    }

    return NULL;
}

/**
* @details This function changes the stack that is passed as a function's parameter 
* (specifically adds to the stack the operand that was passed as the second parameter of the function).
* @param operand_stack - stack of operands to which the operand will be added.
* @param operand - predefined mathematical operand(more in defines.h).
* @return char* - changed operand_stack, passed as function's parameter,
* in which the transmitted operand was added.
*/
char* add_operand_to_stack(char* operand_stack, char operand) {
    int len;

    if (!operand_stack) {
        return NULL;
    }

    len = strlen(operand_stack) + 2;
    operand_stack = (char*)realloc(operand_stack, len);
    if (!operand_stack) {
        return NULL;
    }

    operand_stack[len - 2] = operand;
    operand_stack[len - 1] = CHAR_END_LINE;

    return operand_stack;
}

/**
* @details This function removes the operand that is the last one in the stack.
* @param operand_stack - stack of operands from which the operand will be removed.
* @return char* - changed operand_stack, passed as function's parameter,
* from which the operand, that was last in stack, was removed.
*/
char* remove_top_operand_from_stack(char* operand_stack) {
    if (!operand_stack) {
        return NULL;
    }

    if (!strlen(operand_stack)) {
        return NULL;
    }

    operand_stack[strlen(operand_stack) - 1] = CHAR_END_LINE;

    return operand_stack;
}

/**
* @details This function will process operand and number stacks,
* function will refer to predefined operations priorities will invoke the functions necessary to calculate
* in the required order, taking into account the priorities and unanality of operators.
* @param number_stack stack of numbers, number presented as pointer of big_int structure, which represents
* pseudo hexadecimal number(more in long_arithmetic.h).
* @param number_stack_size - length of number's stack.
* @param operand_stack - stack that contains operands, only 
* predefined mathematical operands(more in defines.h) are allowed.
* @param success_flag - pointer of int that indicates errors in calculations.
* @return big_int** - changed numer_stack, passed as function's parameter,
* from which the numbers was calculate and the result was added to the stack.
*/
big_int** proceed_stack(big_int** number_stack, int* number_stack_size, char* operand_stack, int* success_flag) {
    char c;
    big_int *(*binary_operation)(big_int* a, big_int* b) = NULL;
    big_int *(*unary_operation)(big_int* a) = NULL;
    big_int *result;
    
    *success_flag = STATUS_NOT_OK;

    if (!number_stack || !operand_stack) {
        return NULL;
    }


    c = operand_stack[(strlen(operand_stack)) - 1];
    switch (c) {
        case OPERAND_MINUS:       binary_operation = minus; break;
        case OPERAND_PLUS:        binary_operation = plus; break;
        case OPERAND_MOD:         binary_operation = divide_mod; break;
        case OPERAND_DIV:         binary_operation = divide_div; break;
        case OPERAND_MULTIPLY:    binary_operation = multiply; break;
        case OPERAND_UNARY_MINUS: unary_operation = reverse; break;
        case OPERAND_POWER:       binary_operation = power; break;
        case OPERAND_FACTORIAL:   unary_operation = factorial; break;
        default: return NULL;
    }

    if (binary_operation) { /* Binary operation, operate two numbers */
        if (*number_stack_size < 2) {
            number_stack = clear_number_stack(number_stack, *number_stack_size);
            return NULL;
        }

        result = binary_operation(number_stack[(*number_stack_size) - 2], number_stack[(*number_stack_size) - 1]);
        if (!result) {
            return number_stack;
        }

        destroy_bigint_object(number_stack[(*number_stack_size) - 2]);
        destroy_bigint_object(number_stack[(*number_stack_size) - 1]);

        number_stack = (big_int**)realloc(number_stack, (--(*number_stack_size)) * sizeof(big_int*));
        if (!number_stack) {
            return NULL;
        }
    
        number_stack[(*number_stack_size) - 1] = result;

    } else { /* Unary operation, operate one number */
        if (*number_stack_size < 1) {
            number_stack = clear_number_stack(number_stack, *number_stack_size);
            return NULL;
        }

        result = unary_operation(number_stack[(*number_stack_size) - 1]);
        if (!result) {
            return number_stack;
        }

        destroy_bigint_object(number_stack[(*number_stack_size) - 1]);
        number_stack[(*number_stack_size) - 1] = result;

    }

    *success_flag = STATUS_OK;
    return number_stack;
}

/**
* @details The function that will process the mathematical expression by converting the input numbers into
* the correct numerical system (pseudo hexadecimal), will process the brackets and operands,
* will gradually add them and numbers to the stacks and work with it, calling the necessary functions
* for computing, also will call the function to_string, which will handle the further display of
* the result in the desired number system in the console.
* @param input - char pointer("c string") of the entry string from which the desired operation will be executed.
* @param mode - an indicator that shows in which number system the number will be written to the console,
* the possible variations of this indicator are predefined(more in defines.h).
* @return bool - an indicator that shows whether the arithmetic expression was counted or if there was an error.
*/
bool work_with_expression(char* input, char mode, int* success_flag) {
    int number_stack_size, shift;
    unsigned long int i, j, size_of_substr, index;
    char mode_temp;
    char *operand_stack, *symbols_for_check, *substr, *print_result;
    big_int **number_stack;
    big_int *big_num;
    big_dec *big_dec_num;

    if (!input) {
        return false;
    }
    
    if (!is_the_expression_correct(input)) {
        return false;
    }

    input = replace_unary_minus(input);

    number_stack_size = 0;
    number_stack = (big_int**)malloc((number_stack_size + 1) * sizeof(big_int*));
    if (!number_stack) {
        return false;
    }

    operand_stack = (char*)malloc(1 * sizeof(char));
    if (!operand_stack) {
        free(number_stack);
        return false;
    }

	strcpy(operand_stack, "");

    for (i = 0; i < strlen(input); i++) {

        if (strchr(SYMBOLS_DEC_ALLOWED, input[i])) { /* Find number */
            /* Digit */
            shift = 1;
            mode_temp = MODE_DEC;
            symbols_for_check = (char*)malloc((strlen(SYMBOLS_DEC_ALLOWED) + 1) * sizeof(char));
            strcpy(symbols_for_check, SYMBOLS_DEC_ALLOWED);
            if (i + 1 < strlen(input)) {
                if (input[i + 1] == PREFIX_HEX[1]) {
                    ++shift;
                    mode_temp = MODE_HEX;
                    symbols_for_check = (char*)realloc(symbols_for_check, (strlen(SYMBOLS_HEX_ALLOWED) + 1) * sizeof(char));
                    strcpy(symbols_for_check, SYMBOLS_HEX_ALLOWED);
                } else
                if (input[i + 1] == PREFIX_BIN[1]) {
                    ++shift;
                    mode_temp = MODE_BIN;
                    symbols_for_check = (char*)realloc(symbols_for_check, (strlen(SYMBOLS_BIN_ALLOWED) + 1) * sizeof(char));
                    strcpy(symbols_for_check, SYMBOLS_BIN_ALLOWED);
                }
            }

            if (!symbols_for_check) {
                number_stack = clear_number_stack(number_stack, number_stack_size);
                operand_stack = clear_operand_stack(operand_stack);
                return false;
            }

            index = i + 1;
            for (j = i + shift; j < strlen(input); j++) {
                if (!strchr(symbols_for_check, input[j])) {
                    index = j;
                    break;
                } else 
                if (strchr(symbols_for_check, input[j]) && j == strlen(input) - 1) {
                    index = j + 1;
                    break;
                }
            }

            free(symbols_for_check);

            size_of_substr = index - i;
            substr = (char*)malloc((size_of_substr + 1) * sizeof(char));
            if (!substr) {
                number_stack = clear_number_stack(number_stack, number_stack_size);
                operand_stack = clear_operand_stack(operand_stack);
                return false;
            }
            
            strncpy(substr, input + i, size_of_substr);
            substr[size_of_substr] = CHAR_END_LINE;

            i = index - 1;

            /* more about HEX_FIGURE_AS_SIGN_DIVIDER_DIVIDER in defines.h*/
            if (mode_temp == MODE_HEX) {
                if (substr[POSITION_SIGN] != CHAR_ZERO && substr[POSITION_SIGN] != CHAR_F) {
                    substr = (char*)realloc(substr, (strlen(substr) + 2) * sizeof(char));
                    if (substr[POSITION_SIGN] - CHAR_ZERO < HEX_FIGURE_AS_SIGN_DIVIDER) { 
                        substr = str_replace(substr, PREFIX_HEX, PREFIX_HEX_POSITIVE);
                    } else {
                        substr = str_replace(substr, PREFIX_HEX, PREFIX_HEX_NEGATIVE);
                    }
                }

                big_dec_num = hex_to_dec(substr);
                big_num = dec_to_big(big_dec_num);
                free(substr);
            } else 
            if (mode_temp == MODE_BIN) {
                big_dec_num = bin_to_dec(substr);
                big_num = dec_to_big(big_dec_num);
                free(substr);
            } else {
                substr = remove_leading_zeros(substr);
                big_dec_num = create_bigdec_object(SIGN_POSITIVE, substr);
                big_num = dec_to_big(big_dec_num);
            }
            destroy_bigdec_object(big_dec_num);
 
            number_stack = (big_int**)realloc(number_stack, (++number_stack_size) * sizeof(big_int*));
            if (!number_stack) {
                number_stack = clear_number_stack(number_stack, number_stack_size);
                operand_stack = clear_operand_stack(operand_stack);
                return false;
            }
            
            number_stack[number_stack_size - 1] = big_num;
        } else {
            /* Operand or bracket */
            if (input[i] == SYMBOLS_BRACKETS[0] || !strlen(operand_stack) || get_operand_priority(input[i]) > get_operand_priority(operand_stack[strlen(operand_stack) - 1])) {
                operand_stack = add_operand_to_stack(operand_stack, input[i]);
                if (!operand_stack) {
                    number_stack = clear_number_stack(number_stack, number_stack_size);
                    operand_stack = clear_operand_stack(operand_stack);
                    return false;
                }

                continue;
            }

            if (input[i] == SYMBOLS_BRACKETS[1]) {
                while (operand_stack[strlen(operand_stack) - 1] != SYMBOLS_BRACKETS[0]) {
                    number_stack = proceed_stack(number_stack, &number_stack_size, operand_stack, success_flag);
                    operand_stack = remove_top_operand_from_stack(operand_stack);
                    if (!operand_stack || !number_stack || *success_flag == STATUS_NOT_OK) {
                        if (!number_stack) {
                            *success_flag = STATUS_OK;
                        }
                        number_stack = clear_number_stack(number_stack, number_stack_size);
                        operand_stack = clear_operand_stack(operand_stack);
                        return false;
                    }
                }

                operand_stack = remove_top_operand_from_stack(operand_stack);
                if (!operand_stack) {
                    number_stack = clear_number_stack(number_stack, number_stack_size);
                    operand_stack = clear_operand_stack(operand_stack);
                    return false;
                }

                continue;
            }

            if (get_operand_priority(input[i]) <= get_operand_priority(operand_stack[strlen(operand_stack) - 1])) {
                number_stack = proceed_stack(number_stack, &number_stack_size, operand_stack, success_flag);
                operand_stack = remove_top_operand_from_stack(operand_stack);
                if (!operand_stack || !number_stack || *success_flag == STATUS_NOT_OK) {
                    if (!number_stack) {
                        *success_flag = STATUS_OK;
                    }
                    number_stack = clear_number_stack(number_stack, number_stack_size);
                    operand_stack = clear_operand_stack(operand_stack);
                    return false;
                }

                --i;
                continue;
            }
        }
    }

    while (strlen(operand_stack) > 0) {
        number_stack = proceed_stack(number_stack, &number_stack_size, operand_stack, success_flag);
        operand_stack = remove_top_operand_from_stack(operand_stack);
        if (!operand_stack || !number_stack || *success_flag == STATUS_NOT_OK) {
            if (!number_stack) {
                *success_flag = STATUS_OK;
            }
            number_stack = clear_number_stack(number_stack, number_stack_size);
            operand_stack = clear_operand_stack(operand_stack);
            return false;
        }
    }

    if (number_stack_size != 1) {
        number_stack = clear_number_stack(number_stack, number_stack_size);
        operand_stack = clear_operand_stack(operand_stack);
        *success_flag = STATUS_OK;
        return false;
    }

    print_result = to_string(number_stack[number_stack_size - 1], mode);
    printf("%s\n", print_result);
    free(print_result);

    number_stack = clear_number_stack(number_stack, number_stack_size);
    operand_stack = clear_operand_stack(operand_stack);

    return true;
}