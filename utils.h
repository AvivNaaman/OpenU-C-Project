#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

/* TODO: Refactor */
bool parse_symbol(char *line, char *symbol_dest);

/**
 * Returns whether the string is a valid 21-bit integer
 * @param string The number string
 * @return Whether a valid 21-bit signed integer.
 */
bool is_int(char* string);

/* TODO: Remove this function and migrate to the one used in code.c! */
/**
 * Converts a 21-bit signed integer into
 * @return A pointer to 3-byte char array which holds the bytes from high to low.
 */
char *int_to_word(long);

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
bool is_legal_label_name(char* name);

/**
 * Returns whether a string is alphanumeric.
 * @param string The string
 * @return Whether it's alphanumeric
 */
bool is_alphanumeric_str(char *string);

/*Returns TRUE if string is saved word*/
bool is_saved_word(char *string);

/* Prints a detailed error message to the user, including file, line, and message. */
void print_error(char *message);

/* Frees all the allocated memory inside the code image */
void free_code_image(machine_word **code_image, long fic);

/* Frees all the allocated memory inside the data image */
void free_data_image(machine_data ** data_image, long fdc);

#endif