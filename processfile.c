#include <stdio.h>
#include "processfile.h"
#include "processline.h"
#include "utils.h"

/* Globals because we want to access them only across this file (for get_curr_line/curr_filename, mostly for printing errors) */
static char *curr_filename;
static int curr_line;

/* Fully processes the assembly file, and writing all the associated files. Returns whether succeeded. */
void process_file(char *filename) {
	int ic, dc, is_error;
	char temp_line[MAX_LINE_LENGTH+1]; /* temporary string for storing line, read from file */
	FILE *file_des; /* Current assembly file descriptor to process */
	char code_img[CODE_ARR_IMG_LENGTH], data_img[CODE_ARR_IMG_LENGTH]; /* Contains an image of the machine code */
	/* We'll use multiple symbol tables, for each symbol type: .data, .code, .ext, .ent */
	table data_table, code_table, ext_table, ent_table;

	file_des = fopen(filename, "r"); /* TODO: The filename, passed from command line args, shouldn't contain extension! it should be added here! */
	if (file_des == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: file \"%s\" is inaccessible for reading.", filename);
		return;
	}
	/* Put in static variables for error messaging */
	curr_filename = filename;
	/* Initialize  */
	data_table = code_table = ext_table = NULL;
	is_error = FALSE;
	ic = 100;
	dc = 0;
	/* File opened successfully, start first pass: */
	for (curr_line = 1; !feof(file_des); curr_line++) {
		fgets(temp_line, MAX_LINE_LENGTH, file_des); /* Get line */ /* TODO Make sure pointer to pointer to struct is sent in table arguments! */
		is_error = is_error || process_line_fpass(temp_line, &data_table, &code_table, &ext_table, &ic, &dc, code_img, data_img);
	}
	if (is_error) {
		printf("Stopped assembling the file %s. See the above output for more information.\n", filename);
		return;
	}
	/* First pass done right. start second pass: */
	rewind(file_des); /* Reread the file from the beginning */
	for (curr_line = 1; !feof(file_des); curr_line++)  {
		fgets(temp_line, MAX_LINE_LENGTH, file_des); /* Get line */ /* TODO: Implement */
		/*is_error = is_error || process_line_spass(temp_line, &ent_table, &code_table, &ic, ext_table, data_table);*/
	}
	/* Everything was done. Write outputs to *filename.ob/.ext/.ent */
}

int get_curr_line() {
	return curr_line;
}

char *get_curr_filename() {
	return curr_filename;
}