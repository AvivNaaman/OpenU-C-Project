
#include "code.h"
/*
 * Analyzes a line in first pass: adds to symbol table if needed,
 * returns the 24-bit word count that the line requires in final assembled code
 */
int process_line_fpass(char *line, table *datas, table *codes, table *externals, int *IC, int *DC, machine_word **code_img, machine_data **data_img);

/* Processes a single line of assembly source in the second pass. returns whether an error occurred. */
int process_line_spass(char *line, table *ent_table, table code_table, int *ic, table ext_table, table data_table, machine_word **code_img);
