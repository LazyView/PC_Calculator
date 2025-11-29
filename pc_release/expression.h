/**
* @file expression.h - header file of expression.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @details connecting this file, it is important to check 
* that the header files stdbool.h and long_arithmetic are connected,
* as this file uses bool and big_int structure.
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef EXPRESSION
#define EXPRESSION

/**
* @details This function checks whether the input is an arithmetical expression or not.
* @param input - char pointer ("c string") of the entry string from that the desired operation will be executed.
* @return bool - true will be returned if this char pointer is an arithmetical expression, false if not.
*/
bool is_it_an_arithmetical_expression(char* input);

/**
* @details This function will check for the correctness of the arithmetical expression record,
* specifically the correctly scaled brackets, the correctness of the numbers in binary, 
* hexadecimal and decimal numerals.
* @param input - char pointer ("c string") of arithmetical expression, 
* this parameter should be checked beforehand by is_it_an_arithmetical_expression function.
* @return bool - true will be returned if this arithmetical expression is correct, false if not.
*/
bool is_the_expression_correct(char* input);

/**
* @details This function will identify all unary minuses in the char pointer ("c string") 
* and replace them with char '_', and move them by number (or bracket expression),
* this is necessary for correct operation of the stack of operations. 
* @example '-1' will be changed on '1_'.
* @example '-(1+1)' will be changed on '(1+1)_'.  
* @param input - char pointer ("c string") of the entry string from that the desired operation will be executed.
* @return char* - char pointer ("c string") with modified unary minuses.
*/
char* replace_unary_minus(char* input);

/**
* @details This function returns a predefined operand that is passed as a function parameter.
* @param c - operand that priority the function should return.
* @return int - operand priority, that was predefined.
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
int get_operand_priority(char c);

/**
* @details This function will clear (free) the memory of the number's stack that contains pointers to big_int. 
* @param number_stack - stack of numbers, number presented as pointer of big_int structure, that represents
* pseudo hexadecimal number(more in long_arithmetic.h). 
* @param number_stack_size - stack length.
* @return big_int** - NULL, stack will be cleared (memory free).
*/
big_int** clear_number_stack(big_int** number_stack, int number_stack_size);

/**
* @details This function will clear (free) the memory of the stack 
* that contains mathematical operands such as addition, subtraction and so on...
* @param operand_stack - stack that contains operands, only 
* predefined mathematical operands(more in defines.h) are allowed.
* @return char* - NULL, stack will be cleared (memory free).
*/
char* clear_operand_stack(char* operand_stack);

/**
* @details This function changes the stack that is passed as a function's parameter 
* (specifically adds to the stack the operand that was passed as the second parameter of the function).
* @param operand_stack - stack of operands to that the operand will be added.
* @param operand - predefined mathematical operand(more in defines.h).
* @return char* - changed operand_stack, passed as function's parameter,
* in that the transmitted operand was added.
*/
char* add_operand_to_stack(char* operand_stack, char operand);

/**
* @details This function removes the operand that is the last one in the stack.
* @param operand_stack - stack of operands from that the operand will be removed.
* @return char* - changed operand_stack, passed as function's parameter,
* from that the operand, that was last in stack, was removed.
*/
char* remove_top_operand_from_stack(char* operand_stack);

/**
* @details This function will process operand and number stacks,
* function will refer to predefined operations priorities will invoke the functions necessary to calculate
* in the required order, taking into account the priorities and unanality of operators.
* @param number_stack stack of numbers, number presented as pointer of big_int structure, that represents
* pseudo hexadecimal number(more in long_arithmetic.h).
* @param number_stack_size - length of number's stack.
* @param operand_stack - stack that contains operands, only 
* predefined mathematical operands(more in defines.h) are allowed.
* @param success_flag - pointer of int that indicates errors in calculations.
* @return big_int** - changed numer_stack, passed as function's parameter,
* from that the numbers was calculate and the result was added to the stack.
*/
big_int** proceed_stack(big_int** number_stack, int* number_stack_size, char* operand_stack, int* success_flag);

/**
* @details The function that will process the mathematical expression by converting the input numbers into
* the correct numerical system (pseudo hexadecimal), will process the brackets and operands,
* will gradually add them and numbers to the stacks and work with it, calling the necessary functions
* for computing, also will call the function to_string, that will handle the further display of
* the result in the desired number system in the console.
* @param input - char pointer("c string") of the entry string from that the desired operation will be executed.
* @param mode - an indicator that shows in that number system the number will be written to the console,
* the possible variations of this indicator are predefined(more in defines.h).
* @return bool - an indicator that shows whether the arithmetic expression was counted or if there was an error.
*/
bool work_with_expression(char* input, char mode, int* success_flag);

#endif