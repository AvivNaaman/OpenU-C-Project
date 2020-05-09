#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"
#include "assembler.h"
#include "code.h" /* for checking reserved words */

/* TODO: Refactor! */
/* Returns whether an error ocurred during the try of parsing the symbol. puts the symbol into the second buffer. */
bool parse_symbol(char *line, char *symbol_dest) {
	int j, i;
	bool isvalid; /* Indexes + can it be a valid label */
	i = j = 0;

	isvalid = TRUE;
	/* Skip white chars at the beginning TODO: Check if necessary */
	MOVE_TO_NOT_WHITE(line, i)

	/* Label should start with alpha char */
	if (!isalpha(line[i])) {
		isvalid = FALSE;
	}
	/* Let's allocate some memory to the string needed to be returned */
	for (; line[i] && line[i] != ':' && i <= 100; i++, j++) {
		symbol_dest[j] = line[i]; /* Go on until empty char OR symbol */
		/*max length of label is 32 characters*/
		if (j == 31) {
			isvalid = FALSE;
		}
		/* Label must be alphanumeric! */
		if (!isalnum(line[i])) {
			isvalid = FALSE;
		}
	}
	symbol_dest[j] = '\0'; /* End of string */

	/* if it was a try to define label, print errors if needed. */
	if (line[i] == ':') {
		if (!isvalid) {
			print_error(
					"Label must start with a letter, contain letters and digits only, do not exceed thirty-two characters long and end with ':'.");
			symbol_dest[0] = '\0';
			return TRUE; /* No valid symbol, and no try to define one */
		}
		return FALSE;
	}
	symbol_dest[0] = '\0';
	return FALSE; /* There was no error */
}


bool is_int(char *string) {
	int i = 0;
	if (string[0] == '-' || string[0] == '+') string++; /* if string starts with +/-, it's OK */
	for (; string[i]; i++) { /* Just make sure that everything is a digit until the end */
		if (!isdigit(string[i])) {
			return FALSE;
		}
	}
	/* TODO: Check if it's a 21-bit signed integer */
	return i > 0; /* if i==0 then it was an empty string! */
}

void *malloc_with_check(long size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		print_error("Memory allocation failed");
		exit(1);
	}
	return ptr;
}

bool is_valid_label_name(char *name) {
	/* Check length, first char is alpha and all the others are alphanumeric, and not saved word */
	return name[0] && strlen(name) <= 31 && isalpha(name[0]) && is_alphanumeric_str(name + 1) &&
	       !is_reserved_word(name);
}

bool is_alphanumeric_str(char *string) {
	int i;
	/*check for every char in string if it is non alphanumeric char if it is function returns true*/
	for (i = 0; string[i]; i++) {
		if (!isalpha(string[i]) && !isdigit(string[i])) return FALSE;
	}
	return TRUE;
}

bool is_reserved_word(char *name) {
	int fun, opc;
	/* check if not a command (jmp,cmp,etc.) using the method from code.h. we need only opc, which has to be NONE_OP if name isn't a command. */
	get_opcode_func(name, &opc, (funct *) &fun);
	if (opc != NONE_OP) return FALSE;
		/* check if not a register */
	else if (get_register_by_name(name) != NONE_REG) return FALSE;

/* TODO: See definition of reserved word again! */
	else if (strcmp(name, "entry") == 0) {
		return TRUE;
	} else if (strcmp(name, "extern") == 0) {
		return TRUE;
	} else if (strcmp(name, ".data") == 0) {
		return TRUE;
	} else if (strcmp(name, ".name") == 0) {
		return TRUE;
	}
	return FALSE;
}

/* Prints a detailed error message to the user, including file, line, and message. */
int print_error(char *message, ...) {
	int result;
	va_list arglist; /* for formatting */
	/* Print file+line */
	printf("Error: In file %s:%ld: ", get_curr_filename(), get_curr_line());
	/* use vprintf to call printf from variable argument function (from stdio.h) with message + format */
	va_start(arglist, message);
	result = vprintf(message, arglist);
	puts(""); /* Line break */
	return result;
}

void free_code_image(machine_word **code_image, long fic) {
	long i;
	/* foreach non-null cell */
	for (i = 0; i < fic; i++) {
		machine_word *curr_word = code_image[i];
		if (curr_word != NULL) {
			if (curr_word->length > 0) {
				free(curr_word->word.code);
			} else {
				free(curr_word->word.data);
			}
			free(curr_word);
			code_image[i] = NULL;
		}
	}
}
