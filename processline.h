#include "table.h"
/*
 * Analyzes a line in first pass: adds to symbol table if needed,
 * returns the 24-bit word count that the line requires in final assembled code
 */
bool process_line_fpass(char *line, table *datas, table *codes, table *externals, int *IC, int *DC, char *code_img, char *data_img, char *filename);

/* Processes a single line of assembly source in the second pass. returns whether an error occurred. */
bool process_line_spass(char *line, table *ent_table, table *code_table, int *ic, table ext_table, table data_table);
