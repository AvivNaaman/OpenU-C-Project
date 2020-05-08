/* Contains major function that are related to the first pass */
#include "constants.h"
#include "code.h"
#include "utils.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"


/**
 * Processes a single line in the first pass
 * @param line The line text
 * @param datas The data symbol table
 * @param codes The code symbol table
 * @param externals The externals symbol table
 * @param IC A pointer to the current instruction counter
 * @param DC A pointer to the current data counter
 * @param code_img The code image array
 * @param data_img The data image array
 * @return Whether succeeded.
 */
bool process_line_fpass(char *line, long *IC, long *DC, machine_word **code_img, long *data_img,
                        table *symbol_table) {
	int i, j;
	char symbol[MAX_LINE_LENGTH];
	instruction instruction;

	i = 0;

	MOVE_TO_NOT_WHITE(line, i) /* Move to next non-white char */
	if (line[i] == '\n' || line[i] == EOF || line[i] == ';')
		return TRUE; /* Empty/Comment line - no errors found (of course) */

	/* Check if symbol (*:), stages 1.3-1.5 */
	/* if tried to define label, but it's invalid, return that an error occurred. */
	/* TODO: Remove double validation here \/ */
	if (parse_symbol(line, symbol)) {
		return FALSE;
	}

	/* if illegal name */
	if (!is_legal_label_name(symbol) && symbol[0]) {
		print_error("Illegal label name %s", symbol);
		return FALSE;
	}
	/* try using strtok instead... */
	if (symbol[0] != '\0') {
		for (; line[i] != ':'; i++); /* if symbol detected, start analyzing from it's deceleration end */
		i++;
	}
	MOVE_TO_NOT_WHITE(line, i) /* Move to next not-white char */
	if (line[i] == '\n') return TRUE;

	/* if already defined as data/external/code and not empty line */
	if (find_by_types(*symbol_table, symbol, 3, EXTERNAL_SYMBOL, DATA_SYMBOL, CODE_SYMBOL)) {
		print_error("Symbol %s is already defined.",symbol);
		return FALSE;
	}

	/* Check if it's an instruction (starting with '.') */
	instruction = find_instruction_from_index(line, &i);

	MOVE_TO_NOT_WHITE(line, i)

	/* is it's an instruction */
	if (instruction != NONE_INST) {
		/* if .string or .data, and symbol defined, put it into the symbol table */
		if ((instruction == DATA_INST || instruction == STRING_INST) && symbol[0] != '\0')
			/* is data or string, add DC with the symbol to the table as data */
			add_table_item(symbol_table, symbol, *DC, DATA_SYMBOL);

		/* if string, encode into data image buffer and increase dc as needed. */
		if (instruction == STRING_INST)
			return process_string_instruction(line, i, data_img, DC);
			/* if .data, do same but parse numbers. */
		else if (instruction == DATA_INST)
			return process_data_instruction(line, i, data_img, DC);
			/* if .extern, add to externals symbol table */
		else if (instruction == EXTERN_INST) {
			/* if label is defined before (e.g. LABEL: .extern something) */
			if (symbol[0] != '\0') {
				print_error("Can't define a label to an extern instruction.");
			}
			MOVE_TO_NOT_WHITE(line, i)
			/* if external symbol detected, start analyzing from it's deceleration end */
			for (j = 0; line[i] && line[i] != '\n' && line[i] != '\t' && line[i] != ' ' && line[i] != EOF; i++, j++) {
				symbol[j] = line[i];
			}
			symbol[j] = 0;
			/* TODO: Validate that symbol contains a valid symbol! */
			add_table_item(symbol_table, symbol, 0, EXTERNAL_SYMBOL); /* Extern value is defaulted to 0 */
		}
		/* if entry and symbol defined, print error */
		else if (instruction == ENTRY_INST && symbol[0] != '\0') {
			print_error("Can't define a label to an entry instruction.");
			return FALSE;
		}
		/* .entry is handled in second pass! */
	} /* end if (instruction != NONE) */
		/* not instruction=>it's a command! */
	else {
		/* if symbol defined, add it to the table */
		if (symbol[0] != '\0')
			add_table_item(symbol_table, symbol, *IC, CODE_SYMBOL);
		/* Analyze code */
		return process_code(line, i, IC, code_img);
	}
	return TRUE;
}

/**
 * Processes a single code line in the first pass.
 * Adds the code build binary structure to the code_img,
 * encodes immediately-addresses operands and leaves required data word that use labels NULL.
 * @param line The code line to process
 * @param i Where to start processing the line from
 * @param ic A pointer to the current instruction counter
 * @param code_img The code image array
 * @return Whether succeeded or notssss
 */
bool process_code(char *line, int i, long *ic, machine_word **code_img) {
	char operation[8]; /* stores the string of the current code operation */
	char *operands[2]; /* 2 strings, each for operand */
	opcode curr_opcode; /* the current opcode and funct values */
	funct curr_funct;
	code_word *codeword; /* The current code word */
	long ic_before;
	int j, operand_count;
	machine_word *word_to_write;
	/* Skip white chars */
	MOVE_TO_NOT_WHITE(line, i)

	/* Until white char, end of line, or too big op */
	for (j = 0; line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF && j < 6; i++, j++) {
		operation[j] = line[i];
	}
	operation[j] = '\0'; /* End of string */
	/* Get opcode & funct by command name into curr_opcode/curr_funct */
	get_opcode_func(operation, &curr_opcode, &curr_funct);
	/* If invalid operation, print and skip processing the line. */
	if (curr_opcode == NONE_OP) {
		print_error("Unrecognized operation: %s.", operation);
		return FALSE; /* an error occurred */
	}

	/* Analyze operands */
	if (!analyze_operands(line, i, operands, &operand_count, operation)) return FALSE; /* if error, return error */

	/* Build code word (returns null if validation failed) */
	if ((codeword = get_code_word(curr_opcode, curr_funct, operand_count, operands)) == NULL) return FALSE;

	ic_before = *ic;

	word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
	(word_to_write->word).code = codeword;

	code_img[(*ic) - IC_INIT_VALUE] = word_to_write; /* Avoid "spending" cells of the array */

	/* Build additional data words: */
	{
		addressing_type first_addr, second_addr;
		first_addr = get_addressing_type(operands[0]);
		second_addr = get_addressing_type(operands[1]);
		/* if an additional data word is required */
		if (first_addr != NONE_ADDR && first_addr != REGISTER_ADDR) {
			(*ic)++; /* increase ci */
			/* if the operand is immediately addressed, we can encode it right now: */
			if (first_addr == IMMEDIATE_ADDR) {
				char *ptr;
				/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to strtol */
				long value = strtol(operands[0] + 1, &ptr, 10);
				word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
				word_to_write->length = 0; /* Not code word! */
				(word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, FALSE);
				code_img[(*ic) - IC_INIT_VALUE] = word_to_write;

			}
		}
		/* And again - if another data word is required, increase CI. if it's an immediate addressing, encode it. */
		if (second_addr != NONE_ADDR && second_addr != REGISTER_ADDR) {
			(*ic)++;
			if (get_addressing_type(operands[1]) == IMMEDIATE_ADDR) {
				char *ptr;
				/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to strtol */
				long value = strtol(operands[1] + 1, &ptr, 10);
				word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
				word_to_write->length = 0; /* Not Code word! */
				(word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, FALSE);

				code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
			}
		}
	}

	(*ic)++; /* increase ci to point the next cell */

	/* Add the final length (of code word + data words) to the code word struct: */
	code_img[ic_before - IC_INIT_VALUE]->length = (*ic) - ic_before;

	return TRUE; /* No errors */
}