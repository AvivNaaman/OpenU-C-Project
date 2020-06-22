#include <stdio.h>
#include "assembler.h"
#include <string.h>
#include <stdlib.h>
#include "writefiles.h"
#include "utils.h"
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
 * @return Whether succeeded
 */
static bool process_file(char *filename);

/**
 * Entry point - 24bit assembler
 * Assembly language specification appears in booklet
 */
int main(int argc, char *argv[]) {
	int i;
	/* To break line if needed */
	bool succeeded = TRUE;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* if last process failed and there's another file, break line: */
		if (!succeeded) puts("");
		/* foreach argument (file name), send it for full processing. */
		succeeded = process_file(argv[i]);
		/* Line break if failed */
	}
	return 0;
}

static bool process_file(char *filename) {
	/* Memory address counters */
	long ic, dc, icf, dcf;
	bool is_success; /* is succeeded so far */
	char *input_filename;
	char temp_line[MAX_LINE_LENGTH + 1]; /* temporary string for storing line, read from file */
	FILE *file_des; /* Current assembly file descriptor to process */
	long data_img[CODE_ARR_IMG_LENGTH]; /* Contains an image of the machine code */
	machine_word *code_img[CODE_ARR_IMG_LENGTH];
	/* Out symbol table */
	table symbol_table = NULL;

	/* We get just the name, without the extension, so we have to add it (.as+'\0'): */
	input_filename = malloc_with_check(strlen(filename) + (4 * sizeof(char)));
	strcpy(input_filename, filename);
	strcat(input_filename, ".as");

	/* Open file, exit in failure */
	file_des = fopen(input_filename, "r");
	/* Put filename in static variables for error messaging */
	curr_filename = input_filename;
	/* Make sure opening the file succeeded */
	if (file_des == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: file \"%s.as\" is inaccessible for reading.", filename);
		free(input_filename); /* The only allocated space is for the full file name */
		return FALSE;
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

	/* if first pass didn't fail, start the second pass */
	if (is_success) {

		/* Save ICF & DCF (1.18) */
		icf = ic;
		dcf = dc;
		ic = 100;
		/* Now let's add IC to each DC for data symbols in table (step 1.19) */
		add_value_to_type(symbol_table, icf, DATA_SYMBOL);

		/* First pass done right. start second pass: */
		rewind(file_des); /* Reread the file from the beginning */
		for (curr_line = 1; !feof(file_des); curr_line++) {
			int i = 0;
			fgets(temp_line, MAX_LINE_LENGTH, file_des); /* Get line */
			MOVE_TO_NOT_WHITE(temp_line, i)
			if (code_img[ic - 100] != NULL || temp_line[i] == '.')
				is_success &= process_line_spass(temp_line, &ic, code_img, &symbol_table);
		}

		/* Write files if second pass succeeded */
		if (is_success) {
			/* Everything was done. Write to *filename.ob/.ext/.ent */
			is_success = write_output_files(code_img, data_img, icf, dcf, filename, symbol_table);
		}
	}

	/* Now let's free some pointer: */
	/* current file name */
	free(input_filename);
	/* Free symbol table */
	free_table(symbol_table);
	/* Free code & data buffer contents */
	free_code_image(code_img, icf);

	/* return whether every assembling succeeded */
	return is_success;
}

long get_curr_line() {
	return curr_line;
}

char *get_curr_filename() {
	return curr_filename;
}