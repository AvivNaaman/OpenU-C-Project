/* Represents an allowed addressing type */
typedef enum addressing_types {
	/* Immediate addressing (0) */
	IMMEDIATE = 0,
	/* Direct addressing (1) */
	DIRECT = 1,
	/* Relative addressing (2) */
	RELATIVE = 2,
	/* address via register*/
	REG = 3,
	/*illegal addressing type*/
	ILLEAGEL_ADDRESS = 4
} addressing_type ;

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
	STOP_OP = 15,

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
typedef enum registers{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
}registers;
/* Represents a single code word */
typedef struct code_word {
	/* First byte: ARE+funct */
	int ARE:3;
	int funct:5;
	/* Second byte: destination+addressing, source */
	int dest_register:3;
	int dest_addressing:2;
	int src_register:3;
	/* Third byte: source addressing, opcode */
	int src_addressing:2;
	int opcode: 6;
} code_word;

/* Represents a single data word. */
typedef struct data_word {
	int ARE:3;
	/* The data content itself (a method for putting data into these field is defined) */
	int data:21;
} data_word;

/* Processes a code line in first pass */
/*first parameter is the line and second parameter is the code img*/
int process_code(char *line, int i);
/* Puts the opcode and the funct values inside the arguments by the name of the command */
void get_opcode_func(char* cmd, opcode *opcode_out, funct *funct_out);
/*checks addressing type*/
addressing_type check_type(char *operand);
/*check if addressing type is through register*/
int is_register(char *operand);
/*check if addressing type is direct*/
int is_direct(char *operand);
/*check if addressing type is immediate*/
int is_immediate(char  *operand);
/*check if addressing type is relative*/
int is_relative(char *operand);