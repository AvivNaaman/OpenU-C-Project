/* Implements a basic table ("dictionary") data structure. sorted by value, ascending. */
#include <stdlib.h>
#include <stdio.h>
#include "table.h"

/* Adds a new entry to the table. keep it sorted by value. creates new table if the sent table is null. */
table_entry *add_item(table_entry *table, char key[3], int value) {
	table_entry *curr_temp, *prev_temp, *new_entry;

	/* create a new struct entry with the value and key */
	new_entry = (table_entry*)malloc(sizeof(table_entry));
	if (new_entry == NULL) {
		fputs("Memory allocation failed.", stderr);
	}

	/* if no head, put current as head and return it */
	if (table == NULL) {
		return new_entry;
	}

	/* Move in the table until: prev_temp->value < value < curr_temp->value - the place to insert is between */
	for (curr_temp = table; curr_temp->value < value && curr_temp != NULL;) {
		prev_temp = curr_temp;
		curr_temp = curr_temp->next;
	}
	new_entry->key = key;
	new_entry->value = value;
	new_entry->next = curr_temp;
	prev_temp->next = new_entry;

	return table;
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