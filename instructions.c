#include "instructions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "utils.h"

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction_type find_instruction_from_index(char *string, int index){
	char temp[MAX_LINE_LENGTH];
	int j;
	MOVE_TO_NOT_WHITE(string, index) /* get index to first not white place */
	if (string[index] != '.') return NONE;

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
	return NONE;
}

/* Instruction line processing helper functions */
/*
 * Processes a line data instruction. puts 2 empty bytes in the data_img array and then 2bytes of the char, for each char.
 * Returns the count of processed chars.
 * */
int process_string_instruction(char *line, int index, char* data_img, int data_img_indx) {
	int data_added_counter; /* counts processed chars amount */
	data_added_counter = 0;
	MOVE_TO_NOT_WHITE(line, index)
	if (line[index] == '"') {
		index++;
		/* Foreach char between the two " */
		for (;line[index] != '"' && line[index] && line[index] != '\n' && line[index] != EOF;index++) {
			/* ASCII char is 1byte but one word is 3byte. we need to insert 2 zero-bytes, and then the actual data */
			write_word(data_img, data_img_indx, '\0','\0',line[index]);
			data_img_indx++;
			data_added_counter++; /* counter that will be returned, indicating the number of processed chars */
		} /* TODO: Error when no second " at the end! */
	}
	else {
		/* something like: 'LABEL: .string  hello, world\n' - the string isn't surrounded with "" */
		printf("String must be defined between quotation marks");
		return 0;
	}
	if (data_added_counter == 0) {
		printf("Error: Empty string definition");
	}
	/* Return processed chars count */
	return data_added_counter;
}

/*
 * Parses a .data instruction. copies each number value to data_img by dc position, and returns the amount of processed data.
 */
int process_data_instruction(char *line, int index, char *data_img, int dc) {
	char temp[80], byte0, byte1, byte2, *temp_ptr;
	int i, curr_val, data_counter;
	data_counter = 0;
	do {
		MOVE_TO_NOT_WHITE(line, index)
		for (i = 0; line[index] && line[index] != EOF && line[index] != '\t' && line[index] != ' ' && line[index] != ',' ; index++,i++) {
			temp[i] = line[index];
		}
		temp[i] = '\0'; /* End of string */
		if (!is_int(temp)) {
			/* TODO: Write error and return flag that indicates error (somehow) */
			printf("Error: line : expected number after .data instruction (got %s)", temp);
			return FALSE;
		}
		/* Now let's write to data buffer */
		temp_ptr = int_to_word(atoi(temp));
		write_word(data_img, dc+data_counter, temp_ptr[0], temp_ptr[1], temp_ptr[2]); /* TODO: Free allocated data for temp_ptr! */
		data_counter++; /* a word was written right now */
		MOVE_TO_NOT_WHITE(line, index)
		/* TODO: Some error if something (Think!) */
		if (line[index] == ',') index++;
		else if (!line[index] || line[index] == '\n' || line[index] == EOF) break; /* End of line/file/string => nothing to process anymore */
	} while (line[index] != '\n' && line[index] != EOF);
	return data_counter;
}