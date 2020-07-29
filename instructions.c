#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

struct instruction_lookup_item {
	char *name;
	instruction value;
};

static struct instruction_lookup_item
		instructions_lookup_table[] = {
		{".string", STRING_INST},
		{".data",   DATA_INST},
		{".entry",  ENTRY_INST},
		{".extern", EXTERN_INST},
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

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction find_instruction_from_index(line_info line, int *index) {
	char temp[MAX_LINE_LENGTH];
	int j;
	instruction result;

	MOVE_TO_NOT_WHITE(line.content, *index) /* get index to first not white place */
	if (line.content[*index] != '.') return NONE_INST;

	for (j = 0; line.content[*index] && line.content[*index] != '\t' && line.content[*index] != ' '; (*index)++, j++) {
		temp[j] = line.content[*index];
	}
	temp[j] = '\0'; /* End of string */
	/* if invalid instruction but starts with ., return error */
	if ((result = find_instruction_by_name(temp)) != NONE_INST) return result;
	print_error(line, "Invalid instruction name: %s", temp);
	return ERROR_INST; /* starts with '.' but not a valid instruction! */
}

/* Instruction line processing helper functions */

bool process_string_instruction(line_info line, int index, long *data_img, long *dc) {
	long data;
	MOVE_TO_NOT_WHITE(line.content, index)
	char temp_str[MAX_LINE_LENGTH];
	char *last_quote_location = strrchr(line.content, '"');
	if (line.content[index] != '"') {
		/* something like: LABEL: .string  hello, world\n - the string isn't surrounded with "" */
		print_error(line, "Missing opening quote of string");
		return FALSE;
	} else if (&line.content[index] == last_quote_location) { /* last quote is same as first quote */
		print_error(line, "Missing closing quote of string");
		return FALSE;
	} else {
		int i;
		/* Copy the string including quotes & everything until end of line */
		for (i = 0;line.content[index] && line.content[index] != '\n' &&
		       line.content[index] != EOF; index++,i++) {
				temp_str[i] = line.content[index];
		}
		/* TODO: Aviv: Run over it again! */
		/* Put string terminator instead of last quote: */
		temp_str[last_quote_location - line.content] = '\0';
		for(i = 1;temp_str[i] && temp_str[i] != '"'; i++) {
			/* sort of strcpy but with dc increment */
			data_img[*dc] = temp_str[i];
			(*dc)++;
		}
		/* Put string terminator */
		data_img[*dc] = '\0';
		(*dc)++;
	}
	/* Return processed chars count */
	return TRUE;
}

/*
 * Parses a .data instruction. copies each number value to data_img by dc position, and returns the amount of processed data.
 */
bool process_data_instruction(line_info line, int index, long *data_img, long *dc) {
	char temp[80], *temp_ptr;
	long value;
	int i;
	MOVE_TO_NOT_WHITE(line.content, index)
	if (line.content[index] == ',') {
		print_error(line, "Unexpected comma after .data instruction");
	}
	do {
		for (i = 0;
		     line.content[index] && line.content[index] != EOF && line.content[index] != '\t' &&
		     line.content[index] != ' ' && line.content[index] != ',' &&
		     line.content[index] != '\n'; index++, i++) {
			temp[i] = line.content[index];
		}
		temp[i] = '\0'; /* End of string */
		if (!is_int(temp)) {
			print_error(line, "Expected integer for .data instruction (got '%s')", temp);
			return FALSE;
		}
		/* Now let's write to data buffer */
		value = strtol(temp, &temp_ptr, 10);

		data_img[*dc] = value;

		(*dc)++; /* a word was written right now */
		MOVE_TO_NOT_WHITE(line.content, index)
		/* TODO: Some error if something (Think!) */
		if (line.content[index] == ',') index++;
		else if (!line.content[index] || line.content[index] == '\n' || line.content[index] == EOF)
			break; /* End of line/file/string => nothing to process anymore */
		/* Got comma. Skip white chars and check if end of line (if so, there's extraneous comma!) */
		MOVE_TO_NOT_WHITE(line.content, index)
		if (line.content[index] == ',') {
			print_error(line, "Multiple consecutive commas.");
			return FALSE;
		} else if (line.content[index] == EOF || line.content[index] == '\n' || !line.content[index]) {
			print_error(line, "Missing data after comma");
			return FALSE;
		}
	} while (line.content[index] != '\n' && line.content[index] != EOF);
	return TRUE;
}
