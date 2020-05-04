#include <stdio.h>
#include "second_pass.h"
#include "code.h"
#include "utils.h"
#include "string.h"

int process_spass_operand(long *curr_ic, long *ic, char *operand, table data, table code, table externals, table *external_references, machine_word **code_img);

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
	/* TODO: @AvivNaaman: This code is nice for the first pass. use it! */
	if(line[i]==';'||strcmp("\n",line)==0) return TRUE;
	indexOfColon = strchr(line, ':');
	/*check for label */
	if (indexOfColon != NULL) {
		i = indexOfColon - line;
		i++;
	}
	MOVE_TO_NOT_WHITE(line, i)
	/* .instruction */
	if (line[i] == '.') {
		/*if it's entry we add it to the symbol table*/
		if (strncmp(".entry", line, 6) == 0) {
			i += 6;
			MOVE_TO_NOT_WHITE(line, i);
			token = strtok(line, " ");
			/*if label is already in table dont add it*/
			if (find_by_key(*ent_table, token) == NULL) {
				table_entry *entry;
				token = strtok(NULL, "\n"); /*get name of label*/
				if(token[0] == '&') token++;
				entry = find_by_key(data_table, token);
				if(entry == NULL){
					entry = find_by_key(code_table, token);
					if(entry == NULL){
						print_error("symbol not found");
						return FALSE;
					}
				}
				add_table_item(ent_table, token, entry->value);
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
	int i = 0, operand_count;
	long curr_ic = *ic; /* using curr_ic as temp index inside the code image, in the current line code+data words */
	/* Get the total word length of current code text line in code binary image */
	int length = code_img[(*ic) - IC_INIT_VALUE]->length;
	/* if the length is 1, then there's only the code word, no data. */
	if (length > 1) {
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
		/* Process both operands, if failed return failure. otherwise continue */
		if (!process_spass_operand(&curr_ic, ic, operands[0],data_table,code_table,ext_table,ext_references,code_img)) return FALSE;
		if (!process_spass_operand(&curr_ic, ic, operands[1],data_table,code_table,ext_table,ext_references,code_img)) return FALSE;
	}
	/* Make the current pass IC as the next line ic */
	(*ic) = (*ic)+length;
	return TRUE;
}

/**
 * Builds the additional data word for operand in the second pass, if needed.
 * @param curr_ic Current instruction pointer of source code line
 * @param ic Current instruction pointer of source code line start
 * @param operand The operand string
 * @param data The data symbol table
 * @param code The code symbol table
 * @param externals The externals symbol table
 * @param external_references The external references symbol table
 * @param code_img The code image array
 * @return Whetehr succeeded
 */
int process_spass_operand(long *curr_ic, long *ic, char *operand, table data, table code, table externals, table *external_references, machine_word **code_img) {
	bool is_extern_symbol = FALSE;
	addressing_type addr = get_addressing_type(operand);
	machine_word *word_to_write;
	/* if the word on *IC has the immediately addressed value (done in first pass), go to next cell (increase ic) */
	if (addr == IMMEDIATE) (*curr_ic)++;
	if (addr == RELATIVE) operand++;
	if (DIRECT == addr || RELATIVE == addr) {
		long data_to_add;
		table_entry *entry = find_by_key(data, operand);
		if (entry == NULL) {
			entry = find_by_key(code, operand);
			if (entry == NULL) {
				entry = find_by_key(externals, operand);
				if (entry == NULL) /* Symbol not found! */{
					print_error("Symbol not found.");
					return FALSE;
				}
				is_extern_symbol = TRUE;
			}
		}
		/*found symbol*/
		data_to_add = entry->value;
		/* Calculate the distance to the label from "here" */
		/* TODO: Fix relative addressing doesn't work properly! */
		if (addr == RELATIVE) {
			data_to_add =  data_to_add - *ic;
		}

		/* Add to externals reference table if it's an external. increase ic because it's the next data word */
		if (is_extern_symbol) {
			add_table_item(external_references, operand, (*curr_ic)+1);
		}

		/*found symbol*/
		word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
		word_to_write->length = 0;
		word_to_write->word.data = build_data_word(addr, data_to_add, is_extern_symbol);
		code_img[(++(*curr_ic)) - IC_INIT_VALUE] = word_to_write;

	}
	return TRUE;
}
