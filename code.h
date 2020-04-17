/* Represents an allowed addressing type */
typedef enum addressing_types {
	/* Immediate addressing (0) */
	IMMEDIATE = 0,
	/* Direct addressing (1) */
	DIRECT,
	/* Relative addressing (2) */
	RELATIVE,
	/* Register (3) */
	REGISTER
} addressing_types ;

/* definition for each command opcode */
typedef enum opcode {
	MOV_OP = 0,
	CMP_OP = 1,
	ADD_OP = 2,
	SUB_OP = 2,
	LEA_OP = 4,
	CLR_OP = 5,
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,
	JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,
	RED_OP = 12,
	PRN_OP = 13,
	RTS_OP = 14,
	STOP_OP = 15
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
	JSR__FUNCT = 3
} funct;

int process_code(char *line, int i);