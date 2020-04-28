#include <stdio.h>
#include "second_pass.h"
#include "code.h"
#include "utils.h"
#include "string.h"

/**
 * Processes a single line in the second pass
 * @param line The line string
 * @param ent_table The entries symbol table
 * @param ext_references The externals symbol table
 * @param code_table The code symbol table
 * @param ic A pointer to instruction counter
 * @param ext_table Externals table
 * @param data_table Data table
 * @param code_img Code image
 * @return Whether operation succeeded
 */
bool process_line_spass(char *line, table *ent_table, table *ext_references, table code_table, long *ic, table ext_table,
                        table data_table, machine_word **code_img) { /*TODO: DEBUG*/
	char *indexOfColon;
	char *token;
	int i = 0;
	MOVE_TO_NOT_WHITE(line,i)
	if(line[i]==';') return TRUE;
	indexOfColon = strchr(line, ':');
	/*check for label */
	if (indexOfColon != NULL) {
		i = indexOfColon - line;
		i++;
	}
	/* if it's guide line*/
	if (line[i] == '.') {
		/*if it's entry we add it to the symbol table*/
		if (strncmp(".entry", line, 6) == 0) {
			i += 6;
			MOVE_TO_NOT_WHITE(line, i);
			token = strtok(line, " ");
			/*if label is already in table dont add it*/
			if (find_by_key(*ent_table, token) == NULL) {
				table_entry *entry;
				int val;
				token = strtok(NULL, "\n"); /*get name of label*/
				if(token[0] == '&') token++;
				entry = find_by_key(data_table, token);
				if(entry == NULL){
					entry = find_by_key(code_table, token);
					if(entry == NULL){
						print_error("symbol not found");
						return FALSE;
					}
					val = entry->value;
				}
				add_table_item(ent_table, token, val);
			}
		}
		return TRUE;
	}
	return add_symbols_to_code(line, ic, code_img, data_table, code_table, ext_table, ext_references);
}

/**
 * Find the symbol that need replacment in a code line, and replacing them by the address in the symbol table.
 * @param line The current code line that is being processed
 * @param ic A pointer to the current instruction counter
 * @param code_img The machine code image array
 * @param data_table The data symbol table
 * @param code_table The code symbol table
 * @param ext_table The externals symbol table
 * @param ext_references A pointer to the external symbols references table
 * @return whether succeeded
 */
bool add_symbols_to_code(char *line, long *ic, machine_word **code_img,
                    table data_table, table code_table, table ext_table, table *ext_references) {
	char temp[80];
	char *operands[2];
	int i = 0, operand_count, curr_ic = *ic;
	/* Get the total word length of current code text line in code binary image */
	int length = code_img[(*ic) - IC_INIT_VALUE]->length;
	/* if the length is 1, then there's only the code word, no data. */
	if (length > 1) {
		addressing_type op1_addr, op2_addr;
		machine_word *word_to_write;
		bool is_extern_symbol = FALSE;
		/* Now, we need to skip command, and get the operands themselves: */
		MOVE_TO_NOT_WHITE(line, i)
		parse_symbol(line, temp);
		if (temp[0] != '\0') { /* if symbol is defined */
			/* move i right after it's end */
			for (; line[i] && line[i] != '\n' && line[i] != EOF && line[i] != ' ' && line[i] != '\t'; i++);
			i++;
		}
		MOVE_TO_NOT_WHITE(line, i)
		/* now skip command */
		for (; line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF; i++);
		/* now analyze operands */
		analyze_operands(line, i, operands, &operand_count);
		/* Now check each operand addressing, determine whether we should change anything and if so, change that thing: */
		op1_addr = get_addressing_type(operands[0]);
		op2_addr = get_addressing_type(operands[1]);
		/* if relative, move the pointer to 2nd char (the cymbol itself) */
		if (operands[0][0] == '&') operands[0]++;
		if (operands[1][0] == '&') operands[0]++;
		if (op1_addr == DIRECT || op1_addr == RELATIVE) {
			table_entry *entry = find_by_key(data_table, operands[0]);
			if (entry == NULL) {
				entry = find_by_key(code_table, operands[0]);
				if (entry == NULL) {
					entry = find_by_key(ext_table, operands[0]);
					if (entry == NULL)/* Symbol not found! */{
						print_error("Symbol not found.");
						return FALSE;
					}
					is_extern_symbol = TRUE;
				}
			}
			/*found symbol*/
			long data_to_add = entry->value;
			/* Calculate the distance to the label from "here" */
			if (op1_addr == RELATIVE) {
				data_to_add =  data_to_add - *ic;
			}

			/* Add to externals reference table if it's an external. add 1 to address because it's the word after the code word itself. */
			if (is_extern_symbol) {
				add_table_item(ext_references, operands[0], curr_ic+1);
			}

			word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
			word_to_write->length = 0; /* it's a data word */
			word_to_write->word.data = build_data_word(op1_addr, data_to_add, is_extern_symbol); /* build data word and put it in place: */
			code_img[(++(curr_ic)) - IC_INIT_VALUE] = word_to_write;
		}
		if (DIRECT == op2_addr || RELATIVE == op2_addr) {
			is_extern_symbol = FALSE;
			table_entry *entry = find_by_key(data_table, operands[1]);
			if (entry == NULL) {
				entry = find_by_key(code_table, operands[1]);
				if (entry == NULL) {
					entry = find_by_key(ext_table, operands[1]);
					if (entry == NULL)/* Symbol not found! */{
						print_error("Symbol not found.");
						return FALSE;
					}
					is_extern_symbol = TRUE;
				}
			}
			/*found symbol*/
			long data_to_add = entry->value;
			/* Calculate the distance to the label from "here" */
			if (op1_addr == RELATIVE) {
				data_to_add = *ic - data_to_add;
			}

			/* Add to externals reference table if it's an external. increase ic because it's the next data word */
			if (is_extern_symbol) {
				add_table_item(ext_references, operands[1], curr_ic+1);
			}

			/*found symbol*/
			word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
			word_to_write->length = 0;
			word_to_write->word.data = build_data_word(op2_addr, data_to_add, is_extern_symbol);
			code_img[(++(curr_ic)) - IC_INIT_VALUE] = word_to_write;

		}
	}
	/* Make the current pass IC as the next line ic */
	(*ic) = (*ic)+length;
	return TRUE;
}

