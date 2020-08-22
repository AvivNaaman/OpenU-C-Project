/* Contains major function that are related to the first pass */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "code.h"
#include "utils.h"
#include "instructions.h"
#include "first_pass.h"


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
static bool process_code(line_info line, int i, long *ic, machine_word **code_img);

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
bool process_line_fpass(line_info line, long *IC, long *DC, machine_word **code_img, long *data_img,
                        table *symbol_table) {
	int i, j;
	char symbol[MAX_LINE_LENGTH];
	instruction instruction;

	i = 0;

	MOVE_TO_NOT_WHITE(line.content, i) /* Move to next non-white char */
	if (!line.content[i] || line.content[i] == '\n' || line.content[i] == EOF || line.content[i] == ';')
		return TRUE; /* Empty/Comment line - no errors found (of course) */

	/* Check if symbol (*:), stages 1.3-1.5 */
	/* if tried to define label, but it's invalid, return that an error occurred. */
	if (find_label(line, symbol)) {
		return FALSE;
	}

	/* if illegal name */
	if (symbol[0] && !is_valid_label_name(symbol)) {
		printf_line_error(line, "Illegal label name: %s", symbol);
		return FALSE;
	}
	/* try using strtok instead... */
	if (symbol[0] != '\0') {
		for (; line.content[i] != ':'; i++); /* if symbol detected, start analyzing from it's deceleration end */
		i++;
	}

	MOVE_TO_NOT_WHITE(line.content, i) /* Move to next not-white char */

	if (line.content[i] == '\n') return TRUE; /* Label-only line - skip */

	/* if already defined as data/external/code and not empty line */
	if (find_by_types(*symbol_table, symbol, 3, EXTERNAL_SYMBOL, DATA_SYMBOL, CODE_SYMBOL)) {
		printf_line_error(line, "Symbol %s is already defined.", symbol);
		return FALSE;
	}

	/* Check if it's an instruction (starting with '.') */
	instruction = find_instruction_from_index(line, &i);

	if (instruction == ERROR_INST) { /* Syntax error found */
		return FALSE;
	}

	MOVE_TO_NOT_WHITE(line.content, i)

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
			MOVE_TO_NOT_WHITE(line.content, i)
			/* if external symbol detected, start analyzing from it's deceleration end */
			for (j = 0; line.content[i] && line.content[i] != '\n' && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != EOF; i++, j++) {
				symbol[j] = line.content[i];
			}
			symbol[j] = 0;
			/* If invalid external label name, it's an error */
			if (!is_valid_label_name(symbol)) {
				printf_line_error(line, "Invalid external label name: %s", symbol);
				return TRUE;
			}
			add_table_item(symbol_table, symbol, 0, EXTERNAL_SYMBOL); /* Extern value is defaulted to 0 */
		}
			/* if entry and symbol defined, print error */
		else if (instruction == ENTRY_INST && symbol[0] != '\0') {
			printf_line_error(line, "Can't define a label to an entry instruction.");
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
 * Allocates and builds the data inside the additional code word by the given operand,
 * Only in the first pass
 * @param code_img The current code image
 * @param ic The current instruction counter
 * @param operand The operand to check
 */
static void build_extra_codeword_fpass(machine_word **code_img, long *ic, char *operand);

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
static bool process_code(line_info line, int i, long *ic, machine_word **code_img) {
	char operation[8]; /* stores the string of the current code instruction */
	char *operands[2]; /* 2 strings, each for operand */
	opcode curr_opcode; /* the current opcode and funct values */
	funct curr_funct;
	code_word *codeword; /* The current code word */
	long ic_before;
	int j, operand_count;
	machine_word *word_to_write;
	/* Skip white chars */
	MOVE_TO_NOT_WHITE(line.content, i)

	/* Until white char, end of line, or too big instruction, copy it: */
	for (j = 0; line.content[i] && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != '\n' && line.content[i] != EOF && j < 6; i++, j++) {
		operation[j] = line.content[i];
	}
	operation[j] = '\0'; /* End of string */
	/* Get opcode & funct by command name into curr_opcode/curr_funct */
	get_opcode_func(operation, &curr_opcode, &curr_funct);
	/* If invalid operation (opcode is NONE_OP=-1), print and skip processing the line. */
	if (curr_opcode == NONE_OP) {
		printf_line_error(line, "Unrecognized instruction: %s.", operation);
		return FALSE; /* an error occurred */
	}

	/* Separate operands and get their count */
	if (!analyze_operands(line, i, operands, &operand_count, operation))  {
		return FALSE;
	}

	/* Build code word struct to store in code image array */
	if ((codeword = get_code_word(line, curr_opcode, curr_funct, operand_count, operands)) == NULL) {
		/* Release allocated memory for operands */
		if (operands[0]) {
			free(operands[0]);
			if (operands[1]) {
				free(operands[1]);
			}
		}
		return FALSE;
	}

	/* ic in position of new code word */
	ic_before = *ic;

	/* allocate memory for a new word in the code image, and put the code word into it */
	word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
	(word_to_write->word).code = codeword;
	code_img[(*ic) -
	         IC_INIT_VALUE] = word_to_write; /* Avoid "spending" cells of the array, by starting from initial value of ic */

	/* Build extra information code word if possible, free pointers with no need */
	if (operand_count--) { /* If there's 1 operand at least */
		build_extra_codeword_fpass(code_img, ic, operands[0]);
		free(operands[0]);
		if (operand_count) { /* If there are 2 operands */
			build_extra_codeword_fpass(code_img, ic, operands[1]);
			free(operands[1]);
		}
	}

	(*ic)++; /* increase ic to point the next cell */

	/* Add the final length (of code word + data words) to the code word struct: */
	code_img[ic_before - IC_INIT_VALUE]->length = (*ic) - ic_before;

	return TRUE; /* No errors */
}

static void build_extra_codeword_fpass(machine_word **code_img, long *ic, char *operand) {
	addressing_type operand_addressing = get_addressing_type(operand);
	/* And again - if another data word is required, increase CI. if it's an immediate addressing, encode it. */
	if (operand_addressing != NONE_ADDR && operand_addressing != REGISTER_ADDR) {
		(*ic)++;
		if (operand_addressing == IMMEDIATE_ADDR) {
			char *ptr;
			machine_word *word_to_write;
			/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to strtol */
			long value = strtol(operand + 1, &ptr, 10);
			word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
			word_to_write->length = 0; /* Not Code word! */
			(word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, FALSE);

			code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
		}
	}
}