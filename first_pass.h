#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H
/* Processes a code line in first pass */
#include "globals.h"

/**
 * Processes a single line in the first pass
 * @param line The line text
 * @param datas The data symbol table
 * @param codes The code symbol table
 * @param externals The externals symbol table
 * @param IC A pointer to the current instruction counter
 * @param DC A pointer to the current data counter
 * @param code_img The code image array
 * @param data_img The data image array
 * @return Whether succeeded.
 */
bool process_line_fpass(line_info line, long *IC, long *DC, machine_word **code_img, long *data_img,
                        table *symbol_table);

#endif