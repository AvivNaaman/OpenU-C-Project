#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

/* Returns whether a string contains a symbol from a certain index. copies the symbol (if exists) to the string argument */
char *parse_symbol(char *line) {
	int j,i;
	i = j = 0;
	char *result;

	/* Skip white chars at the beginning TODO: Check if necessary */
	MOVE_TO_NOT_WHITE(line, i);

	/* Label should start with alpha char */
	if (!isalpha(line[i])) {
		printf("Error: label must start with a letter");
		return NULL;
	}
	/* Let's allocate some memory to the string needed to be returned */
	char *to_return = malloc_with_check(sizeof(char) * 32); /* Max length +1 */
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
void write_word(char *buffer, int index, char byte0, char byte1, char byte2) {
	buffer[index++] = byte0;
	buffer[index++] = byte1;
	buffer[index] = byte2;
}

int is_int(char *string) {
	int i;
	for (i = 0; string[i]; i++) {
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
	if (ptr == NULL) printf("Memory allocation failed");
	return ptr;
}