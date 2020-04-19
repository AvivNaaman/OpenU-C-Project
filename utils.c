#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Returns whether a string contains a symbol from a certain index. copies the symbol (if exists) to the string argument */
char *parse_symbol(char *line) {
	int j, i;
	char *to_return;
	i = j = 0;

	/* Skip white chars at the beginning TODO: Check if necessary */
	MOVE_TO_NOT_WHITE(line, i);

	/* Label should start with alpha char */
	if (!isalpha(line[i])) {
		printf("Error: label must start with a letter");
		return NULL;
	}
	/* Let's allocate some memory to the string needed to be returned */
	to_return = malloc_with_check(sizeof(char) * 32); /* Max length +1 */
	if (to_return == NULL) return NULL;
	for (; line[i] && line[i] != ':' && i <= 31; i++, j++) {
		to_return[j] = line[i]; /* Go on until empty char OR symbol */
		/* Label must be alphanumeric! */
		if (!isalnum(line[i])) {
			free(to_return);
			printf("Error: label must start with a letter, contain letters and digits and end with ':'.");
			return NULL;
		}
	}
	to_return[j] = '\0'; /* End of string */

	/* if IS symbol: (if the last char of the first word in line is ':' */
	if (line[i] == ':')
		return to_return;

	printf("Error: label miss ':' right after it's end");
	return NULL;
}


/* Writes a 24-bit word to a buffer in the specified index */
void write_word(char *buffer, int dc, char byte0, char byte1, char byte2) {
	dc *= 3; /* TODO explain! */
	buffer[dc++] = byte0;
	buffer[dc++] = byte1;
	buffer[dc] = byte2;
}


int is_int(char *string) {
	int i = 0;
	if (string[0] == '-' || string[0] == '+') i++; /* if string starts with +/-, it's OK */
	for (; string[i]; i++) { /* Just make sure that everything is a digit until the end */
		if (!isdigit(string[i])) {
			return FALSE;
		}
	}
	return i > 0; /* if i==0 then it was an empty string! */
}

char *int_to_word(int num) {
	char *word = (char *) calloc(3, sizeof(char));
	if (word == NULL) {
		printf("Memory Allocation Failed");
		exit(1);
	}
	/* Shift bits if needed and keep only eight (2bytes) */
	word[0] = (num >> 16) & 0xFF;
	word[1] = (num >> 8) & 0xFF;
	word[2] = num & 0xFF;
	return word;
}

void *malloc_with_check(long size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Memory allocation failed");
		exit(1);
	}
	return ptr;
}

int is_legal_label(char *string) {
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

int is_contain_non_alphanumeric(char *string) {
	int i;
	/*check for every char in string if it is non alphanumeric char if it is function returns true*/
	for (i = 0; string[i]; i++) {
		if (!(isalpha(string[i]) || isdigit(string[i]))) return TRUE;
	}
	return FALSE;
}

int is_saved_word(char *string) {
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