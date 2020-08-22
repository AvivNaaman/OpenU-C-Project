/* Contains general-purposed functions, for both passes and many usages */
#ifndef _UTILS_H
#define _UTILS_H

#include "globals.h"


/** moves the index to the next place in string where the char isn't white */
#define MOVE_TO_NOT_WHITE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;

/**
 * Concatenates both string to a new allocated memory
 * @param s0 The first string
 * @param s1 The second string
 * @return A pointer to the new, allocated string
 */
char *strallocat(char *s0, char* s1);

/**
 * Finds the defined label in the code if exists, and saves it into the buffer.
 * Returns whether syntax error found.
 * @param line The source line to find in
 * @param symbol_dest The buffer for copying the found label in
 * @return Whether syntax error found
 */
bool find_label(line_info line, char *symbol_dest);

/**
 * Returns the instruction enum by the instruction's name, without the opening '.'
 * @param name The instruction name, without the '.'
 * @return The instruction enum if found, NONE_INST if not found.
 */
instruction find_instruction_by_name(char *name);

/**
 * Returns whether the string is a valid 21-bit integer
 * @param string The number string
 * @return Whether a valid 21-bit signed integer.
 */
bool is_int(char* string);

/**
 * Allocates memory in the required size. Exits the program if failed.
 * @param size The size to allocate in bytes
 * @return A generic pointer to the allocated memory if succeeded
 */
void *malloc_with_check(long size);

/**
 * Returns whether a label can be defined with the specified name.
 * @param name The label name
 * @return Whether the specified name is valid,
 */
bool is_valid_label_name(char* name);

/**
 * Returns whether a string is alphanumeric.
 * @param string The string
 * @return Whether it's alphanumeric
 */
bool is_alphanumeric_str(char *string);

/*Returns TRUE if name is saved word*/
bool is_reserved_word(char *name);

/**
 * Prints a detailed error message, including file name and line number by the specified message,
 * formatted as specified in App. B of "The C Programming language" for printf.
 * @param message The error message
 * @param ... The arguments to format into the message
 * @return printf result of the message
 */
int printf_line_error(line_info line, char *message, ...);

/**
 * Frees all the dynamically-allocated memory for the code image.
 * @param code_image A pointer to the code images buffer
 * @param fic The final instruction counter value
 */
void free_code_image(machine_word **code_image, long fic);

#endif