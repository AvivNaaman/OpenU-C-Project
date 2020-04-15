/* Single table entry */
typedef struct entry {
	/* Next table entry */
	struct entry *next;
	/* key (string) and value (int) pair */
	char *key;
	int value;
} table_entry;

/* Adds a new entry to the table. */
table_entry *add_item(table_entry *table, char *key, int value);

/* TODO: Document functions down here */
table_entry *find_by_key(table_entry *table, char *key);

table_entry *find_by_value(table_entry * table, int value);