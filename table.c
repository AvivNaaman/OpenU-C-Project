/* Implements a basic table ("dictionary") data structure. sorted by value, ascending. */
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <string.h>
#include "table.h"

/* Adds a new entry to the table. the value argument is converted to char[3], which is 24bit word. */
void add_table_item(table *tab, char *key, long value) {
	char *temp;
	table prev_entry, curr_entry, new_entry;
	new_entry = (table)malloc(sizeof(table_entry));
	if (new_entry == NULL) {
		printf("Memory Allocation Failed");
		exit(1); /* if can't allocate memory, can't do anything anymore. */
	}
	/* Prevent "Aliasing" of pointers. Don't worry-when we free the list, we also free these allocated char ptrs */
	temp = (char *)malloc_with_check(strlen(key));
	strcpy(temp, key);
	new_entry->key = temp;
	new_entry->value = value;
	/* if the table's null, set the new entry as the head. */
	if ((*tab) == NULL || (*tab)->value > value) {
		new_entry->next = (*tab);
		(*tab) = new_entry;
		return;
	}

	curr_entry = *tab;
	while (curr_entry != NULL && (curr_entry->value < value||(curr_entry->value == 0 && value == 0))) {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
	}

	new_entry->next = curr_entry;
	prev_entry->next = new_entry;
}

/* TODO: Documentation =>\/ */

table_entry *find_by_key(table tab, char *key) {
	while (tab != NULL) {
		if (strcmp(key,tab->key) == 0) return tab;
		tab = tab->next;
	}
	return NULL;
}

table_entry *find_by_value(table  tab, long value){
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
		free(prev_entry->key); /* Didn't forget you!ssss */
		free(prev_entry);
	}
}

void add_to_each_value(table tab, long to_add) {
	table curr_entry;
	/* for each entry, increase value by to_add */
	for (curr_entry = tab; curr_entry != NULL; curr_entry = curr_entry->next) {
		curr_entry->value += to_add;
	}
}