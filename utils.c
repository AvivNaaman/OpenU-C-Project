#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "assembler.h"

/* Returns whether an error ocurred during the try of parsing the symbol. puts the symbol into the second buffer. */
bool parse_symbol(char *line, char *symbol_dest) {
	int j, i;
	bool isvalid; /* Indexes + can it be a valid label */
	i = j = 0;

	isvalid = TRUE;
	/* Skip white chars at the beginning TODO: Check if necessary */
	MOVE_TO_NOT_WHITE(line, i);

	/* Label should start with alpha char */
	if (!isalpha(line[i])) {
		isvalid = FALSE;
	}
	/* Let's allocate some memory to the string needed to be returned */
	for (; line[i] && line[i] != ':' && i <= 31; i++, j++) {
		symbol_dest[j] = line[i]; /* Go on until empty char OR symbol */
		/* Label must be alphanumeric! */
		if (!isalnum(line[i])) {
			isvalid = FALSE;
		}
	}
	symbol_dest[j] = '\0'; /* End of string */

	/* if it was a try to define label, print errors if needed. */
	if (line[i] == ':')
	{
		if (!isvalid) {
			print_error("Label must start with a letter, contain letters and digits only and end with ':'.");
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

char *int_to_word(long num) {
	char *word = (char *) malloc_with_check(3* sizeof(char));
	/* Shift bits if needed and keep only eight (2bytes) */
	word[0] = (num >> 16) & 0xFF;
	word[1] = (num >> 8) & 0xFF;
	word[2] = num & 0xFF;
	return word;
}

void *malloc_with_check(long size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		print_error("Memory allocation failed");
		exit(1);
	}
	return ptr;
}

bool is_legal_label_name(char *string) {
	/*if the length of the label is more than 31 characters it's illegal name*/
	if (strlen(string) > 31) return FALSE;
		/*if the string doesn't start with a letter it's illegal name*/
	else if (!isalpha(string[0])) return FALSE;
		/*if the string contains non alphanumeric characters it's illegal name*/
	else if (is_contain_non_alphanumeric(string)) return FALSE;
		/*if the string is saved word it's illegal name*/
	else if (is_saved_word(string)) return FALSE;
	return TRUE;


}

bool is_contain_non_alphanumeric(char *string) {
	int i;
	/*check for every char in string if it is non alphanumeric char if it is function returns true*/
	for (i = 0; string[i]; i++) {
		if (!(isalpha(string[i]) || isdigit(string[i]))) return TRUE;
	}
	return FALSE;
}

bool is_saved_word(char *string) {
	/*compare to every saved word*/
	if (strcmp(string, "mov") == 0) {
		return TRUE;
	} else if (strcmp(string, "cmp") == 0) {
		return TRUE;
	} else if (strcmp(string, "add") == 0) {
		return TRUE;
	} else if (strcmp(string, "sub") == 0) {
		return TRUE;
	} else if (strcmp(string, "lea") == 0) {
		return TRUE;
	} else if (strcmp(string, "clr") == 0) {
		return TRUE;
	} else if (strcmp(string, "not") == 0) {
		return TRUE;
	} else if (strcmp(string, "inc") == 0) {
		return TRUE;
	} else if (strcmp(string, "dec") == 0) {
		return TRUE;
	} else if (strcmp(string, "jmp") == 0) {
		return TRUE;
	} else if (strcmp(string, "bne") == 0) {
		return TRUE;
	} else if (strcmp(string, "jsr") == 0) {
		return TRUE;
	} else if (strcmp(string, "red") == 0) {
		return TRUE;
	} else if (strcmp(string, "prn") == 0) {
		return TRUE;
	} else if (strcmp(string, "rts") == 0) {
		return TRUE;
	} else if (strcmp(string, "stop") == 0) {
		return TRUE;
	} else if ((strcmp(string, "r0") == 0) || (strcmp(string, "r1") == 0) || (strcmp(string, "r2") == 0) ||
	           (strcmp(string, "r3") == 0) ||
	           (strcmp(string, "r4") == 0) || (strcmp(string, "r5") == 0) || (strcmp(string, "r6") == 0) ||
	           (strcmp(string, "r7") == 0)) {
		return TRUE;
	} else if (strcmp(string, "entry") == 0) {
		return TRUE;
	} else if (strcmp(string, "extern") == 0) {
		return TRUE;
	} else if (strcmp(string, ".data") == 0) {
		return TRUE;
	} else if (strcmp(string, ".string") == 0) {
		return TRUE;
	}
	return FALSE;
}

/* Prints a detailed error message to the user, including file, line, and message. */
void print_error(char *message) {
	printf("Error: In file %s:%ld: %s\n",get_curr_filename(),get_curr_line(),message);
}

void free_code_image(machine_word **code_image, long fic) {
	long i;
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

void free_data_image(machine_data ** data_image, long fdc) {
	long i;
	for (i = 0; i < fdc; i++) {
		if (data_image[i] != NULL) {
			free(data_image[i]);
			data_image[i] = NULL;
		}
	}
}