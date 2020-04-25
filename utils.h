/* Booleans */
typedef enum booleans {
	FALSE = 0, TRUE =1
} bool;

/* Maximum size of machine code array - provides at least 200 code lines in *.as file */
#define CODE_ARR_IMG_LENGTH 1200
#define MAX_LINE_LENGTH 80

#define IC_INIT_VALUE 100 /* Initial value of IC */

/* moves the index to the next place in string where the char isn't white */
#define MOVE_TO_NOT_WHITE(string, index) \
		for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
		;

#define MOVE_UNTIL_CHAR_OR_WHITE(string, index, char)\
		for (;!string[index] || string[index] == '\t' || string[index] == ' ' || string[index] == char; ++(index)) ;

/* Returns whether an error ocurred during the try of parsing the symbol. puts the symbol into the second buffer. */
bool parse_symbol(char *line, char *symbol_dest);

/* Writes 2 bytes in order to buffer from index */
void write_word(char* buffer, int index, char byte0, char byte1, char byte2);

/* Returns TRUE if the string is int */
int is_int(char* string);

/* Returns a 3-byte array that contains the lower 24byte content of the argument. */
char *int_to_word(int);

/* allocates a memory block in length of size argument. returns NULL if failed, and prints error if failed. */
void *malloc_with_check(long size);

/*Returns true if the string is legal label name*/
int is_legal_label(char* string);

/*Returns TRUE if string contains non alphanumeric chars*/
int is_contain_non_alphanumeric(char *string);

/*Returns TRUE if string is saved word*/
int is_saved_word(char *string);

/* Prints a detailed error message to the user, including file, line, and message. */
void print_error(char *message);