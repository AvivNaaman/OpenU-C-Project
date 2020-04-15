/* Booleans */
#define TRUE 1
#define FALSE 0

/* Maximum size of machine code array - provides at least 200 code lines in *.as file */
#define CODE_ARR_IMG_LENGTH 1200
#define MAX_LINE_LENGTH 80

/* moves the index to the next place in string where the char isn't white */
#define MOVE_TO_NOT_WHITE(string, index) \
		for (;!string[index] && (string[index] == '\t' || string[index] == ' '); ++(index)) ;