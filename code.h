#include "utils.h"
#include "table.h"

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

	NONE_FUNCT = 0
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
	unsigned int ARE:3;
	/* The data content itself (a method for putting data into these field is defined) */
	unsigned int data:21;
} data_word;

/* Represents a general machine code word contents */
typedef struct machine_code_word {
	int length; /* The length of the code word (if it's a code word), including data words required by it. */
	union word {
		data_word *data;
		code_word *code;
	} word;
} machine_word;

/* Processes a code line in first pass */
/*first parameter is the line and second parameter is the code img*/
int process_code(char *line, int i, int *ic, machine_word **code_img);
/* Puts the opcode and the funct values inside the arguments by the name of the command */
void get_opcode_func(char* cmd, opcode *opcode_out, funct *funct_out);
/* Returns the addressing type enum value of the operand's addressing type */
addressing_type get_addressing_type(char *operand);
/* Returns whether an operand is a register. */
bool is_register(char *operand);
/* Returns whether an operand is directly addressed. */
bool is_direct(char *operand);
/* Returns whether an operand is immediately addressed. */
bool is_immediate(char  *operand);
/* Returns whether an operand is relatively addressed. */
bool is_relative(char *operand);

/*
 * builds a code binary word by the opcode, funct, operand count and operands. validates that the operands are in the correct form.
 * if operation or validation fail, prints an error and returns NULL. otherwise returns a pointer to struct code_word, with the required contents.
 */
code_word *get_code_word(opcode curr_opcode, funct curr_funct, int op_count, char *operands[2]);

/* Returns whether both operands' addressing types are valid, by the valid addressing types passed after the hard-coded arguments.
 * addressing_type arguments are the types to check, both valid_addr_counts are the count for each valida adreesing type list length.
 */
bool validate_op_addr(addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count, int op2_valid_addr_count,...);

/* Returns the register enum value by it's name. if not found, returns NONE_REG */
reg get_register_by_name(char *name);

/* builds a data word by the addressing type and the data of it. */
data_word *build_data_word(addressing_type addressing, long data, bool is_extern_symbol);

/* TODO: Doc better - returns whether an error occurred */
int analyze_operands(char *line, int i, char *destination[2], int *operand_count);

int add_symbols_to_code(char *line, int *ic, machine_word **code_img, table data_table, table code_table, table ext_table);