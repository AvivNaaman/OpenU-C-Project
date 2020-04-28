#include "table.h"
#include "globals.h"

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
bool analyze_operands(char *line, int i, char **destination, int *operand_count);
