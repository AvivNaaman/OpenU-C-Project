#include <stdio.h>
#include "assembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "writefiles.h"
#include "utils.h"
#include "table.h"
#include "first_pass.h"
#include "second_pass.h"

/* Global, static variables for getting the current line and file name being processed. */
static char *curr_filename;
static int curr_line;

/*
 * Fully processes the assembly file, and writing all the associated files. Returns whether succeeded.
 * Accessible for the main only.
 */
void process_file(char *filename);

/* Entry Point */
int main(int argc, char *argv[]) {
	int i;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* foreach argument (file name), send it for full processing. */
		process_file(argv[i]);
	}
	return 0;
}

/* Fully processes a single assembly file, and writing all the associated output files. */
void process_file(char *filename) {
	/* Memory address counters */
	long ic, dc, icf, dcf;
	bool is_success; /* is an error occurred during current stage */
	char *input_filename;
	char temp_line[MAX_LINE_LENGTH + 1]; /* temporary string for storing line, read from file */
	FILE *file_des; /* Current assembly file descriptor to process */
	machine_data *data_img[CODE_ARR_IMG_LENGTH]; /* Contains an image of the machine code */
	machine_word *code_img[CODE_ARR_IMG_LENGTH];
	/* We'll use multiple symbol tables, for each symbol type: .data, .code, .ext, .ent and for external labels references */
	table data_table, code_table, ext_table, ent_table, ext_references;

	/* We get just the name, without the extension, so we have to add it (.as+'\0'): */
	input_filename = malloc_with_check(strlen(filename) + (4 * sizeof(char)));
	strcpy(input_filename, filename);
	strcat(input_filename, ".as"); /* add extension */

	file_des = fopen(input_filename, "r");
	free(input_filename); /* Release dynamically allocated memory for string */
	/* Make sure opening the file succeeded */
	if (file_des == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: file \"%s\" is inaccessible for reading.", filename);
		return;
	}

	/* Put filename in static variables for error messaging */
	curr_filename = input_filename;

	/* Initialize  */
	data_table = code_table = ext_table = NULL;
	is_success = TRUE;
	ic = 100;
	dc = 0;

	/* start first pass: */
	/* Read line - stop if read failed (when NULL returned) - usually when EOF. increase line counter for error printing. */
	for (curr_line = 1; fgets(temp_line, MAX_LINE_LENGTH, file_des) != NULL; curr_line++) {
		is_success &= process_line_fpass(temp_line, &data_table, &code_table, &ext_table, &ic, &dc, code_img, data_img);
	}
	if (!is_success) {
		printf("Stopped assembling the file %s. See the above output for more information.\n", filename);
		return;
	}

	/* Save ICF & DCF (1.18) */
	icf = ic;
	dcf = dc;
	ic = 100;
	/* Now let's add IC to each DC in data symbols table (step 1.19) */
	add_to_each_value(data_table, icf);

	/* First pass done right. start second pass: */
	rewind(file_des); /* Reread the file from the beginning */
	for (curr_line = 1; !feof(file_des); curr_line++) {
	    int i;
		fgets(temp_line, MAX_LINE_LENGTH, file_des); /* Get line */
		MOVE_TO_NOT_WHITE(temp_line,i)
		if (code_img[ic - 100] != NULL||temp_line[i]=='.')
			is_success &= process_line_spass(temp_line, &ent_table, &ext_references, code_table, &ic, ext_table,
			                                 data_table, code_img);
	}
	if (!is_success) {
		printf("Stopped assembling the file %s. See the above output for more information.\n", filename);
		return;
	}

	/* Everything was done. Write to *filename.ob/.ext/.ent */
	if (!write_output_files(code_img, data_img, icf, dcf, filename, ent_table, ext_references)) {
		printf("Failed to write some of the output files. See the above output for more information.\n");
	}

	/* TODO: Free these pointers on errors above! */
	/* Now let's free some pointer: */
	/* Free symbol tables */
	free_table(code_table);
	free_table(ext_table);
	free_table(data_table);
	free_table(ent_table);
	/* Free code & data buffer contents */
	free_code_image(code_img, icf);
	free_data_image(data_img, dcf);
}

long get_curr_line() {
	return curr_line;
}

char *get_curr_filename() {
	return curr_filename;
}