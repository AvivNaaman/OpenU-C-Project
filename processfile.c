#include <stdio.h>
#include "processfile.h"
#include "globals.h" /* constants */
#include "first_pass_line.h"

/* Fully processes the assembly file, and writing all the associated files. Returns whether succeeded. */
int process_file(char *filename) {
	int i, ic, dc, iserror;
	char temp_line[MAX_LINE_LENGTH+1]; /* temporary string for storing line, read from file */
	FILE *filedes; /* Current assembly file descriptor to process */
	char code_img[CODE_ARR_IMG_LENGTH], data_img[CODE_ARR_IMG_LENGTH]; /* Contains an image of the machine code */
	/* We'll use multiple symbol tables, for each symbol type: .data, .code, .ext, .ent */
	table data_table, code_table, ext_table, ent_table;

	filedes = fopen(filename, "r");
	if (filedes == NULL) {
		/* if file couldn't be opened, write to stderr. */
		fprintf(stderr,"Error: Can't open file \"%s\" for reading.", filename);
		return 0; /* TODO: Ask if the program should halt when any error occur, or just current file processing should. */
	}
	/* Initialize  */
	data_table = code_table = ext_table = NULL;
	iserror = FALSE;
	ic = 100;
	dc = 0;
	/* File opened successfully, start first pass: */
	while (!feof(filedes)) {
		fgets(temp_line, MAX_LINE_LENGTH, filedes); /* Get line */
		 iserror = iserror || firstpass_analyze_line(temp_line, data_table, code_table, ext_table, &ic, &dc, code_img, data_img, filename);
	}
	/* First pass done right. start second pass: */

	/* Everything was done. Write outputs to *filename.ob/.ext/.ent */
}