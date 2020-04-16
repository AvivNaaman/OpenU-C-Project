#include <stdio.h>
#include "../table.h"
#include "assertions.h"
#include "UnitTests.h"

TEST(Table) {
	int i, prev_val;
	/* Initialize */
	table t, temp;
	t = NULL;
	/* END Initialize */

	add_item(&t, "test129", 129);
	add_item(&t, "test101", 101);
	add_item(&t, "test274", 274);
	add_item(&t, "test311", 311);
	add_item(&t, "test107", 107);

	/* Make sure the table is sorted */
	temp = t;
	prev_val = -1;
	for (i = 0; i < 5; i++) {
		assert_true(prev_val < temp->value, "testTable - is the table sorted", total, failed);
		temp = temp->next;
	}

	assert_int(129,find_by_key(t, "test129")->value, "testTable - find by key", total, failed);
	assert_int(274,find_by_key(t, "test274")->value, "testTable - find by key", total, failed);
	assert_int(107,find_by_key(t, "test107")->value, "testTable - find by key", total, failed);
	assert_int(101,find_by_key(t, "test101")->value, "testTable - find by key", total, failed);

	assert_str("test129",find_by_value(t, 129)->key, "testTable - find by key", total, failed);
	assert_str("test274",find_by_value(t, 274)->key, "testTable - find by key", total, failed);
	assert_str("test107",find_by_value(t, 107)->key, "testTable - find by key", total, failed);
	assert_str("test101",find_by_value(t, 101)->key, "testTable - find by key", total, failed);

	return 0;
}
