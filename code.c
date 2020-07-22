#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "code.h"
#include "utils.h"


/**
 * Returns whether the current addressing types of the operands are valid by the valid types, provided in the variable param.
 * @param op1_addressing The addressing type of the first operand
 * @param op2_addressing The addressing type of the second operand
 * @param op1_valid_addr_count The count of the valid addressing types for the first operand, specified afterwards.
 * @param op2_valid_addr_count The count of the valid addressing types for the second operand, specified afterwards.
 * @param ... The valid addressing types (as enum addressing_type) for the operands. The valid for the first, followed by the valid for the second.
 * @return Whether the addressing types are valid as specified
 */
static bool validate_op_addr(line_info line, addressing_type op1_addressing, addressing_type op2_addressing,
		int op1_valid_addr_count, int op2_valid_addr_count,...);

/* TODO: Check if possible to make it nicer to read */
bool analyze_operands(line_info line, int i, char **destination, int *operand_count, char *c) {
/* Make some space to the operand strings */
	int j;
	*operand_count = 0;
	MOVE_TO_NOT_WHITE(line.content, i)
	if (line.content[i] == ',') {
		print_error(line, "Unexpected comma after command.");
		return FALSE; /* an error occurred */
	}
	destination[0] = malloc_with_check(MAX_LINE_LENGTH);
	destination[1] = malloc_with_check(MAX_LINE_LENGTH);

	/* until no too many operands (max of 2) and it's not the end of the line */
	for (*operand_count = 0; line.content[i] != EOF && line.content[i] != '\n' && line.content[i];) {
		if (*operand_count == 2) /* =We already got 2 operands in, We're going ot get the third! */ {
			print_error(line, "Too many operands for operation (got >%d)", *operand_count);
			free(destination[0]);
			free(destination[1]);
			return FALSE; /* an error occurred */
		}

		/* as long we're still on same operand */
		for (j = 0; line.content[i] && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != '\n' && line.content[i] != EOF &&
		            line.content[i] != ','; i++, j++) {
			destination[*operand_count][j] = line.content[i];
		}

		destination[*operand_count][j] = '\0';
		(*operand_count)++; /* We've just saved another operand! */
		MOVE_TO_NOT_WHITE(line.content, i)

		if (line.content[i] == '\n' || line.content[i] == EOF || !line.content[i]) break;
		else if (line.content[i] != ',') {
			/* After operand & after white chars there's something that isn't ',' or end of line.. */
			print_error(line, "Expecting ',' between operands");
			/* Release operands dynamically allocated memory */
			free(destination[0]);
			free(destination[1]);
			return FALSE;
		}
		i++;
		MOVE_TO_NOT_WHITE(line.content, i)
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line.content[i] == '\n' || line.content[i] == EOF || !line.content[i]) print_error(line, "Missing operand after comma.");
		else if (line.content[i] == ',') print_error(line, "Multiple consecutive commas.");
		else continue; /* No errors, continue */
		{ /* Error found! (didn't continue) */
			/* No one forgot you two! */
			free(destination[0]);
			free(destination[1]);
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * A single lookup table element
 */
struct cmd_lookup_element {
	char *cmd;
	opcode op;
	funct fun;
};
/**
 * A lookup table for opcode & funct by command name
 */
static struct cmd_lookup_element lookup_table[] = {
		{"mov", MOV_OP, NONE_FUNCT},
		{"cmp",CMP_OP, NONE_FUNCT},
		{"add",ADD_OP, ADD_FUNCT},
		{"sub",SUB_OP, SUB_FUNCT},
		{"lea",LEA_OP, NONE_FUNCT},
		{"clr",CLR_OP, CLR_FUNCT},
		{"not",NOT_OP, NOT_FUNCT},
		{"inc",INC_OP, INC_FUNCT},
		{"dec",DEC_OP, DEC_FUNCT},
		{"jmp",JMP_OP, JMP_FUNCT},
		{"bne",BNE_OP, BNE_FUNCT},
		{"jsr",JSR_OP, JSR_FUNCT},
		{"red",RED_OP, NONE_FUNCT},
		{"prn",PRN_OP, NONE_FUNCT},
		{"rts",RTS_OP, NONE_FUNCT},
		{"stop",STOP_OP, NONE_FUNCT},
		{NULL, NONE_OP, NONE_FUNCT}
};
void get_opcode_func(char *cmd, opcode *opcode_out, funct *funct_out) {
	struct cmd_lookup_element *e;
	*opcode_out = NONE_OP;
	*funct_out = NONE_FUNCT;
	/* iterate over the lookup table, if cmds are same return the opcode of found. */
	for (e = lookup_table; e->cmd != NULL; e++) {
		if (strcmp(e->cmd, cmd) == 0) {
			*opcode_out = e->op;
			*funct_out = e->fun;
			return;
		}
	}
}

addressing_type get_addressing_type(char *operand) {
	/* if nothing, just return none */
	if (operand[0] == '\0') return NONE_ADDR;
	/* if first char is 'r', second is number in range 0-7 and third is end of string, it's a register */
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' && operand[2] == '\0') return REGISTER_ADDR;
	/* if operand starts with # and a number right after that, it's immediately addressed */
	else if (operand[0] == '#' && is_int(operand + 1)) return IMMEDIATE_ADDR;
	/* if operand starts with & and has label afterwards, it;s realtively addressed */
	else if (operand[0] && operand[0] == '&' && is_valid_label_name(operand + 1)) return RELATIVE_ADDR;
	/* if operand is a valid label name, it's directly addressed */
	else if (is_valid_label_name(operand)) return DIRECT_ADDR;
	else return NONE_ADDR;
}

/* TODO: Refactor(?): Pass only operation name to this function */
code_word *get_code_word(line_info line, opcode curr_opcode, funct curr_funct, int op_count, char *operands[2]) {
	addressing_type first_addressing, second_addressing;
	bool is_valid = TRUE;
	code_word *codeword;
	first_addressing = get_addressing_type(operands[0]);
	second_addressing = get_addressing_type(operands[1]);
	/* Validate the operand count and types */
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) {
		/* 2 operands required */
		if (op_count != 2) {
			print_error(line, "Operation requires 2 operands (got %d)", op_count);
			return NULL;
		}
		/* validate operand addressings */
		if (curr_opcode == CMP_OP) {
			is_valid = validate_op_addr(line, first_addressing, second_addressing, 3, 3, IMMEDIATE_ADDR, DIRECT_ADDR,
			                            REGISTER_ADDR,
			                            IMMEDIATE_ADDR, DIRECT_ADDR, REGISTER_ADDR);
		} else if (curr_opcode == ADD_OP || curr_opcode == MOV_OP) { /* Also SUB_OP */
			is_valid = validate_op_addr(line, first_addressing, second_addressing, 3, 2, IMMEDIATE_ADDR, DIRECT_ADDR,
			                            REGISTER_ADDR,
			                            DIRECT_ADDR, REGISTER_ADDR);
		} else if (curr_opcode == LEA_OP) {

			is_valid = validate_op_addr(line, first_addressing, second_addressing, 1, 2, DIRECT_ADDR,
			                            DIRECT_ADDR, REGISTER_ADDR);
		}
	} else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) {
		/* 1 operand required */
		if (op_count != 1) {
			if (op_count < 1) print_error(line, "Operation requires 1 operand (got %d)", op_count);
			return NULL;
		}
		/* validate operand addressing */
		first_addressing = get_addressing_type(operands[0]);
		if (curr_opcode == RED_OP || curr_opcode == CLR_OP) { /* Also for NOT, INC, DEC */
			is_valid = validate_op_addr(line, first_addressing, NONE_ADDR, 2, 0, DIRECT_ADDR, REGISTER_ADDR);
		} else if (curr_opcode == JMP_OP) {/* Also for BNE,JSR */
			is_valid = validate_op_addr(line, first_addressing, NONE_ADDR, 2, 0, DIRECT_ADDR, RELATIVE_ADDR);
		} else { /* Then it's PRN */
			is_valid = validate_op_addr(line, first_addressing, NONE_ADDR, 3, 0, IMMEDIATE_ADDR, DIRECT_ADDR, REGISTER_ADDR);
		}
	} else if (curr_opcode <= STOP_OP && curr_opcode >= RTS_OP) {
		/* 0 operands exactly */
		if (op_count > 0) {
			print_error(line, "Operation requires no operands (got %d)", op_count);
			return NULL;
		}
	}
	if (!is_valid) return NULL; /* Return null if invalid */

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
		if (first_addressing == REGISTER_ADDR) {
			codeword->src_register = get_register_by_name(operands[0]);
		}
		if (second_addressing == REGISTER_ADDR) {
			codeword->dest_register = get_register_by_name(operands[1]);
		}
	} else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) {
		codeword->dest_addressing = first_addressing;
		if (first_addressing == REGISTER_ADDR) {
			codeword->dest_register = get_register_by_name(operands[0]);
		}
	}
	return codeword;
}

/* TODO(?): Refactor to using an array instead of many many single vars */
static bool validate_op_addr(line_info line, addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count,
                      int op2_valid_addr_count, ...) {
	bool is_valid;
	va_list list;
	addressing_type op1_0, op1_1, op1_2, op1_3, op2_0, op2_1, op2_2, op2_3; /* valid addressing types for each operand */
	op1_0 = op1_1 = op1_2 = op1_3 = op2_0 = op2_1 = op2_2 = op2_3 = NONE_ADDR;
	va_start(list, op2_valid_addr_count);
	/* Put the valid addressing types in the op[1|2]_[0|1|2] varriables, using va_arg (we user the count operand to determine how many arguments are sent */
	if (op1_valid_addr_count >= 1) {
		op1_0 = va_arg(list, int);
	}
	if (op1_valid_addr_count >= 2) {
		op1_1 = va_arg(list, int);
	}
	if (op1_valid_addr_count >= 3) {
		op1_2 = va_arg(list, int);
	}
	if (op1_valid_addr_count >= 4) {
		op1_3 = va_arg(list, int);
	}
	for (; op1_valid_addr_count > 5; va_arg(list,
	                                        int), op1_valid_addr_count--); /* Go on with stack until got all (even above limitation of 4) */
	/* Again for second operand by the count */
	if (op2_valid_addr_count >= 1) {
		op2_0 = va_arg(list, int);
	}
	if (op2_valid_addr_count >= 2) {
		op2_1 = va_arg(list, int);
	}
	if (op2_valid_addr_count >= 3) {
		op2_2 = va_arg(list, int);
	}
	if (op2_valid_addr_count >= 4) {
		op2_3 = va_arg(list, int);
	}
	va_end(list);  /* We got all the arguments we wanted */
	is_valid = TRUE;
	/* if operand addressing is not valid, print error */
	/* A little bit complex, but basically: if there's something to check, find out what to check by the count params and check if any of those. */
	if (!((op1_valid_addr_count == 0 && op1_addressing == NONE_ADDR) ||
	      (op1_valid_addr_count > 0 && op1_0 == op1_addressing) ||
	      (op1_valid_addr_count > 1 && op1_1 == op1_addressing) ||
	      (op1_valid_addr_count > 2 && op1_2 == op1_addressing) ||
	      (op1_valid_addr_count > 3 && op1_3 == op1_addressing))) {
		print_error(line, "Invalid addressing mode for first operand.");
		is_valid = FALSE;
	}
	/* Again for second addressing */
	if (!((op2_valid_addr_count == 0 && op2_addressing == NONE_ADDR) ||
	      (op2_valid_addr_count > 0 && op2_0 == op2_addressing) ||
	      (op2_valid_addr_count > 1 && op2_1 == op2_addressing) ||
	      (op2_valid_addr_count > 2 && op2_2 == op2_addressing) ||
	      (op2_valid_addr_count > 3 && op2_3 == op2_addressing))) {
		print_error(line, "Invalid addressing mode for second operand.");
		is_valid = FALSE;
	}
	return is_valid;
}

reg get_register_by_name(char *name) {
	if (name[0] == 'r' && isdigit(name[1]) && name[2] == '\0') {
		int digit = name[1] - '0';
		if (digit >= 0 && digit <= 7) return digit;
	}
	return NONE_REG; /* No match */
}

data_word *build_data_word(addressing_type addressing, long data, bool is_extern_symbol) {
	signed long mask; /* For bitwise operations for data conversion */
	unsigned long ARE = 4, mask_un; /* 4 = 2^2 = 1 << 2 */
	data_word *dataword = malloc_with_check(sizeof(data_word));

	if (addressing == DIRECT_ADDR) {
		ARE = is_extern_symbol ? 1 : 2;
	}
	dataword->ARE = ARE; /* Set ARE field value */

	/* Now all left is to encode the data */
	mask = -1;
	mask_un = mask; /* both hold 11...11 */
	mask_un >>= 11; /* Now mask_un holds 000000000001111....111, 11 zeros and 21 ones */
	dataword->data = mask_un & data; /* Now just the 21 lsb bits area left and assigned to data field. */
	return dataword;
}