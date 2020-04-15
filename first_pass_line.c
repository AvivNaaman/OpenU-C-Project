#include "first_pass_line.h"
#include "globals.h"
#include "input_helpers.h"

/* Returns whether a string contains a symbol from a certain index. */
int parse_symbol(char *line, int, char*);

/* TODO: DOC */
int process_string_instruction(char *line, int index, char *, int);

/*
 * processes a single instruction line. returns whether an error occured.
 * line - the text line from .asm file.
 * datas, code, externals - symbol tables for .data, .code and .external instruction.
 * IC - a pointer to the instruction counter.
 * DC - a pointer to the data counter.
 * code_img - the code image array.
 * data_img - the data image array.
 */
int firstpass_analyze_line(char *line, table datas, table codes, table externals, int *IC, int *DC, char *code_img, char *data_img) {
	int i;
	int is_symbol, is_instruction;
	char temp[80];
	/* TODO: implement */

	MOVE_TO_NOT_WHITE(line, i); /* Move to next not-white char */
	if (line[i] == '\n') return FALSE; /* Empty line */

	/* Check if symbol (*:), stages 1.3-1.5 */
	is_symbol = parse_symbol(line, i, temp);



	MOVE_TO_NOT_WHITE(line, i); /* Move to next not-white char */

	/* Check if it's an instruction (starting with '.') */
	instruction_type instruction = find_instruction_from_index(line, i);

	MOVE_TO_NOT_WHITE(line, i);

	if (instruction != NONE) {
		if (instruction == DATA || instruction == STRING) {
			if (is_symbol) {
				/* is data or string, add DC with the symbol to the table */
				add_item(datas, temp, *DC);
			}
			/* If instruction is string, encode it */
			if (instruction == STRING) {
				(*DC) += process_string_instruction(line, i, data_img, DC);
			}
			/* Handle number(s) */
			else {
				/* TODO: Implement. add to dc size of 24-bit words added to data_img. */
			}
		}
		else if (instruction == EXTERN) {
			add_item(externals, temp, *DC);
		}
	} /* end if (instruction != NONE) */
	else { /* It's command 1.11 -1.17*/

	}
	return FALSE;
}

/* Returns whether a string contains a symbol from a certain index. copies the symbol (if exists) to the string argument */
int parse_symbol(char *line, int i, char *parsed_symbol) {
	int j;
	for (j=0; line[i] && line[i] != ':' && (line[i] != '\t' && line[i] != ' ') ; i++)
		parsed_symbol[j] = line[i]; /* Go on until empty char OR symbol */

	/* if IS symbol: (if the last char of the first word in line is ':' */
	if (line[i] == ':')
		return TRUE;

	parsed_symbol[0] = '\0';
	return FALSE;
}

/*
 * Processes a line data instruction. puts 2 empty bytes in the data_img array and then 2bytes of the char, for each char.
 * Returns the count of processed chars.
 * */
int process_string_instruction(char *line, int index, char* data_img, int data_img_indx) {
	int cntr; /* counts processed chars amount */
	data_img_indx *= 3; /* We need to find location in char array. each word is 3-byte, char in ansi-c is 1-byte */
	MOVE_TO_NOT_WHITE(line, index)
	if (line[index] == '"') {
		index++;
		/* Foreach char between the two " */
		for (cntr = 0;line[index] != '"';index++) {
			/* ASCII char is 1byte but one word is 3byte. we need to insert 2 zero-bytes, and then the actual data */
			data_img[data_img_indx++] = '\0';
			data_img[data_img_indx++] = '\0';
			data_img[data_img_indx++] = line[index];
		}
	}
	/* Return processed chars count */
	return cntr;
}