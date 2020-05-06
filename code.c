#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "code.h"
#include "utils.h"

/* TODO: Check if possible to make it nicer to read */
bool analyze_operands(char *line, int i, char **destination, int *operand_count, char *command) {
/* Make some space to the operand strings */
	int j;
	*operand_count = 0;
	MOVE_TO_NOT_WHITE(line, i)
	if (line[i] == ',') {
		print_error("Unexpected comma after command.");
		return FALSE; /* an error occurred */
	}
	destination[0] = malloc_with_check(MAX_LINE_LENGTH);
	destination[1] = malloc_with_check(MAX_LINE_LENGTH);
	/* until no too many operands (max of 2) and it's not the end of the line */
	for (*operand_count = 0; line[i] != EOF && line[i] != '\n' && line[i];) {
		if (*operand_count == 2) /* =We already got 2 operands in, We're going ot get the third! */ {
			print_error("Too many operands for operation %s.", command);
			return FALSE; /* an error occurred */
		}
		/* as long we're still on same operand */
		for (j = 0; line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF &&
		            line[i] != ','; i++, j++) {
			destination[*operand_count][j] = line[i];
		}
		destination[*operand_count][j] = '\0';
		(*operand_count)++; /* We've just saved another operand! */
		MOVE_TO_NOT_WHITE(line, i)
		if (line[i] == '\n' || line[i] == EOF || !line[i]) break;
		else if (line[i] != ',') {
			/* After operand & after white chars there's something that isn't ',' or end of line.. */
			print_error("Expecting ',' between operands");
			/* Release operands dynamically allocated memory */
			free(destination[0]);
			free(destination[1]);
			return FALSE;
		}
		i++;
		MOVE_TO_NOT_WHITE(line, i)
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line[i] == '\n' || line[i] == EOF || !line[i]) print_error("Missing operand after comma.");
		else if (line[i] == ',') print_error("Multiple consecutive commas.");
		else continue; /* No errors, continue */
		{ /* Error found */
			/* No one forgot you two! */
			free(destination[0]);
			free(destination[1]);
			return FALSE;
		}
	}
	return TRUE;
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
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7') return REGISTER;
	else if (operand[0] == '#' && is_int(operand + 1)) return IMMEDIATE;
	else if (is_legal_label_name(operand)) return DIRECT;
	else if (operand[0] && operand[0] == '&' && is_legal_label_name(operand + 1)) return RELATIVE;
	else return NONE_ADDR;
}

/* TODO: Refactor(?): Pass only operation name to this function */
code_word *get_code_word(opcode curr_opcode, funct curr_funct, int op_count, char *operands[2]) {
	addressing_type first_addressing, second_addressing;
	bool is_valid = TRUE;
	code_word *codeword;
	first_addressing = get_addressing_type(operands[0]);
	second_addressing = get_addressing_type(operands[1]);
	/* Validate the operand count and types */
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) {
		/* 2 operands required */
		if (op_count != 2) {
			print_error("Operation requires 2 operands (got %d)", op_count);
			return NULL;
		}
		/* validate operand addressing */
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
		/* 1 operand required */
		if (op_count != 1) {
			if (op_count < 1) print_error("Operation requires 1 operand (got %d)",op_count);
			return NULL;
		}
		/* validate operand addressing */
		first_addressing = get_addressing_type(operands[0]);
		if (curr_opcode == RED_OP || curr_opcode == CLR_OP) { /* Also for NOT, INC, DEC */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 2, 0, DIRECT, REGISTER);
		} else if (curr_opcode == JMP_OP) {/* Also for BNE,JSR */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 2, 0, DIRECT, RELATIVE);
		} else { /* Then it's PRN */
			is_valid = validate_op_addr(first_addressing, NONE_ADDR, 3, 0, IMMEDIATE, DIRECT, REGISTER);
		}
	} else if (curr_opcode <= STOP_OP && curr_opcode >= RTS_OP) {
		/* 0 operands exactly */
		if (op_count > 0) {
			print_error("Operation requires no operands (got %d)",op_count);
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

/* TODO(?): Migrate to array */
bool validate_op_addr(addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count,
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
	for (; op1_valid_addr_count > 5; va_arg(list, int), op1_valid_addr_count--); /* Go on with stack until got all (even above limitation of 4) */
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
		print_error("Invalid addressing mode for first operand.");
		is_valid = FALSE;
	}
	/* Again for second addressing */
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

	if (addressing == DIRECT) {
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