/**
* @file main.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "utils.h"
#include "long_arithmetic.h"
#include "expression.h"
#include "defines.h"

/**
* This function will simulate the program when reading from a file passed as a program argument.
* @param str - lines that have been read from files and should be processed.
* @param count_of_lines - number of lines that were read from the file.
*/
void work_with_file(char **str, int* count_of_lines) {
    char mode = MODE_DEC;
    int i;
    bool quit_flag = false;

    for (i = 0; i < *count_of_lines; i++) {
        if (!quit_flag) {
            mode = menu(&mode, str[i]);
            if (mode == MODE_QUIT) {
                quit_flag = true;
            }
        } else {
            free(str[i]);
        }
    }
}

/**
* This function will simulate the program from the console.
*/
void start_work() {
    char mode = MODE_DEC;

    while ((mode = menu(&mode, NULL)) != MODE_QUIT);
}

/**
* This function will simulate menu, will call certain functions for string processing, 
* will call functions to verify the entered expression, if the expression is a valid command,
* then certain actions will be performed, if the entered expression is an arithmetic expression,
* corresponding functions will be called and the expression will be evaluated.
* @param mode - the active mod of the program to be passed along with the expression for evaluation.
* @param str - received string, whether it was received like input from console or
* by reading from a file passed as a program argument.
* @return char - predefined mode of program, after evaluation of the received expression.
*/
char menu(char* mode, char* str) {
    int success_flag;
    char return_result;
    char *input, *orig_input;
    
    if (!mode) {
        return MODE_QUIT;
    }

    printf("> ");
    if (str) {
        printf("%s\n", str);
    }
    
    return_result = *mode;
    if (!str) {
        input = get_input_string();
    } 
    else {
        input = str;
    }

    orig_input = (char*)malloc((strlen(input) + 1) * sizeof(char));
    if (!orig_input) {
        free(input);
        return return_result;
    }
    strcpy(orig_input, input);

    input = trim_string(input);

    if (!strcmp(input, MENU_DEC)) {
        puts(MENU_DEC);
        return_result = MODE_DEC;
    } else 
    if (!strcmp(input, MENU_BIN)) {
        puts(MENU_BIN);
        return_result = MODE_BIN;
    } else 
    if (!strcmp(input, MENU_HEX)) {
        puts(MENU_HEX);
        return_result = MODE_HEX;
    } else 
    if (!strcmp(input, MENU_OUT)) {
        switch (*mode) {
            case MODE_DEC: puts(MENU_DEC); break;
            case MODE_BIN: puts(MENU_BIN); break;
            case MODE_HEX: puts(MENU_HEX); break;
        }
    } else 
    if (!strcmp(input, MENU_QUIT)) {
        puts(MENU_QUIT);
        return_result = MODE_QUIT;
    } else 
    if (is_it_an_arithmetical_expression(input)) {
        
        success_flag = STATUS_OK;
        if (!work_with_expression(input, *mode, &success_flag)) {
            if (success_flag == STATUS_OK) {
                puts("Syntax error!");
            }
        }
    
    } else {
        printf("Invalid command \"%s\"!\n", orig_input);
    }

    free(input);
    free(orig_input);

    return return_result;
}