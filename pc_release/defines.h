/**
* @file defines.h - header file that contains predefined values for most 
* program files.
* @author Andrei Akhramchuk A21B0074P
* @version 1.0
* @date 07.01.2023
* @details Сontains predefined values for most program files.
* @copyright © 2022 Andrei Akhramchuk, All Rights Reserved.
*/
#ifndef DEFINES
#define DEFINES

/**************************************************************************
*                                CHARS                                    *
**************************************************************************/

/*
* Define of a new line char. 
*/
#define CHAR_NEW_LINE            '\n'

/*
* Define of an end line char symbol. 
*/
#define CHAR_END_LINE            '\0'

/*
* Define of a carriage char symbol, 
* is necessary because of on some operation systems end line is \r\n on 
* other just \n.
*/
#define CHAR_CARRIAGE_RETURN     '\r'

/*
* Define of a minus char symbol.
*/
#define CHAR_MINUS               '-'

/*
* Define of a space char symbol.
*/
#define CHAR_SPACE               ' '

/**
 * Define of a begin alphabet char A.
*/
#define CHAR_A                   'A'
/**
 * Define of an end alphabet char Z.
*/
#define CHAR_Z                   'Z'

/*
* Define of an end last hex allowed char f.
*/
#define CHAR_F                   'f'

/*
* Define of a char 0.
*/
#define CHAR_ZERO                '0'

/*
* Define of a char 10.
*/
#define CHAR_ONE                 '1'

/*
* Define of a char difference between the difference between small.
* and large letters
*/
#define CHAR_TO_LOWER_DIFFERENCE ('Z' - 'z')

/*
* Define of a char tabulation symbol.
*/
#define CHAR_TAB                 ((char)9)




/**************************************************************************
*                              STRINGS                                    *
**************************************************************************/

/*
* Define of a string that contains char 'zero' in string representation.
*/
#define STRING_ZERO          "0"

/*
* Define of a string that contains char 'one' in string representation.
*/
#define STRING_ONE           "1"

/*
* Define of a string that contains char 'minus' in string representation.
*/
#define STRING_MINUS         "-"

/*
* Define of an empty string.
*/
#define STRING_EMPTY         ""

/*
* Define of a string that represents hexadecimal base to decimal 
* that is 16.
*/
#define BASE_HEX             "16"

/*
* Define of a string that represents binary base to decimal that is 2.
*/
#define BASE_BIN             "2"




/**************************************************************************
*                         CONVERTATION STRINGS                            *
**************************************************************************/

/*
* Define of a string to convert '0' from hex representation 
* to binary representation "0001".
*/
#define HEX_TO_BIN_ZERO      "0000"

/*
* Define of a string to convert '0' from hex representation 
* to binary representation "0001".
*/
#define HEX_TO_BIN_ONE       "0001"

/*
* Define of a string to convert '2' from hex representation 
* to binary representation "0010".
*/
#define HEX_TO_BIN_TWO       "0010"

/*
* Define of a string to convert '3' from hex representation 
* to binary representation "0011".
*/
#define HEX_TO_BIN_THREE     "0011"

/*
* Define of a string to convert '4' from hex representation 
* to binary representation "0100".
*/
#define HEX_TO_BIN_FOUR      "0100"

/*
* Define of a string to convert '5' from hex representation 
* to binary representation "0101".
*/
#define HEX_TO_BIN_FIVE      "0101"

/*
* Define of a string to convert '6' from hex representation 
* to binary representation "0110".
*/
#define HEX_TO_BIN_SIX       "0110"

/*
* Define of a string to convert '7' from hex representation 
* to binary representation "0111".
*/
#define HEX_TO_BIN_SEVEN     "0111"

/*
* Define of a string to convert '8' from hex representation 
* to binary representation "1000".
*/
#define HEX_TO_BIN_EIGHT     "1000"

/*
* Define of a string to convert '9' from hex representation 
* to binary representation "1001".
*/
#define HEX_TO_BIN_NINE      "1001"

/*
* Define of a string to convert 'a' from hex representation 
* to binary representation "1010".
*/
#define HEX_TO_BIN_A         "1010" 

/*
* Define of a string to convert 'b' from hex representation 
* to binary representation "1011".
*/
#define HEX_TO_BIN_B         "1011"

/*
* Define of a string to convert 'c' from hex representation 
* to binary representation "1100".
*/
#define HEX_TO_BIN_C         "1100"

/*
* Define of a string to convert 'd' from hex representation 
* to binary representation "1101".
*/
#define HEX_TO_BIN_D         "1101"

/*
* Define of a string to convert 'e' from hex representation 
* to binary representation "1110".
*/
#define HEX_TO_BIN_E         "1110"

/*
* Define of a string to convert 'f' from hex representation 
* to binary representation "1111".
*/
#define HEX_TO_BIN_F         "1111"




/**************************************************************************
*                            POWERS OF TWO                                *
**************************************************************************/

/*
* Define of an int that represents 2^3.
*/
#define BIN_THIRD_DEGREE_TO_DEC      8

/*
* Define of an int that represents 2^2.
*/
#define BIN_SECOND_DEGREE_TO_DEC     4

/*
* Define of an int that represents 2^1.
*/
#define BIN_FIRST_DEGREE_TO_DEC      2

/*
* Define of an int that represents 2^0.
*/
#define BIN_ZERO_DEGREE_TO_DEC       1




/**************************************************************************
*                         HEXADECIMAL INFORMATION                         *
**************************************************************************/

/* 
* Define of an int value that defines the sign of a hexadecimal number 
* <8 - hex is positive, >= 8 hex is negative. 
*/
#define HEX_FIGURE_AS_SIGN_DIVIDER  8

/**
* Define of an int value for a string length that contains the length of the 
* binary representation of the hexadecimal number.
*/
#define HEX_FIGURE_LENGTH_IN_BINARY 4

/**
* Define of an int value for a string length that contains all available 
* characters for headecimal number representation.
*/
#define HEX_LENGTH_SYMBOLS_ALLOWED  16




/**************************************************************************
*                                 PREFIXES                                *
**************************************************************************/

/*
* Define of a string that represents binary prefix
*/
#define PREFIX_BIN           "0b"

/*
* Define of a string that represents binary positive prefix
*/
#define PREFIX_BIN_POSITIVE  "0b0"

/*
* Define of a string that represents binary negative prefix
*/
#define PREFIX_BIN_NEGATIVE  "0b1"

/*
* Define of a string that represents hexadecimal prefix
*/
#define PREFIX_HEX           "0x"

/*
* Define of a string that represents hexadecimal positive prefix
*/
#define PREFIX_HEX_POSITIVE  "0x0"

/*
* Define of a string that represents hexadecimal negative prefix
*/
#define PREFIX_HEX_NEGATIVE  "0xf"

/*
* Define of the prefix letter like 'x' or 'b' position. 
*/
#define PREFIX_SYMB_POSITION 1

/**
* Define of the prefix sign position
* HEX -> predefined prefix for negative is f, for positive is 0
* BIN -> predefined prefix for negative is 1, for positive is 0
*/
#define POSITION_SIGN        2





/**************************************************************************
*                                  SIGNS                                  *
**************************************************************************/

/*
* Define of an int value for '+' sign
*/
#define SIGN_POSITIVE  127

/*
* Define of an int value for '-' sign
*/
#define SIGN_NEGATIVE -127




/**************************************************************************
*                                  BASES                                  *
**************************************************************************/

/*
* Define of an int value for hexadecimal's base
*/
#define ARITHMETIC_BASE_HEX      16

/*
* Define of an int value for binary's base
*/
#define ARITHMETIC_BASE_DEC      10




/**************************************************************************
*                             ALLOWED SYMBOLS                             *
**************************************************************************/

/*
* Define of a string that represents symbols that are allowed 
* in math expression.
*/
#define SYMBOLS_ALLOWED         "0123456789abcdefx!^-*/%+()"

/*
* Define of a string that represents symbols that are operands.
*/
#define SYMBOLS_OPERANDS        "!^-*/%+()_"

/*
* Define of a string that represents symbols that are allowed 
* in hexadecimal number.
*/
#define SYMBOLS_HEX_ALLOWED     "0123456789abcdef"

/*
* Define of a string that represents symbols that are allowed 
* in binary number.
*/
#define SYMBOLS_BIN_ALLOWED     "01"

/*
* Define of a string that represents symbols that are allowed 
* in decimal number.
*/
#define SYMBOLS_DEC_ALLOWED     "0123456789"

/*
* Define of a string that represents latters that are allowed 
* in hexadecimal number.
*/
#define SYMBOLS_LETTERS_ALLOWED "abcdef"

/*
* Define of a string that represents brackets. 
*/
#define SYMBOLS_BRACKETS        "()"




/**************************************************************************
*                               OPERANDS                                  *
**************************************************************************/

/*
* Define of an operand that represents a minus.
*/
#define OPERAND_MINUS       '-'

/*
* Define of an operand that represents a plus.
*/
#define OPERAND_PLUS        '+'

/*
* Define of an operand that represents a modulo.
*/
#define OPERAND_MOD         '%'

/*
* Define of an operand that represents a division.
*/
#define OPERAND_DIV         '/'

/*
* Define of an operand that represents a multiplication.
*/
#define OPERAND_MULTIPLY    '*'

/*
* Define of an operand that represents an unary minus.
*/
#define OPERAND_UNARY_MINUS '_'

/*
* Define of an operand that represents a power.
*/
#define OPERAND_POWER       '^'

/*
* Define of an operand that represents a factorial.
*/
#define OPERAND_FACTORIAL   '!'




/**************************************************************************
*                               PRECENDENCES                              *
**************************************************************************/

/*
* Define of an int value for an erroneous operands priority.
*/
#define PRECEDENCE_MINUS -1

/*
* Define of an int value - minus and plus operands priority.
*/
#define PRECEDENCE_ZERO   0

/*
* Define of an int value - modulo operand priority.
*/
#define PRECEDENCE_ONE    1

/*
* Define of an int value - divide and multiply operands priority.
*/
#define PRECEDENCE_TWO    2

/*
* Define of an int value - power operand priority.
*/
#define PRECEDENCE_THREE  3

/*
* Define of an int value - unary minus operand priority.
*/
#define PRECEDENCE_FOUR   4

/*
* Define of an int value - factorial operand priority.
*/
#define PRECEDENCE_FIVE   5




/**************************************************************************
*                                ADDITIONS                                *
**************************************************************************/

/* ALOCATE SIZES */
#define ALLOCATE_SIZE       4096
#define DEFAULT_STRING_SIZE 10000

/*
* Define of an int value of program's arguments count.
*/
#define ARGUMENT_COUNT 2

/*
* Define of an int value for function error status.
*/
#define STATUS_NOT_OK -1

/*
* Define of an int value for function OK status.
*/
#define STATUS_OK      1




/**************************************************************************
*                                 MODES                                   *
**************************************************************************/

/**
* Define of a char for decimal mode identification. 
*/
#define MODE_DEC  'd'

/**
* Define of a char for binary mode identification. 
*/
#define MODE_BIN  'b'

/**
* Define of a char for hexadecimal mode identification. 
*/
#define MODE_HEX  'h'

/**
* Define of a char for exit(quit) command identification. 
*/
#define MODE_QUIT 'q'

/**
* Define of a string for decimal mode identification. 
*/
#define MENU_DEC  "dec"

/**
* Define of a string for binary mode identification. 
*/
#define MENU_BIN  "bin"

/**
* Define of a string for hexadecimal mode identification. 
*/
#define MENU_HEX  "hex"

/**
* Define of a string for "out" command identification. 
*/
#define MENU_OUT  "out"

/**
* Define of a string for exit(quit) command identification. 
*/
#define MENU_QUIT "quit"

/*
* Defines of max and min functions.
*/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif