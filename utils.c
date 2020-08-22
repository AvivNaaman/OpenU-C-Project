#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"
#include "code.h" /* for checking reserved words */

#define ERR_OUTPUT_FILE stderr

char *strallocat(char *s0, char* s1) {
	char *str = (char *)malloc_with_check(strlen(s0) + strlen(s1) + 1);
	strcpy(str, s0);
	strcat(str, s1);
	return str;
}


/* Returns whether an error occurred during the try of parsing the symbol. puts the symbol into the second buffer. */
bool find_label(line_info line, char *symbol_dest) {
	int j, i;
	i = j = 0;

	/* Skip white chars at the beginning anyway */
	MOVE_TO_NOT_WHITE(line.content, i)

	/* Let's allocate some memory to the string needed to be returned */
	for (; line.content[i] && line.content[i] != ':' && line.content[i] != EOF && i <= MAX_LINE_LENGTH; i++, j++) {
		symbol_dest[j] = line.content[i];
	}
	symbol_dest[j] = '\0'; /* End of string */

	/* if it was a try to define label, print errors if needed. */
	if (line.content[i] == ':') {
		if (!is_valid_label_name(symbol_dest)) {
			printf_line_error(line,
			                  "Invalid label name - cannot be longer than 32 chars, may only start with letter be alphanumeric.");
			symbol_dest[0] = '\0';
			return TRUE; /* No valid symbol, and no try to define one */
		}
		return FALSE;
	}
	symbol_dest[0] = '\0';
	return FALSE; /* There was no error */
}


struct instruction_lookup_item {
	char *name;
	instruction value;
};

static struct instruction_lookup_item
		instructions_lookup_table[] = {
		{"string", STRING_INST},
		{"data",   DATA_INST},
		{"entry",  ENTRY_INST},
		{"extern", EXTERN_INST},
		{NULL, NONE_INST}
};

instruction find_instruction_by_name(char *name) {
	struct instruction_lookup_item *curr_item;
	for (curr_item = instructions_lookup_table; curr_item->name != NULL; curr_item++) {
		if (strcmp(curr_item->name, name) == 0) {
			return curr_item->value;
		}
	}
	return NONE_INST;
}

bool is_int(char *string) {
	int i = 0;
	if (string[0] == '-' || string[0] == '+') string++; /* if string starts with +/-, it's OK */
	for (; string[i]; i++) { /* Just make sure that everything is a digit until the end */
		if (!isdigit(string[i])) {
			return FALSE;
		}
	}
	return i > 0; /* if i==0 then it was an empty string! */
}

void *malloc_with_check(long size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Error: Fatal: Memory allocation failed.");
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
	/* check if register or command */
	get_opcode_func(name, &opc, (funct *) &fun);
	if (opc != NONE_OP || get_register_by_name(name) != NONE_REG || find_instruction_by_name(name) != NONE_INST) return TRUE;

	return FALSE;
}

int printf_line_error(line_info line, char *message, ...) { /* Prints the errors into a file, defined above as macro */
	int result;
	va_list args; /* for formatting */
	/* Print file+line */
	fprintf(ERR_OUTPUT_FILE,"Error In %s:%ld: ", line.file_name, line.line_number);

	/* use vprintf to call printf from variable argument function (from stdio.h) with message + format */
	va_start(args, message);
	result = vfprintf(ERR_OUTPUT_FILE, message, args);
	va_end(args);

	fprintf(ERR_OUTPUT_FILE, "\n");
	return result;
}

void free_code_image(machine_word **code_image, long fic) {
	long i;
	/* for each not-null cell (we might have some "holes", so we won't stop on first null) */
	for (i = 0; i < fic; i++) {
		machine_word *curr_word = code_image[i];
		if (curr_word != NULL) {
			/* free code/data word */
			if (curr_word->length > 0) {
				free(curr_word->word.code);
			} else {
				free(curr_word->word.data);
			}
			/* free the pointer to the union */
			free(curr_word);
			code_image[i] = NULL;
		}
	}
}
