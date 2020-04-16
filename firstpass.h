#include "table.h"
/*
 * Analyzes a line in first pass: adds to symbol table if needed,
 * returns the 24-bit word count that the line requires in final assembled code
 */
int process_line_fpass(char *line, table datas, table codes, table externals, int *IC, int *DC, char *code_img, char *data_img, char *filename);