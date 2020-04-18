#include <stdio.h>
#include <stdlib.h>
#include "processfile.h"
#include "processline.h"
#include "utils.h"
/* Fully processes the assembly file, and writing all the associated files. Returns whether succeeded. */
void process_file(char *filename) {
	int ic, dc, iserror;
	char temp_line[MAX_LINE_LENGTH+1]; /* temporary string for storing line, read from file */
	FILE *filedes; /* Current assembly file descriptor to process */
	char code_img[CODE_ARR_IMG_LENGTH], data_img[CODE_ARR_IMG_LENGTH]; /* Contains an image of the machine code */
	/* We'll use multiple symbol tables, for each symbol type: .data, .code, .ext, .ent */
	table data_table, code_table, ext_table, ent_table;

	filedes = fopen(filename, "r"); /* TODO: The filename, passed from command line args, shouldn't contain extension! it should be added here! */
	if (filedes == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: Can't open file \"%s\" for reading.", filename);
		return; /* TODO: Ask if the program should halt when any error occur, or just current file processing should. */
	}
	/* Initialize  */
	data_table = code_table = ext_table = NULL;
	iserror = FALSE;
	ic = 100;
	dc = 0;
	/* File opened successfully, start first pass: */
	while (!feof(filedes)) {
		fgets(temp_line, MAX_LINE_LENGTH, filedes); /* Get line */ /* TODO Make sure pointer to pointer to struct is sent in table arguments! */
		iserror = iserror || process_line_fpass(temp_line, &data_table, &code_table, &ext_table, &ic, &dc, code_img, data_img, filename);
	}
	if (iserror) {
		printf("Assemble of the file %s stopped.\n", filename);
		printf("Syntax error found in the file. See the above output for more information");
		return;
	}
	/* First pass done right. start second pass: */
	rewind(filedes); /* Reread the file from the beginning */
	while (!feof(filedes)) {
		fgets(temp_line, MAX_LINE_LENGTH, filedes); /* Get line */ /* TODO: Implement */
		iserror = iserror || process_line_spass(temp_line, &ent_table, &code_table, &ic, ext_table, data_table);
	}
	/* Everything was done. Write outputs to *filename.ob/.ext/.ent */
}