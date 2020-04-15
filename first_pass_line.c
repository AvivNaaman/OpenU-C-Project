#include "first_pass_line.h"
#include "globals.h"
#include "input_helpers.h"

/* Returns whether a string contains a symbol from a certain index. */
int has_symbol(char *line, int);

/*
 * processes a single instruction line. returns whether an error occured.
 * line - the text line from .asm file.
 * datas, code, externals - symbol tables for .data, .code and .external instruction.
 * IC - a pointer to the instruction counter.
 * DC - a pointer to the data counter.
 */
int firstpass_analyze_line(char *line, table datas, table codes, table externals, int *IC, int *DC) {
	int i;
	int is_symbol, is_instruction;
	/* TODO: implement */

	MOVE_TO_NOT_WHITE(line, i); /* Move to next not-white char */
	if (line[i] == '\n') return FALSE; /* Empty line */

	/* Check if symbol (*:), stages 1.3-1.5 */
	is_symbol = has_symbol(line, i);

	MOVE_TO_NOT_WHITE(line, i); /* Move to next not-white char */

	/* Check if it's an instruction (starting with '.') */
	instruction_type instruction = find_instruction_from_index(line, i);
	if (instruction == NONE)

	return FALSE;
}

/* Returns whether a string contains a symbol from a certain index. */
int has_symbol(char *line, int i) {

	for (i = 0; line[i] && line[i] != ':' && !is_white_char(line[i]) ; i++) ; /* Go on until empty char OR symbol */

	/* if IS symbol: (if the last char of the first word in line is ':' */
	if (line[i] == ':')
		return TRUE;

	return FALSE;
}
