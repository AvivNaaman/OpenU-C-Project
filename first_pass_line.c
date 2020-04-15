#include "first_pass_line.h"
#include "globals.h"
/*
 * processes a single instruction line. returns whether an error occured.
 * line - the text line from .asm file.
 * datas, code, externals - symbol tables for .data, .code and .external instruction.
 * IC - a pointer to the instruction counter.
 * DC - a pointer to the data counter.
 */
int firstpass_analyze_line(char *line, table datas, table codes, table externals, int *IC, int *DC) {
	/* TODO: implement */
	return FALSE;
}