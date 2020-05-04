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

/**
 * Contains the current file name that is being processed (for error printing)
 */
static char *curr_filename;
/**
 * Contains the current source line that is being processed (for error printing)
 */
static int curr_line;

/**
 * Processes a single assembly source file
 * @param filename
 */
static void process_file(char *filename);

/**
 * Entry point - a 24bit assembler
 */
int main(int argc, char *argv[]) {
	int i;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* foreach argument (file name), send it for full processing. */
		process_file(argv[i]);
	}
	return 0;
}


static void process_file(char *filename) {
	/* Memory address counters */
	long ic, dc, icf, dcf;
	bool is_success; /* is succeeded so far */
	char *input_filename;
	char temp_line[MAX_LINE_LENGTH + 1]; /* temporary string for storing line, read from file */
	FILE *file_des; /* Current assembly file descriptor to process */
	machine_data *data_img[CODE_ARR_IMG_LENGTH]; /* Contains an image of the machine code */
	machine_word *code_img[CODE_ARR_IMG_LENGTH];
	/* Out symbol table */
	table symbol_table = NULL;

	/* We get just the name, without the extension, so we have to add it (.as+'\0'): */
	input_filename = malloc_with_check(strlen(filename) + (4 * sizeof(char)));
	strcpy(input_filename, filename);
	strcat(input_filename, ".as"); /* add file extension */

	file_des = fopen(input_filename, "r");
	/* Put filename in static variables for error messaging */
	curr_filename = input_filename;
	/* Make sure opening the file succeeded */
	if (file_des == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: file \"%s.as\" is inaccessible for reading.", filename);
		return;
	}


	/* Initialize  */
	is_success = TRUE;
	ic = IC_INIT_VALUE;
	dc = 0;

	/* start first pass: */
	/* Read line - stop if read failed (when NULL returned) - usually when EOF. increase line counter for error printing. */
	for (curr_line = 1; fgets(temp_line, MAX_LINE_LENGTH, file_des) != NULL; curr_line++) {
		is_success &= process_line_fpass(temp_line, &ic, &dc, code_img, data_img, &symbol_table);
	}
	if (!is_success) {
		printf("Stopped assembling the file %s.as. See the above output for more information.\n", filename);
		return;
	}

	/* Save ICF & DCF (1.18) */
	icf = ic;
	dcf = dc;
	ic = 100;
	/* Now let's add IC to each DC for data symbols in table (step 1.19) */
	add_value_to_type(symbol_table, icf, DATA_SYMBOL);

	/* First pass done right. start second pass: */
	rewind(file_des); /* Reread the file from the beginning */
	for (curr_line = 1; !feof(file_des); curr_line++) {
	    int i;
		fgets(temp_line, MAX_LINE_LENGTH, file_des); /* Get line */
		MOVE_TO_NOT_WHITE(temp_line,i)
		if (code_img[ic - 100] != NULL||temp_line[i]=='.')
			is_success &= process_line_spass(temp_line, &ic, code_img, &symbol_table);
	}
	if (!is_success) {
		printf("Stopped assembling the file %s.as. See the above output for more information.\n", filename);
		return;
	}

	/* Everything was done. Write to *filename.ob/.ext/.ent */
	if (!write_output_files(code_img, data_img, icf, dcf, filename, symbol_table)) {
		printf("Failed to write some of the output files. See the above output for more information.\n");
	}

	/* Now let's free some pointer: */
	/* current file name */
	free(input_filename);
	/* Free symbol table */
	free_table(symbol_table);
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