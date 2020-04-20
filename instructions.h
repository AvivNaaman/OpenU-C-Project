typedef enum instruction {
	DATA,
	EXTERN,
	ENTRY,
	STRING,
	NONE_INST /* no instruction found */
} instruction_type;

typedef struct machine_code_data {
	char byte0;
	char byte1;
	char byte2;
} machine_data;

/* Returns the first instruction from the specified index. if no such one, returns NONE */
instruction_type find_instruction_from_index(char *string, int index);
/*
 * Processes a .string instruction from certain index of line. encodes each string char into the data array,
 * from the DC argument index. returns how much 24-bit word have been written.
 */
int process_string_instruction(char *line, int index, machine_data **data_img, int *dc);

/* same for data instruction */
int process_data_instruction(char *line, int index, machine_data **data_img, int *dc);