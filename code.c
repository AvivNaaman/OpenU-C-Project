#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "code.h"


int process_code(char *line, int i, int *ci, char *code_img) {
	char operation[80];
	char *temp;
	char operands[2][80]; /* 2 strings, each for operand */
	opcode curr_opcode;
	code_word *codeword;
	funct curr_funct;
	int j, operand_count;
	/* Skip white chars */
	MOVE_TO_NOT_WHITE(line, i)

	for (j = 0; line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF; i++, j++) {
		operation[j] = line[i];
	}
	operation[j] = '\0'; /* End of string */

	/* Get opcode & funct by command name into curr_opcode/curr_funct */
	get_opcode_func(operation, &curr_opcode, &curr_funct);
	/* If invalid operation, print and skip processing the line. */
	if (curr_opcode == NONE_OP) {
		printf("Unrecognized instruction: %s.", operation);
		return TRUE;
	}
	/* Analyze operands */
	MOVE_TO_NOT_WHITE(line, i)
	/* until no too many operands (max of 2) and it's not the end of the line */
	for (operand_count = 0; line[i] != EOF && line[i] != '\n'; operand_count++) {
		if (operand_count == 2) /* =We already got 2 operands in, this is the 3rd! */ {
			printf("Too many operands for operation %s", operation);
			return TRUE;
		}
		/* as long we're still on same operand */ /* TODO: Validate chars of operand */
		for (j = 0; line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF; i++, j++) {
			operands[operand_count][j] = line[i];
		}
		MOVE_TO_NOT_WHITE(line, i)
		if (line[i] == '\n' || line[i] == EOF) break;
		else if (line[i] != ',') { /* TODO: Error about unexpected character */
			/* After operand & after white chars there's something that isn't ',' or end of line.. */
			printf("Error: Expecting ',' between operands (got '%c')", line[i]);
			return TRUE;
		}
		i++;
		operands[operand_count][j] = '\0';
		MOVE_TO_NOT_WHITE(line, i);
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line[i] == '\n' || line[i] == EOF) printf("Error: missing operand after comma");
		else if (line[i] == ',') printf("Error: Multiple consecutive commas");
		else continue; /* No errors, continue */
		return TRUE; /* return that errors found */
	}

	/* Build code word (returns null if validation failed) */
	if ((codeword = get_code_word(curr_opcode, curr_funct, curr_opcode, (char**)operands)) == NULL) return TRUE;

	/* Now we got our data word in *codeword. all left is to build all the data words for the operation and add the count to ci. */
	/* We can only encode to code image immediate addressed number */
	if (get_addressing_type(operands[0]) == IMMEDIATE){
		/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to atoi */
		int value = atoi(operands[0]+1);
		temp  = (char *)build_data_word(IMMEDIATE, value);
		/* Increase ci because the code word was written already. write data to code image */
		write_word(code_img, ++(*ci), temp[2], temp[1], temp[0]); /* [3] contains msb, we want just the 3 lower bytes. */
	}
	/* Same for second operand */
	if (get_addressing_type(operands[1]) == IMMEDIATE){
		/* Get value of immediate addressed operand. notice that it starts with #, so we're skipping the # in the call to atoi */
		int value = atoi(operands[1]+1);
		temp  = (char *)build_data_word(IMMEDIATE, value);
		/* Increase ci because the code word was written already. write data to code image */
		write_word(code_img, ++(*ci), temp[2], temp[1], temp[0]); /* [3] contains msb, we want just the 3 lower bytes. */
	}
	(*ci)++; /* increase ci to point the next cell */
	free(codeword); /* Release malloced memory */
	return FALSE; /* No errors */
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
	return (operand[0] == '#' && is_int(operand+1));
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
code_word *get_code_word(opcode curr_opcode, funct curr_funct, int op_count, char **operands) {
	addressing_type first_addressing, second_addressing;
	bool is_valid = TRUE;
	code_word *codeword;
	/* TODO: Check for none and print message \/ */
	first_addressing = get_addressing_type(operands[0]);
	second_addressing = get_addressing_type(operands[1]);
	/* Validate the operand types and count */
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) {
		if (op_count != 2) {
			printf("Error: 2 operands required for operation (got %d)", op_count);
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
			if (op_count < 1) printf("Error: Missing operands");
			if (op_count > 1) printf("Error: Too many operands");
			return NULL;
		}
		first_addressing = get_addressing_type(operands[0]);
		if (curr_opcode == RED_OP || curr_opcode == CLR_OP) { /* Also for NOT, INC, DEC */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 2, 0, DIRECT, REGISTER);
		} else if (curr_opcode == JMP_OP) {/* Also for BNE,JSR */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 2, 0, DIRECT, RELATIVE);
		} else { /* Then it's PRN */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 3, 0, IMMEDIATE, DIRECT, RELATIVE);
		}
	} else if (curr_opcode <= STOP_OP && curr_opcode >= RTS_OP) {
		/*if operand number is not 0 there are Too many operands*/
		if (op_count > 0) {
			printf("Error: Too many operands");
			return NULL;
		}
	}
	if (is_valid == FALSE) return NULL; /* Return null if invalid */

	/* Create the code word by the data: */
	codeword = (code_word *) malloc_with_check(sizeof(code_word));
	if (codeword == NULL) return NULL; /* stop if allocation failed */

	codeword->opcode = curr_opcode;
	codeword->funct = curr_funct;
	codeword->ARE = ((1<<2)&0xFF); /* A is the only one who is 1 when it's n operation. we treat ARE as a single unit so j */
	/* Default values of register bits are 0 */
	codeword->dest_addressing = codeword->dest_register = codeword->src_addressing = codeword->src_register = 0;
	/* Check if need to set the registers bits */
	if (curr_opcode >= MOV_OP && curr_opcode <= PRN_OP) { /* First Group, two operands */
		codeword->src_addressing = first_addressing;
		codeword->dest_addressing = second_addressing;
		/* if it's register, set it's name in the proper locations */
		if (first_addressing == REGISTER) {
			codeword->src_register = get_register_by_name(operands[0]);
		}
		if (second_addressing == REGISTER) {
			codeword->dest_register = get_register_by_name(operands[1]);
		}
	} else if (curr_opcode >= MOV_OP && curr_opcode <= PRN_OP) {
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
	addressing_type op1_0, op1_1, op1_2, op2_0, op2_1, op2_2; /* valid addressing types for each operand */
	op1_0 = op1_1 = op1_2 = op2_0 = op2_1 = op2_2 = NONE_ADDR;
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
	va_end(list);
	is_valid = TRUE;
	/* if operand addressing is not valid, print error */
	if (!((op1_valid_addr_count > 0 && op1_0 == op1_addressing) ||
	      (op1_valid_addr_count > 1 && op1_1 == op1_addressing) ||
	      (op1_valid_addr_count > 2 && op1_2 == op1_addressing))) {
		printf("Error: invalid addressing mode for first operand");
		is_valid = FALSE;
	}
	if (!((op2_valid_addr_count > 0 && op2_0 == op2_addressing) ||
	      (op2_valid_addr_count > 1 && op2_1 == op2_addressing) ||
	      (op2_valid_addr_count > 2 && op2_2 == op2_addressing))) {
		printf("Error: invalid addressing mode for second operand");
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

data_word *build_data_word(addressing_type addressing, int data) {
	signed int mask; /* For bitwise operations for data conversion */
	unsigned int ARE = 1 << 2, mask_un;
	data_word *dataword = malloc_with_check(sizeof(data_word));

	if (addressing == DIRECT) ARE = 1;
	dataword->ARE = ARE; /* Set ARE field value */

	/* Now all left is to encode the data */
	mask = -1;
	mask_un = mask; /* both hold 11...11 */
	mask_un >>= 11; /* Now mask_un holds 000000000001111....111, 11 zeros and 21 ones */
	dataword->data = mask_un & data; /* Now just the 21 lsb bits area left and assigned to data field. */
	return dataword;
}