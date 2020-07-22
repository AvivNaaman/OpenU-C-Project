#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction find_instruction_from_index(char *string, int *index) {
	char temp[MAX_LINE_LENGTH];
	int j;
	MOVE_TO_NOT_WHITE(string, *index) /* get index to first not white place */
	if (string[*index] != '.') return NONE_INST;

	for (j = 0; string[*index] && string[*index] != '\t' && string[*index] != ' '; (*index)++, j++) {
		temp[j] = string[*index];
	}
	temp[j] = '\0'; /* End of string */

	if (strcmp(temp, ".extern") == 0) {
		return EXTERN_INST;
	} else if (strcmp(temp, ".data") == 0) {
		return DATA_INST;
	} else if (strcmp(temp, ".entry") == 0) {
		return ENTRY_INST;
	} else if (strcmp(temp, ".string") == 0) {
		return STRING_INST;
	}
	return ERROR_INST; /* starts with '.' but not a valid instruction! */
}

/* Instruction line processing helper functions */
/*
 * Processes a string instruction from index. encode into data image and change dc.
 * Returns whether encountered an error.
 */
bool process_string_instruction(char *line, int index, long *data_img, long *dc) {
	long data;
	MOVE_TO_NOT_WHITE(line, index)
	if (line[index] != '"') {
		/* something like: LABEL: .string  hello, world\n - the string isn't surrounded with "" */
		print_error("String must be defined between quotation marks");
		return FALSE;
	} else {
		index++;
		/* Foreach char between the two " */
		for (; line[index] != '"' && line[index] && line[index] != '\n' && line[index] != EOF; index++) {
			/* Save the char into the data image */
			data = line[index];
			data_img[*dc] = data;
			(*dc)++;
		}
		/* Add end of string ('\0') */
		data_img[*dc] = 0;
		(*dc)++;
	}
	if (line[index] != '"') {
		print_error("String must be defined between quotation marks");
		return FALSE;
	}
	/* Return processed chars count */
	return TRUE;
}

/*
 * Parses a .data instruction. copies each number value to data_img by dc position, and returns the amount of processed data.
 */
bool process_data_instruction(char *line, int index, long *data_img, long *dc) {
	char temp[80], *temp_ptr;
	long value;
	int i;
	MOVE_TO_NOT_WHITE(line, index)
	if (line[index] == ',') {
		print_error("Unexpected comma after .data instruction");
	}
	do {
		for (i = 0;
		     line[index] && line[index] != EOF && line[index] != '\t' && line[index] != ' ' && line[index] != ',' &&
		     line[index] != '\n'; index++, i++) {
			temp[i] = line[index];
		}
		temp[i] = '\0'; /* End of string */
		if (!is_int(temp)) {
			print_error("Expected integer for .data instruction (got '%s')", temp);
			return FALSE;
		}
		/* Now let's write to data buffer */
		value = strtol(temp, &temp_ptr, 10);

		/* TODO: Check if validation required here! */

		data_img[*dc] = value;

		(*dc)++; /* a word was written right now */
		MOVE_TO_NOT_WHITE(line, index)
		/* TODO: Some error if something (Think!) */
		if (line[index] == ',') index++;
		else if (!line[index] || line[index] == '\n' || line[index] == EOF)
			break; /* End of line/file/string => nothing to process anymore */
		/* Got comma. Skip white chars and check if end of line (if so, there's extraneous comma!) */
		MOVE_TO_NOT_WHITE(line, index)
		if (line[index] == ',') {
			print_error("Multiple consecutive commas.");
			return FALSE;
		} else if (line[index] == EOF || line[index] == '\n' || !line[index]) {
			print_error("Missing data after comma");
			return FALSE;
		}
	} while (line[index] != '\n' && line[index] != EOF);
	return TRUE;
}
