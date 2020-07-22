#include <stdio.h>
#include "second_pass.h"
#include "code.h"
#include "utils.h"
#include "string.h"

int process_spass_operand(line_info line,long *curr_ic, long *ic, char *operand, machine_word **code_img, table *symbol_table);

/**
 * Processes a single line in the second pass
 * @param line The line string
 * @param ic A pointer to instruction counter
 * @param code_img Code image
 * @param symbol_table The symbol table
 * @return Whether operation succeeded
 */
bool process_line_spass(line_info line, long *ic, machine_word **code_img, table *symbol_table) {
	char *indexOfColon;
	char *token;
	long i = 0;
	MOVE_TO_NOT_WHITE(line.content,i)
	/* TODO: @AvivNaaman: This code is nice for the first pass. use it! */
	if(line.content[i]==';'||strcmp("\n",line.content)==0) return TRUE;
	indexOfColon = strchr(line.content, ':');
	/*check for label */
	if (indexOfColon != NULL) {
		i = indexOfColon - line.content;
		i++;
	}
	MOVE_TO_NOT_WHITE(line.content, i)
	/* .instruction */
	if (line.content[i] == '.') {
		/*if it's entry we add it to the symbol table*/
		if (strncmp(".entry", line.content, 6) == 0) {
			i += 6;
			MOVE_TO_NOT_WHITE(line.content, i)
			token = strtok(line.content+i, " \n\t");
			/* if label is already marked as entry, ignore. */
			if (token == NULL) {
				print_error(line, "You have to specify label name for .entry instruction.");
				return FALSE;
			}
			if (find_by_types(*symbol_table, token, 1, ENTRY_SYMBOL) == NULL) {
				table_entry *entry;
				token = strtok(line.content+i, "\n"); /*get name of label*/
				if(token[0] == '&') token++;
				/* if symbol is not defined as data/code */
				if ((entry = find_by_types(*symbol_table, token, 2, DATA_SYMBOL,CODE_SYMBOL)) == NULL) {
					/* if defined as external print error */
					if ((entry = find_by_types(*symbol_table, token, 1, EXTERNAL_SYMBOL)) != NULL) {
						print_error(line, "External symbol cannot be defined as an entry symbol as well.");
						return FALSE;
					}
					/* otherwise print more general error */
					print_error(line, "Symbol %s for .entry instruction not found.",token);
					return FALSE;
				}
				add_table_item(symbol_table, token, entry->value, ENTRY_SYMBOL);
			}
		}
		return TRUE;
	}
	return add_symbols_to_code(line, ic, code_img, symbol_table);
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
bool add_symbols_to_code(line_info line, long *ic, machine_word **code_img, table *symbol_table) {
	char temp[80];
	char *operands[2];
	int i = 0, operand_count;
	long curr_ic = *ic; /* using curr_ic as temp index inside the code image, in the current line code+data words */
	/* Get the total word length of current code text line in code binary image */
	int length = code_img[(*ic) - IC_INIT_VALUE]->length;
	/* if the length is 1, then there's only the code word, no data. */
	if (length > 1) {
		/* Now, we need to skip command, and get the operands themselves: */
		MOVE_TO_NOT_WHITE(line.content, i)
		parse_symbol(line, temp);
		if (temp[0] != '\0') { /* if symbol is defined */
			/* move i right after it's end */
			for (; line.content[i] && line.content[i] != '\n' && line.content[i] != EOF && line.content[i] != ' ' && line.content[i] != '\t'; i++);
			i++;
		}
		MOVE_TO_NOT_WHITE(line.content, i)
		/* now skip command */
		for (; line.content[i] && line.content[i] != ' ' && line.content[i] != '\t' && line.content[i] != '\n' && line.content[i] != EOF; i++);
		/* now analyze operands We send NULL as string of command because no error will be printed, and that's the only usage for it there. */
		analyze_operands(line, i, operands, &operand_count, NULL);
		/* Process both operands, if failed return failure. otherwise continue */
		if (!process_spass_operand(line, &curr_ic, ic, operands[0], code_img, symbol_table)) return FALSE;
		if (!process_spass_operand(line, &curr_ic, ic, operands[1], code_img, symbol_table)) return FALSE;
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
 * @param code_img The code image array
 * @param symbol_table The symbol table
 * @return Whether succeeded
 */
int process_spass_operand(line_info line, long *curr_ic, long *ic, char *operand, machine_word **code_img, table *symbol_table) {
	bool is_extern_symbol;
	addressing_type addr = get_addressing_type(operand);
	machine_word *word_to_write;
	/* if the word on *IC has the immediately addressed value (done in first pass), go to next cell (increase ic) */
	if (addr == IMMEDIATE_ADDR) (*curr_ic)++;
	if (addr == RELATIVE_ADDR) operand++;
	if (DIRECT_ADDR == addr || RELATIVE_ADDR == addr) {
		long data_to_add;
		table_entry *entry = find_by_types(*symbol_table, operand,3,DATA_SYMBOL,CODE_SYMBOL,EXTERNAL_SYMBOL);
		if (entry == NULL) {
			print_error(line, "Symbol %s not found",operand);
			return FALSE;
		}
		is_extern_symbol = entry->type == EXTERNAL_SYMBOL;
		/*found symbol*/
		data_to_add = entry->value;
		/* Calculate the distance to the label from ic if needed */
		if (addr == RELATIVE_ADDR) {
			/* if not code symbol it's impossible! */
			if (entry->type != CODE_SYMBOL) {
				print_error(line, "Symbol %s cannot be addressed relatively because it's not a code symbol.", operand);
				return FALSE;
			}
			data_to_add =  data_to_add - *ic;
		}
		/* Add to externals reference table if it's an external. increase ic because it's the next data word */
		if (is_extern_symbol) {
			add_table_item(symbol_table, operand, (*curr_ic) + 1, EXTERNAL_REFERENCE);
		}

		/*found symbol*/
		word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
		word_to_write->length = 0;
		word_to_write->word.data = build_data_word(addr, data_to_add, is_extern_symbol);
		code_img[(++(*curr_ic)) - IC_INIT_VALUE] = word_to_write;

	}
	return TRUE;
}
