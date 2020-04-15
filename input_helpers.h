typedef enum instruction {
	DATA,
	EXTERN,
	ENTRY,
	STRING,
	NONE /* no instruction found */
} instruction_type;

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction_type find_instruction_from_index(char *string, int index);