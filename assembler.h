/* Contains function prototypes of the very base of the assembler (for file & line info).  */

/** Returns the line number that is being processed. */
long get_curr_line();

/** Returns the name of the file that is being processed */
char *get_curr_filename();
