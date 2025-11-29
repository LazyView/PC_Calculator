/**
 * @file main.c
 * @brief Main entry point for the calculator application
 */

#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Main function
 *
 * Handles command-line arguments and starts the calculator in
 * either interactive mode or file mode.
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char* argv[]) {
    /* File mode: ./calc <filename> */
    if (argc == 2) {
        if (!runFileMode(argv[1])) {
            printf("Invalid input file!\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    /* Interactive mode: ./calc */
    if (argc == 1) {
        runInteractiveMode();
        return EXIT_SUCCESS;
    }

    /* Invalid arguments */
    fprintf(stderr, "Usage: %s [input_file]\n", argv[0]);
    return EXIT_FAILURE;
}
