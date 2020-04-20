#include "instructions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction_type find_instruction_from_index(char *string, int index){
	char temp[MAX_LINE_LENGTH];
	int j;
	MOVE_TO_NOT_WHITE(string, index) /* get index to first not white place */
	if (string[index] != '.') return NONE_INST;

	for (j = 0;string[index] && string[index] != '\t' && string[index] != ' ';index++,j++) {
		temp[j] = string[index];
	}
	temp[j] = '\0'; /* End of string */

	if (strcmp(temp, ".extern") == 0) {
		return EXTERN;
	}
	else if (strcmp(temp, ".data") == 0) {
		return DATA;
	}
	else if (strcmp(temp, ".entry") == 0) {
		return ENTRY;
	}
	else if (strcmp(temp, ".string") == 0) {
		return STRING;
	}
	return NONE_INST;
}

/* Instruction line processing helper functions */
/*
 * Processes a string instruction from index. encode into data image and change dc.
 * Returns whether encountered an error.
 */
int process_string_instruction(char *line, int index, machine_data **data_img, int *dc) {
	machine_data *data;
	int dc_at_start = *dc;
	MOVE_TO_NOT_WHITE(line, index)
	if (line[index] == '"') {
		index++;
		/* Foreach char between the two " */
		for (;line[index] != '"' && line[index] && line[index] != '\n' && line[index] != EOF;index++) {
			data = malloc_with_check(sizeof(machine_data));
			data->byte0 = data->byte1 = '\0';
			data->byte2 = line[index];
			data_img[*dc] = data;
			(*dc)++;
		}
	}
	else {
		/* something like: 'LABEL: .string  hello, world\n' - the string isn't surrounded with "" */
		print_error("String must be defined between quotation marks");
		return TRUE;
	}
	if (line[index] != '"') {
		print_error("String must be defined between quotation marks");
		return TRUE;
	}
	if (*dc == dc_at_start) { /* Nothing was added */
		print_error("Error: Empty string definition");
		return TRUE;
	}
	/* Return processed chars count */
	return FALSE;
}

/*
 * Parses a .data instruction. copies each number value to data_img by dc position, and returns the amount of processed data.
 */
int process_data_instruction(char *line, int index, machine_data **data_img, int *dc) {
	char temp[80], *temp_ptr;
	machine_data *data;
	int i;
	do {
		MOVE_TO_NOT_WHITE(line, index)
		for (i = 0; line[index] && line[index] != EOF && line[index] != '\t' && line[index] != ' ' && line[index] != ',' ; index++,i++) {
			temp[i] = line[index];
		}
		temp[i] = '\0'; /* End of string */
		if (!is_int(temp)) {
			print_error("Expected a number after .data instruction");
			return TRUE;
		}
		/* Now let's write to data buffer */
		temp_ptr = int_to_word(atoi(temp));
		data = (machine_data *)malloc_with_check(sizeof(machine_data));
		data->byte0 = temp_ptr[0];
		data->byte1 = temp_ptr[1];
		data->byte2 = temp_ptr[2];

		data_img[*dc] = data;

		free(temp_ptr);
		(*dc)++; /* a word was written right now */
		MOVE_TO_NOT_WHITE(line, index)
		/* TODO: Some error if something (Think!) */
		if (line[index] == ',') index++;
		else if (!line[index] || line[index] == '\n' || line[index] == EOF) break; /* End of line/file/string => nothing to process anymore */
	} while (line[index] != '\n' && line[index] != EOF);
	return FALSE;
}