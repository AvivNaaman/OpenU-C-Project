#include <stdio.h>
#include "processfile.h"

/* Fully processes the assembly file, and writing all the associated files. Returns whether succeeded. */
int process_file(char *filename) {
	FILE *filedes; /* Current assembly file descriptor to process */
	filedes = fopen(filename, "r");
	if (filedes == NULL) {
		/* if file couldn't be opened, write to stderr. */
		fprintf(stderr,"Error: Can't open file \"%s\" for reading.", filename);
		return 0; /* TODO: Ask if the program should halt when any error occur, or just current file processing should. */
	}
	/* File opened successfully, start first pass: */

	/* First pass done right. start second pass: */

	/* Everything was done. Write outputs to *filename.ob/.ext/.ent */
}