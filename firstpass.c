#include <stdio.h>
#include "firstpass.h"
#include "utils.h"
#include "instructions.h"

/*
 * processes a single instruction line. returns whether an error occured.
 * line - the text line from .asm file.
 * datas, code, externals - symbol tables for .data, .code and .external instruction.
 * IC - a pointer to the instruction counter.
 * DC - a pointer to the data counter.
 * code_img - the code image array.
 * data_img - the data image array.
 */
int process_line_fpass(char *line, table datas, table codes, table externals, int *IC, int *DC, char *code_img,
                       char *data_img, char *filename) {
	int i;
	int is_symbol;
	char temp[80];
	char *symbol;
	/* TODO: implement */

	i = 0;

	MOVE_TO_NOT_WHITE(line, i) /* Move to next non-white char */
	if (line[i] == '\n') return FALSE; /* Empty line - no errors found (of course) */

	/* Check if symbol (*:), stages 1.3-1.5 */
	symbol = parse_symbol(line);


	MOVE_TO_NOT_WHITE(line, i) /* Move to next not-white char */

	/* Check if it's an instruction (starting with '.') */
	instruction_type instruction = find_instruction_from_index(line, i);

	MOVE_TO_NOT_WHITE(line, i);

	if (instruction != NONE) {
		/* if .string or .data, put it into the symbol table */
		if (instruction == DATA || instruction == STRING) {
			if (symbol != NULL) {
				/* is data or string, add DC with the symbol to the table */
				add_table_item(&datas, temp, *DC);
			}
		}
		/* if string, encode into data image buffer and increase dc as needed. */
		if (instruction == STRING) {
			(*DC) += process_string_instruction(line, i, data_img, DC);
		}
		/* if .data, do same. */
		else if (instruction == DATA) {
			/* TODO: Implement. add to dc size of 24-bit words added to data_img. */
			(*DC) += process_data_instruction(line, i, data_img, DC);
		}
		/* if .extern, add to externs symbol table */
		else if (instruction == EXTERN) {
			add_table_item(&externals, temp, *DC);
		}
	} /* end if (instruction != NONE) */
	else { /* It's command 1.11 -1.17*/

	}
	return FALSE;
}
