#include "input_helpers.h"
#include "globals.h"
#include "string.h"

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction_type find_instruction_from_index(char *string, int index){
	char temp[MAX_LINE_LENGTH];
	int j;
	MOVE_TO_NOT_WHITE(string, index); /* get index to first not white place */
	if (string[index] != '.') return NONE;

	for (j = 0;string[index] && string[index] != '\t' && string[index] != ' ';index++,j++) {
		temp[j] = string[index];
	}

	if (strcmp(temp, ".extern") == 0) {
		return EXTERN;
	}
	else if (strcmp(temp, ".data") == 0) {
		return DATA;
	}
	else if (strcmp(temp, ".entry") == 0) {
		return ENTRY;
	}

}

