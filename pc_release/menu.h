/**
* @file menu.h - header file of menu.c
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef MENU
#define MENU

/**
* This function will simulate the program when reading from a file passed as a program argument.
* @param str - lines that have been read from files and should be processed.
* @param count_of_lines - number of lines that were read from the file.
*/
void work_with_file(char **str, int* count_of_lines);

/**
* This function will simulate the program from the console.
*/
void start_work();

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
char menu(char* mode, char* str);

#endif