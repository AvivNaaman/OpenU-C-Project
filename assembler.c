#include <stdio.h>
#include "processfile.h"

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
