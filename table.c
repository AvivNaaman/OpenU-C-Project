/* Implements a basic table ("dictionary") data structure. sorted by value, ascending. */
#include <stdlib.h>
#include <stdio.h>
#include "table.h"
#include <string.h>

/* Adds a new entry to the table. the value argument is converted to char[3], which is 24bit word. */
void add_item(table *tab, char *key, int value) {
	table prev_entry, curr_entry, new_entry;
	new_entry = (table)malloc(sizeof(table_entry));
	if (new_entry == NULL) {
		printf("Memory Allocation Failed");
		exit(1); /* if can't allocate memory, can't do anything anymore. */
	}
	new_entry->key = key;
	new_entry->value = value;
	/* if the table's null, set the new entry as the head. */
	if ((*tab) == NULL || (*tab)->value > value) {
		new_entry->next = (*tab);
		(*tab) = new_entry;
		return;
	}

	curr_entry = *tab;
	while (curr_entry != NULL && curr_entry->value < value) {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
	}

	new_entry->next = curr_entry;
	prev_entry->next = new_entry;
}

/* TODO: Documentation =>\/ */

table_entry *find_by_key(table tab, char *key) {
	while (tab != NULL) {
		if (strcmp(tab->key,key) == 0) return tab;
		tab = tab->next;
	}
	return NULL;
}

table_entry *find_by_value(table  tab, int value){
	while (tab != NULL) {
		if (tab->value == value) return tab;
		tab = tab->next;
	}
	return NULL;
}

void free_table(table tab) {
	table prev_entry, curr_entry = tab;
	while (curr_entry != NULL) {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
		free(prev_entry);
	}
}