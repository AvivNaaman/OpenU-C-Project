#include <stdio.h>
#include <string.h>
#include "code.h"
#include "utils.h"


int process_code(char *line, int i) {
	char operation[80];
	char operands[2][80]; /* 2 strings, each for operand */
	opcode curr_opcode;
	funct curr_funct;
	code_word *curr_code; /* current code word */
	int j, operand_count;
	/* Skip white chars */
	MOVE_TO_NOT_WHITE(line, i)

	for (j = 0;line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF; i++,j++) {
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
		for (j = 0;line[i] && line[i] != '\t' && line[i] != ' ' && line[i] != '\n' && line[i] != EOF; i++,j++) {
			operands[operand_count][j] = line[i];
		}
		MOVE_TO_NOT_WHITE(line,i)
		if (line[i] == '\n' || line[i] == EOF) break;
		else if (line[i] != ',') { /* TODO: Error about unexpected character */
			/* After operand & after white chars there's something that isn't ',' or end of line.. */
			printf("Error: Expecting ',' between operands (got '%c')", line[i]);
			return TRUE;
		}
		i++;
		MOVE_TO_NOT_WHITE(line, i);
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line[i] == '\n' || line[i] == EOF) printf("Error: missing operand after comma");
		else if (line[i] == ',') printf("Error: Multiple consecutive commas");
		else continue; /* No errors, continue */
		return TRUE; /* return that errors found */
	}

	/*  */

}

void get_opcode_func(char* cmd, opcode *opcode_out, funct *funct_out) {
	*funct_out = NONE_FUNCT;
	if (strcmp(cmd, "mov") == 0) {
		*opcode_out = MOV_OP;
	}
	else if (strcmp(cmd, "cmp") == 0) {
		*opcode_out = CMP_OP;
	}
	else if (strcmp(cmd, "add") == 0) {
		*opcode_out = ADD_OP;
		*funct_out = ADD_FUNCT;
	}
	else if (strcmp(cmd, "sub") == 0) {
		*opcode_out = SUB_OP;
		*funct_out = SUB_FUNCT;
	}
	else if (strcmp(cmd, "lea") == 0) {
		*opcode_out = LEA_OP;
	}
	else if (strcmp(cmd, "clr") == 0) {
		*opcode_out = CLR_OP;
		*funct_out = CLR_FUNCT;
	}
	else if (strcmp(cmd, "not") == 0) {
		*opcode_out = NOT_OP;
		*funct_out = NOT_FUNCT;
	}
	else if (strcmp(cmd, "inc") == 0) {
		*opcode_out = INC_OP;
		*funct_out = INC_FUNCT;
	}
	else if (strcmp(cmd, "dec") == 0) {
		*opcode_out = DEC_OP;
		*funct_out = DEC_FUNCT;
	}
	else if (strcmp(cmd, "jmp") == 0) {
		*opcode_out = JMP_OP;
		*funct_out = JMP_FUNCT;
	}
	else if (strcmp(cmd, "bne") == 0) {
		*opcode_out = BNE_OP;
		*funct_out = BNE_FUNCT;
	}
	else if (strcmp(cmd, "jsr") == 0) {
		*opcode_out = JSR_OP;
		*funct_out = JSR_FUNCT
	}
	else if (strcmp(cmd, "red") == 0) {
		*opcode_out = RED_OP;
	}
	else if (strcmp(cmd, "prn") == 0) {
		*opcode_out = PRN_OP;
	}
	else if (strcmp(cmd, "rts") == 0) {
		*opcode_out = RTS_OP;
	}
	else if (strcmp(cmd, "stop") == 0) {
		*opcode_out = STOP_OP;
	}
	else *opcode_out = NONE_OP; /* Not found! */
}