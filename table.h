#include "utils.h"
/* pointer to table  */
typedef struct entry* table;
/* Single table entry */
typedef struct entry {
	/* Next table entry */
	table next;
	/* one-word sized value (24bit=3byte) */
	int value;
	/* string key */
	char *key;
} table_entry;

/* Adds a new entry to the table. */
void add_table_item(table *tab, char *key, int value);

/* Returns a pointer to the entry where the key is the same as the argument. if no such one, returns null */
table_entry *find_by_key(table tab, char key[]);

/* Returns a pointer to the entry where the value is the same as the argument. if no such one, returns null */
table_entry *find_by_value(table tab, int value);

void free_table(table tab); /* TODO: DOC */