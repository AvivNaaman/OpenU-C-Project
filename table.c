/* Implements a basic table ("dictionary") data structure. sorted by value, ascending. */
#include <stdlib.h>
#include <stdio.h>
#include "table.h"

/* Adds a new entry to the table. keep it sorted by value. creates new table if the sent table is null. */
table add_item(table tab, char key[], int value) {
	/* TODO: Reimplement. Make sure it's ordered!make */
	return NULL;
}

/* TODO: Documentation */

table_entry *find_by_key(table_entry *table, char *key) {
	table_entry *temp;
	/* As long as not end of table */
	for (temp = table; temp ;temp = temp->next){
		/* if entry's key equals the key argument, return the pointer to the entry */
		if (key == temp->key) return temp;
	}
	return NULL;
}

table_entry *find_by_value(table_entry * table, int value){
	table_entry *temp;
	/* As long as not end of table */
	for (temp = table; temp ;temp = temp->next){
		/* if entry's value equals the value argument, return the pointer to the entry */
		if (value == temp->value) return temp;
	}
	return NULL;
}