#include "utils.h"
#include "globals.h"
#include <ctype.h>

/* Returns whether a string contains a symbol from a certain index. copies the symbol (if exists) to the string argument */
int parse_symbol(char *line, int i, char *parsed_symbol_dest) {
	int j;
	for (j=0; line[i] && line[i] != ':' && (line[i] != '\t' && line[i] != ' ') ; i++)
		parsed_symbol_dest[j] = line[i]; /* Go on until empty char OR symbol */

	/* if IS symbol: (if the last char of the first word in line is ':' */
	if (line[i] == ':')
		return TRUE;

	parsed_symbol_dest[0] = '\0';
	return FALSE;
}



/* Writes a 24-bit word to a buffer in the specified index */
void write_word(char* buffer, int index, char byte0, char byte1, char byte2) {
	buffer[index++] = byte0;
	buffer[index++] = byte1;
	buffer[index++] = byte2;
}

int is_int(char* string) {
	int i;
	for (i = 0; string[i]; i++) {
		if (!isdigit(string[i])) {
			return FALSE;
		}
	}
	return TRUE;
}