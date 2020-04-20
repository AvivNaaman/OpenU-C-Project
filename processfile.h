
/* Fully processes the assembly file, and writing all the associated files. Returns whether succeeded. */
void process_file(char *filename);

/* Returns the line number that is being processed. */
int get_curr_line();

/* Returns the name of the file that is being processed */
char *get_curr_filename();