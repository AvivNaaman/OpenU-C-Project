#include "utils.h"
/* Represents an allowed addressing type */
typedef enum addressing_types {
	/* Immediate addressing (0) */
	IMMEDIATE = 0,
	/* Direct addressing (1) */
	DIRECT = 1,
	/* Relative addressing (2) */
	RELATIVE = 2,
	/* address via register*/
	REGISTER = 3,
	NONE_ADDR = -1
} addressing_type ;

/* definition for each command opcode */
typedef enum opcode {
	/* First Group */
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 2,

	LEA_OP = 4,
	/* END First Group */

	/* Second Group */
	CLR_OP = 5,
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,

	JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,

	RED_OP = 12,
	PRN_OP = 13,
	/* END Second Group */

	/* Third Group */
	RTS_OP = 14,
	STOP_OP = 15,
	/* END Third Group */

	/* Failed flag (=no op detected) */
	NONE_OP = -1
} opcode;
/* definition for the value of funct, for different commands with same names */
typedef enum funct {
	/* OPCODE 2 */
	ADD_FUNCT = 1,
	SUB_FUNCT = 2,

	/* OPCODE 5 */
	CLR_FUNCT = 1,
	NOT_FUNCT = 2,
	INC_FUNCT = 3,
	DEC_FUNCT = 4,

	/* OPCODE 9 */
	JMP_FUNCT = 1,
	BNE_FUNCT = 2,
	JSR_FUNCT = 3,

	NONE_FUNCT = -1
} funct;
typedef enum registers {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    NONE_REG = -1
} reg;
/* Represents a single code word */
typedef struct code_word {
	/* First byte: ARE+funct */
	unsigned int ARE:3;
	unsigned int funct:5;
	/* 2nd byte: destination+addressing, source */
	unsigned int dest_register:3;
	unsigned int dest_addressing:2;
	unsigned int src_register:3;
	/* Third byte: source addressing, opcode */
	unsigned int src_addressing:2;
	unsigned int opcode: 6;
} code_word;

/* Represents a single data word. */
typedef struct data_word {
	int ARE:3;
	/* The data content itself (a method for putting data into these field is defined) */
	unsigned int data:21;
} data_word;

/* Processes a code line in first pass */
/*first parameter is the line and second parameter is the code img*/
int process_code(char *line, int i, int *ci, char *code_img);
/* Puts the opcode and the funct values inside the arguments by the name of the command */
void get_opcode_func(char* cmd, opcode *opcode_out, funct *funct_out);
/*checks addressing type*/
addressing_type get_addressing_type(char *operand);
/*check if addressing type is through register*/
bool is_register(char *operand);
/*check if addressing type is direct*/
bool is_direct(char *operand);
/*check if addressing type is immediate*/
bool is_immediate(char  *operand);
/*check if addressing type is relative*/
bool is_relative(char *operand);

code_word *get_code_word(opcode curr_opcode, funct curr_funct, int op_count, char **operands);

bool validate_op_addr(addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count, int op2_valid_addr_count,...);

reg get_register_by_name(char *name);

data_word *build_data_word(addressing_type addressing, int data);
