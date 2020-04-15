#include "table.h"
/*
 * Analyzes a line in first pass: adds to symbol table if needed,
 * returns the 24-bit word count that the line requires in final assembled code
 */
int firstpass_analyze_line(char *line, table_entry *externals, table_entry *entries);