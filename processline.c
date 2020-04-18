/* Contains a method for processing a line in the first pass and another for processing a line in the second pass. */
#include <stdio.h>
#include "processline.h"
#include "instructions.h"
#include "code.h"

/*
 * processes a single instruction line. returns whether an error occurred.
 * line - the text line from .asm file.
 * datas, code, externals - symbol tables for .data, .code and .external instruction.
 * IC - a pointer to the instruction counter.
 * DC - a pointer to the data counter.
 * code_img - the code image array.
 * data_img - the data image array.
 */
bool process_line_fpass(char *line, table *datas, table *codes, table *externals, int *IC, int *DC, char *code_img,
                       char *data_img, char *filename) {
	int i;
	char temp[80];
	char *symbol;
	instruction_type instruction;
	/* TODO: implement */

	i = 0;

	MOVE_TO_NOT_WHITE(line, i) /* Move to next non-white char */
	if (line[i] == '\n' || line[i] == EOF || line[i] == ';') return FALSE; /* Empty/Comment line - no errors found (of course) */

	/* Check if symbol (*:), stages 1.3-1.5 */
	symbol = parse_symbol(line);


	MOVE_TO_NOT_WHITE(line, i) /* Move to next not-white char */

	/* Check if it's an instruction (starting with '.') */
	instruction = find_instruction_from_index(line, i);

	MOVE_TO_NOT_WHITE(line, i);

	/* is it's an instruction */
	if (instruction != NONE) {
		/* if .string or .data, and symbol defined, put it into the symbol table */
		if ((instruction == DATA || instruction == STRING) && symbol != NULL)
			/* is data or string, add DC with the symbol to the table */
			add_table_item(datas, temp, *DC);
		/* if string, encode into data image buffer and increase dc as needed. */
		if (instruction == STRING)
			(*DC) += process_string_instruction(line, i, data_img, *DC);
			/* if .data, do same but parse numbers. */
		else if (instruction == DATA)
			(*DC) += process_data_instruction(line, i, data_img, *DC);
			/* if .extern, add to externals symbol table */
		else if (instruction == EXTERN)
			add_table_item(externals, temp, *DC);
		/* .entry is handled in second pass! */
	} /* end if (instruction != NONE) */
		/* not instruction=>it's a command! */
	else {
		/* if symbol defined, add it to the table */
		if (symbol != NULL)
			add_table_item(codes, temp, *IC);
		/* Analyze code */
		process_code(line, i); /* TODO: Shouldn't it encode into memory?! */
	}
	return FALSE;
}

bool process_line_spass(char *line, table *ent_table, table *code_table, int *ic, table ext_table, table data_table) {
	/* Todo: Implement */
	return TRUE;
}