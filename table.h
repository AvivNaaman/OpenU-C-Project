#include "utils.h"
/* pointer to table  */
typedef struct entry* table;
/* Single table entry */
typedef struct entry {
	/* Next table entry */
	table *next;
	/* key (string) and value (int) pair */
	char value[3];
	char key[MAX_LINE_LENGTH];
} table_entry;

/* Adds a new entry to the table. */
table_entry *add_item(table_entry *table, char key[], int value);

/* Returns a pointer to the entry where the key is the same as the argument. if no such one, returns null */
table_entry *find_by_key(table_entry *table, char key[]);

/* Returns a pointer to the entry where the value is the same as the argument. if no such one, returns null */
table_entry *find_by_value(table_entry * table, int value);