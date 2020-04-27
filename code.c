#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "code.h"
#include "table.h"


int process_code(char *line, int i, int *ic, machine_word **code_img) {
	char operation[8]; /* stores the string of the current code operation */
	char *operands[2]; /* 2 strings, each for operand */
	opcode curr_opcode; /* the current opcode and funct values */
	funct curr_funct;
	code_word *codeword; /* The current code word */
	int j, operand_count, ic_before;
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
		print_error("Unrecognized command.");
		return TRUE; /* an error occurred */
	}
	/* Analyze operands */

	if (analyze_operands(line, i, operands, &operand_count)) return TRUE; /* if error, return error */

	/* Build code word (returns null if validation failed) */
	if ((codeword = get_code_word(curr_opcode, curr_funct, operand_count, operands)) == NULL) return TRUE;

	ic_before = *ic;

	word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
	(word_to_write->word).code = codeword;

	code_img[(*ic) - IC_INIT_VALUE] = word_to_write; /* Avoid "spending" cells of the array */

	{
		addressing_type first_addr, second_addr;
		first_addr = get_addressing_type(operands[0]);
		second_addr = get_addressing_type(operands[1]);
		/* if an additional data word is required */
		if (first_addr != NONE_ADDR && first_addr != REGISTER) {
			(*ic)++; /* increase ci */
			/* if the operand is immediately addressed, we can encode it right now: */
			if (first_addr == IMMEDIATE) {
				char *ptr;
				/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to strtol */
				int value = strtol(operands[0] + 1, &ptr, 10);
				machine_word *word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
				word_to_write->length = 0; /* Not code word! */
				(word_to_write->word).data = build_data_word(IMMEDIATE, value);
				code_img[(*ic) - IC_INIT_VALUE] = word_to_write;

			}
		}
		/* And again - if another data word is required, increase CI. if it's an immediate addressing, encode it. */
		if (second_addr != NONE_ADDR && second_addr != REGISTER) {
			(*ic)++;
			if (get_addressing_type(operands[1]) == IMMEDIATE) {
				char *ptr;
				/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to strtol */
				int value = strtol(operands[1] + 1, &ptr, 10);
				word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
				word_to_write->length = 0; /* Not Code word! */
				(word_to_write->word).data = build_data_word(IMMEDIATE, value);

				code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
			}
		}
	}

	(*ic)++; /* increase ci to point the next cell */

	/* Add the final length (of code word + data words) to the code word struct: */
	code_img[ic_before - IC_INIT_VALUE]->length = (*ic) - ic_before;

	return FALSE; /* No errors */
}

/* Completes the assembling process  */
int
add_symbols_to_code(char *line, int *ic, machine_word **code_img, table code_table, table data_table, table ext_table) {
	char temp[80];
	char *operands[2];
	int i = 0, operand_count, curr_ic = *ic;
	/* Get the total word length of current code text line in code binary image */
	int length = code_img[(*ic) - IC_INIT_VALUE]->length;
	/* if the length is 1, then there's only the code word, no data. */
	if (length > 1) {
		addressing_type op1_addr, op2_addr;
		machine_word *word_to_write;
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
						return TRUE;
					}
				}
			}
			/*found symbol*/
			long data_to_add = entry->value;
			/* Calculate the distance to the label from "here" */
			if (op1_addr == RELATIVE) {
				data_to_add = *ic - data_to_add;
			}
			word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
			word_to_write->length = 0; /* it's a data word */
			word_to_write->word.data = build_data_word(op1_addr, data_to_add); /* build data word and put it in place: */
			code_img[(++(curr_ic)) - IC_INIT_VALUE] = word_to_write;
		}
		if (DIRECT == op2_addr || RELATIVE == op2_addr) {
			table_entry *entry = find_by_key(data_table, operands[1]);
			if (entry == NULL) {
				entry = find_by_key(code_table, operands[1]);
				if (entry == NULL) {
					entry = find_by_key(ext_table, operands[1]);
					if (entry == NULL)/* Symbol not found! */{
						print_error("Symbol not found.");
						return TRUE;
					}
				}
			}
			/*found symbol*/
			long data_to_add = entry->value;
			/* Calculate the distance to the label from "here" */
			if (op1_addr == RELATIVE) {
				data_to_add = *ic - data_to_add;
			}
			/*found symbol*/
			word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
			word_to_write->length = 0;
			word_to_write->word.data = build_data_word(op1_addr, data_to_add);
			code_img[(++(curr_ic)) - IC_INIT_VALUE] = word_to_write;

		}
	}
	/* Make the current pass IC as the next line ic */
	(*ic) = (*ic)+length;
	return FALSE;
}


int analyze_operands(char *line, int i, char *operands[2], int *operand_count) {
/* Make some space to the operand strings */
	int j;
	*operand_count = 0;
	MOVE_TO_NOT_WHITE(line, i)
	if (line[i] == ',') {
		print_error("Unexpected comma after command.");
		return TRUE; /* an error occurred */
	}
	operands[0] = malloc_with_check(MAX_LINE_LENGTH);
	operands[1] = malloc_with_check(MAX_LINE_LENGTH);
	/* until no too many operands (max of 2) and it's not the end of the line */
	for (*operand_count = 0; line[i] != EOF && line[i] != '\n' && line[i];) {
		if (*operand_count == 2) /* =We already got 2 operands in, We're going ot get the third! */ {
			print_error("Too many operands for command.");
			return TRUE; /* an error occurred */
		}
		/* as long we're still on same operand */
		for (j = 0; line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF &&
		            line[i] != ','; i++, j++) {
			operands[*operand_count][j] = line[i];
		}
		operands[*operand_count][j] = '\0';
		(*operand_count)++; /* We've just saved another operand! */
		MOVE_TO_NOT_WHITE(line, i)
		if (line[i] == '\n' || line[i] == EOF || !line[i]) break;
		else if (line[i] != ',') {
			/* After operand & after white chars there's something that isn't ',' or end of line.. */
			print_error("Expecting ',' between operands");
			/* Release operands dynamically allocated memory */
			free(operands[0]);
			free(operands[1]);
			return TRUE;
		}
		i++;
		MOVE_TO_NOT_WHITE(line, i);
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line[i] == '\n' || line[i] == EOF || !line[i]) print_error("Missing operand after comma.");
		else if (line[i] == ',') print_error("Multiple consecutive commas.");
		else continue; /* No errors, continue */
		{ /* Error found */
			/* No one forgot you two! */
			free(operands[0]);
			free(operands[1]);
			return TRUE;
		}
	}
	return FALSE;
}

void get_opcode_func(char *cmd, opcode *opcode_out, funct *funct_out) {
	*funct_out = NONE_FUNCT;
	if (strcmp(cmd, "mov") == 0) {
		*opcode_out = MOV_OP;
	} else if (strcmp(cmd, "cmp") == 0) {
		*opcode_out = CMP_OP;
	} else if (strcmp(cmd, "add") == 0) {
		*opcode_out = ADD_OP;
		*funct_out = ADD_FUNCT;
	} else if (strcmp(cmd, "sub") == 0) {
		*opcode_out = SUB_OP;
		*funct_out = SUB_FUNCT;
	} else if (strcmp(cmd, "lea") == 0) {
		*opcode_out = LEA_OP;
	} else if (strcmp(cmd, "clr") == 0) {
		*opcode_out = CLR_OP;
		*funct_out = CLR_FUNCT;
	} else if (strcmp(cmd, "not") == 0) {
		*opcode_out = NOT_OP;
		*funct_out = NOT_FUNCT;
	} else if (strcmp(cmd, "inc") == 0) {
		*opcode_out = INC_OP;
		*funct_out = INC_FUNCT;
	} else if (strcmp(cmd, "dec") == 0) {
		*opcode_out = DEC_OP;
		*funct_out = DEC_FUNCT;
	} else if (strcmp(cmd, "jmp") == 0) {
		*opcode_out = JMP_OP;
		*funct_out = JMP_FUNCT;
	} else if (strcmp(cmd, "bne") == 0) {
		*opcode_out = BNE_OP;
		*funct_out = BNE_FUNCT;
	} else if (strcmp(cmd, "jsr") == 0) {
		*opcode_out = JSR_OP;
		*funct_out = JSR_FUNCT;
	} else if (strcmp(cmd, "red") == 0) {
		*opcode_out = RED_OP;
	} else if (strcmp(cmd, "prn") == 0) {
		*opcode_out = PRN_OP;
	} else if (strcmp(cmd, "rts") == 0) {
		*opcode_out = RTS_OP;
	} else if (strcmp(cmd, "stop") == 0) {
		*opcode_out = STOP_OP;
	} else *opcode_out = NONE_OP; /* Not found! */
}

addressing_type get_addressing_type(char *operand) {
	if (operand[0] == '\0') return NONE_ADDR;
	if (is_register(operand)) return REGISTER;
	else if (is_immediate(operand)) return IMMEDIATE;
	else if (is_direct(operand)) return DIRECT;
	else if (is_relative(operand)) return RELATIVE;
	else return NONE_ADDR;
}

bool is_register(char *operand) {
	/*check if operand is register*/
	if ((strcmp(operand, "r0") == 0) || (strcmp(operand, "r1") == 0) || (strcmp(operand, "r2") == 0) ||
	    (strcmp(operand, "r3") == 0) ||
	    (strcmp(operand, "r4") == 0) || (strcmp(operand, "r5") == 0) || (strcmp(operand, "r6") == 0) ||
	    (strcmp(operand, "r7") == 0)) {
		return TRUE;
	}
	return FALSE;
}

bool is_immediate(char *operand) {
	/*in immediate addressing first char is #*/
	return (operand[0] == '#' && is_int(operand + 1));
}

bool is_direct(char *operand) {
	/*if the operand is legal name of label it is direct addressing*/
	if (is_legal_label(operand)) return TRUE;
	return FALSE;
}

bool is_relative(char *operand) {
	/* in relative addressing first char is &*/
	if (operand[0] == '&') {
		operand++;
		/*after the & the relative addressing is just like direct addressing*/
		if (is_direct(operand)) {
			return TRUE;
		}
	}
	return FALSE;
}

/* Assembles a code word by the  */
code_word *get_code_word(opcode curr_opcode, funct curr_funct, int op_count, char *operands[2]) {
	addressing_type first_addressing, second_addressing;
	bool is_valid = TRUE;
	code_word *codeword;
	/* TODO: Check for none and print message \/ */
	first_addressing = get_addressing_type(operands[0]);
	second_addressing = get_addressing_type(operands[1]);
	/* Validate the operand types and count */
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) {
		if (op_count != 2) {
			if (op_count == 0)
				print_error("2 operands required for operation (got 0)");
			else
				print_error("2 operands required for operation (got 1)");
			return NULL;
		}

		if (curr_opcode == CMP_OP) {
			is_valid = validate_op_addr(first_addressing, second_addressing, 3, 3, IMMEDIATE, DIRECT, REGISTER,
			                            IMMEDIATE, DIRECT, REGISTER);
		} else if (curr_opcode == ADD_OP || curr_opcode == MOV_OP) { /* Also SUB_OP */
			is_valid = validate_op_addr(first_addressing, second_addressing, 3, 2, IMMEDIATE, DIRECT, REGISTER,
			                            DIRECT, REGISTER);
		} else if (curr_opcode == LEA_OP) {

			is_valid = validate_op_addr(first_addressing, second_addressing, 1, 2, DIRECT,
			                            DIRECT, REGISTER);
		}
	} else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) {
		addressing_type first_addressing;
		/*if operand number is not 1 there are either Too many operands or to few*/
		if (op_count != 1) {
			if (op_count < 1) print_error("Missing operands");
			if (op_count > 1) print_error("Too many operands");
			return NULL;
		}
		first_addressing = get_addressing_type(operands[0]);
		if (curr_opcode == RED_OP || curr_opcode == CLR_OP) { /* Also for NOT, INC, DEC */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 2, 0, DIRECT, REGISTER);
		} else if (curr_opcode == JMP_OP) {/* Also for BNE,JSR */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 2, 0, DIRECT, RELATIVE);
		} else { /* Then it's PRN */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 3, 0, IMMEDIATE, DIRECT, REGISTER);
		}
	} else if (curr_opcode <= STOP_OP && curr_opcode >= RTS_OP) {
		/*if operand number is not 0 there are Too many operands*/
		if (op_count > 0) {
			print_error("Too many operands");
			return NULL;
		}
	}
	if (is_valid == FALSE) return NULL; /* Return null if invalid */

	/* Create the code word by the data: */
	codeword = (code_word *) malloc_with_check(sizeof(code_word));
	if (codeword == NULL) return NULL; /* stop if allocation failed */

	codeword->opcode = curr_opcode;
	codeword->funct = curr_funct; /* if no funct, curr_funct = NONE_FUNCT = 0, and it should be the default. */
	codeword->ARE = ((1 << 2) &
	                 0xFF); /* A is the only one who is 1 when it's n operation. we treat ARE as a single unit so j */
	/* Default values of register bits are 0 */
	codeword->dest_addressing = codeword->dest_register = codeword->src_addressing = codeword->src_register = 0;
	/* Check if need to set the registers bits */
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) { /* First Group, two operands */
		codeword->src_addressing = first_addressing;
		codeword->dest_addressing = second_addressing;
		/* if it's register, set it's name in the proper locations */
		if (first_addressing == REGISTER) {
			codeword->src_register = get_register_by_name(operands[0]);
		}
		if (second_addressing == REGISTER) {
			codeword->dest_register = get_register_by_name(operands[1]);
		}
	} else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) {
		codeword->dest_addressing = first_addressing;
		if (first_addressing == REGISTER) {
			codeword->dest_register = get_register_by_name(operands[0]);
		}
	}
	return codeword;
}

/*
 * Returns whether addressing type is valid for the operands by the current addressing of each operand,
 * the count of valid addressing type given as argument for both operands and the valid addressing types for both operands
 */
bool validate_op_addr(addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count,
                      int op2_valid_addr_count, ...) {
	bool is_valid;
	va_list list;
	addressing_type op1_0, op1_1, op1_2, op1_3, op2_0, op2_1, op2_2, op2_3; /* valid addressing types for each operand */
	op1_0 = op1_1 = op1_2 = op1_3 = op2_0 = op2_1 = op2_2 = op2_3 = NONE_ADDR;
	va_start(list, op2_valid_addr_count);
	/* Put the valid addressing types in the op[1|2]_[0|1|2] varriables, using va_arg (we user the count operand to determine how many arguments are sent */
	if (op1_valid_addr_count >= 1) {
		op1_0 = va_arg(list,
		               int);
	}
	if (op1_valid_addr_count >= 2) {
		op1_1 = va_arg(list,
		               int);
	}
	if (op1_valid_addr_count >= 3) {
		op1_2 = va_arg(list,
		               int);
	}
	if (op1_valid_addr_count >= 4) {
		op1_3 = va_arg(list,
		               int);
	}
	for (; op1_valid_addr_count > 5; va_arg(list,
	                                        int), op1_valid_addr_count--); /* Go on with stack until got all (even above limitation of 4) */
	/* Again for second operand by the count */
	if (op2_valid_addr_count >= 1) {
		op2_0 = va_arg(list,
		               int);
	}
	if (op2_valid_addr_count >= 2) {
		op2_1 = va_arg(list,
		               int);
	}
	if (op2_valid_addr_count >= 3) {
		op2_2 = va_arg(list,
		               int);
	}
	if (op2_valid_addr_count >= 4) {
		op2_3 = va_arg(list,
		               int);
	}
	va_end(list);  /* We got all the arguments we wanted */
	is_valid = TRUE;
	/* if operand addressing is not valid, print error */
	if (!((op1_valid_addr_count == 0 && op1_addressing == NONE_ADDR) ||
	      (op1_valid_addr_count > 0 && op1_0 == op1_addressing) ||
	      (op1_valid_addr_count > 1 && op1_1 == op1_addressing) ||
	      (op1_valid_addr_count > 2 && op1_2 == op1_addressing) ||
	      (op1_valid_addr_count > 3 && op1_3 == op1_addressing))) {
		print_error("Invalid addressing mode for first operand.");
		is_valid = FALSE;
	}
	if (!((op2_valid_addr_count == 0 && op2_addressing == NONE_ADDR) ||
	      (op2_valid_addr_count > 0 && op2_0 == op2_addressing) ||
	      (op2_valid_addr_count > 1 && op2_1 == op2_addressing) ||
	      (op2_valid_addr_count > 2 && op2_2 == op2_addressing) ||
	      (op2_valid_addr_count > 3 && op2_3 == op2_addressing))) {
		print_error("Invalid addressing mode for second operand.");
		is_valid = FALSE;
	}
	return is_valid;
}

/* Returns an enu register by the register's name */
reg get_register_by_name(char *name) {
	if (name[0] == 'r' && isdigit(name[1]) && name[2] == '\0') {
		int digit = name[1] - '0';
		if (digit >= 0 && digit <= 7) return digit;
	}
	return NONE_REG; /* No match */
}

data_word *build_data_word(addressing_type addressing, long data) {
	signed int mask; /* For bitwise operations for data conversion */
	unsigned int ARE = 4, mask_un; /* 4 = 2^2 = 1 << 2 */
	data_word *dataword = malloc_with_check(sizeof(data_word));

	if (addressing == DIRECT) ARE = 2;
	dataword->ARE = ARE; /* Set ARE field value */

	/* Now all left is to encode the data */
	mask = -1;
	mask_un = mask; /* both hold 11...11 */
	mask_un >>= 11; /* Now mask_un holds 000000000001111....111, 11 zeros and 21 ones */
	dataword->data = mask_un & data; /* Now just the 21 lsb bits area left and assigned to data field. */
	return dataword;
}