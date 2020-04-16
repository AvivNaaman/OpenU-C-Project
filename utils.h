/* Useful utilities */

/* Returns whether a string contains a symbol from a certain index. */
int parse_symbol(char *line, int, char*);

/* Writes 2 bytes in order to buffer from index */
void write_word(char* buffer, int index, char byte0, char byte1, char byte2);

/* TODO: DOC */
is_int(char* string);