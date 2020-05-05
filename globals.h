/* Contains useful global definitions for the whole projects */

/* Prevent double definitions */
#ifndef GLOBALS_H
#define GLOBALS_H

/** Boolean (t/f) definition */
typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

/** Maximum size of code image and data image */
#define CODE_ARR_IMG_LENGTH 1200

/** Maximum length of a single source line  */
#define MAX_LINE_LENGTH 80

/** Initial IC value */
#define IC_INIT_VALUE 100

/** moves the index to the next place in string where the char isn't white */
#define MOVE_TO_NOT_WHITE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;

/** Operand addressing type */
typedef enum addressing_types {
	/** Immediate addressing (0) */
	IMMEDIATE = 0,
	/** Direct addressing (1) */
	DIRECT = 1,
	/** Relative addressing (2) */
	RELATIVE = 2,
	/** Register addressing */
	REGISTER = 3,
	/** Failed/Not detected addressing */
	NONE_ADDR = -1
} addressing_type;

/** Commands opcode */
typedef enum opcodes {
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

	/** Failed/Error */
	NONE_OP = -1
} opcode;

/** Commands funct */
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

	/** Default (No need/Error) */
	NONE_FUNCT = 0
} funct;

/** Registers */
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
/** Represents a single code word */
typedef struct code_word {
	/* First byte: ARE+funct */
	unsigned int ARE: 3;
	unsigned int funct: 5;
	/* 2nd byte: destination+addressing, source */
	unsigned int dest_register: 3;
	unsigned int dest_addressing: 2;
	unsigned int src_register: 3;
	/* Third byte: source addressing, opcode */
	unsigned int src_addressing: 2;
	unsigned int opcode: 6;

} code_word;

/** Represents a single data word. */
/* TODO: Migrate to 3-byte because we don't know for sure that int is 32+ bits! */
typedef struct data_word {
	unsigned int ARE: 3;
	/* The data content itself (a method for putting data into these field is defined) */
	unsigned int data: 21;
} data_word;

/** Represents a general machine code word contents */
typedef struct machine_code_word {
	short length; /* The length of the code word (if it's a code word), including data words required by it. */
	union word {
		data_word *data;
		code_word *code;
	} word;
} machine_word;

/** Instruction type */
typedef enum instruction {
	DATA,
	EXTERN,
	ENTRY,
	STRING,
	NONE_INST /* no instruction found */
} instruction_type;

/** General data word */
/* TODO: Migrate to long array instead of this. */
typedef struct machine_code_data {
	char byte0;
	char byte1;
	char byte2;
} machine_data;

#endif
