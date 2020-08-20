/* Implements a basic table ("dictionary") data structure. sorted by value, ascending. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "table.h"
#include "utils.h"

void add_table_item(table *tab, char *key, long value, symbol_type type) {
	char *temp_key;
	table prev_entry, curr_entry, new_entry;
	/* allocate memory for new entry */
	new_entry = (table) malloc_with_check(sizeof(table_entry));
	/* Prevent "Aliasing" of pointers. Don't worry-when we free the list, we also free these allocated char ptrs */
	temp_key = (char *) malloc_with_check(strlen(key) + 1);
	strcpy(temp_key, key);
	new_entry->key = temp_key;
	new_entry->value = value;
	new_entry->type = type;
	/* if the table's null, set the new entry as the head. */
	if ((*tab) == NULL || (*tab)->value > value) {
		new_entry->next = (*tab);
		(*tab) = new_entry;
		return;
	}

	/* Insert the new table entry, keeping it sorted */
	curr_entry = (*tab)->next;
	prev_entry = *tab;
	while (curr_entry != NULL && curr_entry->value < value) {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
	}

	new_entry->next = curr_entry;
	prev_entry->next = new_entry;
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

void add_value_to_type(table tab, long to_add, symbol_type type) {
	table curr_entry;
	/* for each entry, add value to_add if same type */
	for (curr_entry = tab; curr_entry != NULL; curr_entry = curr_entry->next) {
		if (curr_entry->type == type) {
			curr_entry->value += to_add;
		}
	}
}

table filter_table_by_type(table tab, symbol_type type) {
	table new_table = NULL;
	/* For each entry, check if has the type. if so, insert to the new table. */
	do {
		if (tab->type == type) {
			add_table_item(&new_table, tab->key, tab->value, tab->type);
		}
	} while ((tab = tab->next) != NULL);
	return new_table; /* It holds a pointer to the first entry, dynamically-allocated, so it's fine (not in stack) */
}

table_entry *find_by_types(table tab, char *key, int symbol_count, ...) {
	int i;
	symbol_type *valid_symbol_types = malloc_with_check((symbol_count) * sizeof(int));
	/* Build a list of the valid types */
	va_list arglist;
	va_start(arglist, symbol_count);
	for (i = 0; i < symbol_count; i++) {
		valid_symbol_types[i] = va_arg(arglist, symbol_type);
	}
	va_end(arglist);
	/* table null => nothing to dos */
	if (tab == NULL) {
		free(valid_symbol_types);
		return NULL;
	}
	/* iterate over table and then over array of valid. if type is valid and same key, return the entry. */
	do {
		for (i = 0; i < symbol_count; i++) {
			if (valid_symbol_types[i] == tab->type && strcmp(key, tab->key) == 0) {
				free(valid_symbol_types);
				return tab;
			}
		}
	} while ((tab = tab->next) != NULL);
	/* not found, return NULL */
	free(valid_symbol_types);
	return NULL;
}