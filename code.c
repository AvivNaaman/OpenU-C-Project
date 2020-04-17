#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
		operands[operand_count][j]='\0';
		MOVE_TO_NOT_WHITE(line, i);
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line[i] == '\n' || line[i] == EOF) printf("Error: missing operand after comma");
		else if (line[i] == ',') printf("Error: Multiple consecutive commas");
		else continue; /* No errors, continue */
		return TRUE; /* return that errors found */
	}

	/* if opcode belong to third group  */
    if(curr_opcode<=STOP_OP&&curr_opcode>=RTS_OP){
        /*if operand number is not 0 there are to many operands*/
        if(operand_count>0){
            printf("Error: To many operands");
            return TRUE;
        }
        else return FALSE;
    }
    /* if opcode belong to second group  */
    if(curr_opcode>=CLR_OP&&curr_opcode<=PRN_OP){
        /*if operand number is not 1 there are either to many operands or to few*/
        if(operand_count!=1) {
            if (operand_count < 1) printf("Error: Missing operands");
            if (operand_count > 1) printf("Error: To many operands");
            return TRUE;
        }
        /*if opcode is 5 or 12*/
        if((curr_opcode>=CLR_OP&&curr_opcode<=DEC_OP)||curr_opcode==RED_OP){

            check_type(operands[1]);
        }
        /*if opcode is 9*/
        if(curr_opcode>=JMP_OP&&curr_opcode<=JSR_OP){

        }
        /*opcode is 13*/
        else{

        }
    }
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
int check_type(char *operand){
    if(is_register(operand)) return REG;
    else if(is_immediate(operand)) return IMMEDIATE;
    else if(is_direct(operand)) return DIRECT;
    else if(is_relative(operand))
}
int is_register(char *operand){
    /*check if operand is register*/
    if((strcmp(operand,"r0")==0)||(strcmp(operand,"r1")==0)||(strcmp(operand,"r2")==0)||(strcmp(operand,"r3")==0)||
       (strcmp(operand,"r4")==0)||(strcmp(operand,"r5")==0)||(strcmp(operand,"r6")==0)||(strcmp(operand,"r7")==0)){
        return TRUE;
    }
    return FALSE;
}
int is_immediate(char *operand){
    int i,flag = 1;
    i=1;
    /*in immediate addressing first char is #*/
    if(operand[0]=='#'){
        /*second char can be either '-' or digit*/
        if(operand[i]=='-') i++;
        while(operand[i]){
            /*all of the chars after the second must be numbers*/
            if(!isdigit(operand[i])){
                flag = FALSE;
                break;
            }
            i++;
        }
    }
    else flag = FALSE;
    return flag;
}
int is_direct(char *operand){/*TODO: Implement*/

}
int is_relative(char *operand){
    /* in relative addressing first char is &*/
    if(operand[0]=='&'){
        operand++;
        /*after the & the relative addressing is just like direct addressing*/
        if(is_direct(operand)){
            return TRUE;
        }
    }
    return FALSE;
}