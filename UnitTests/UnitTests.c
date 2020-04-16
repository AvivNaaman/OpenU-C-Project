#include "assertions.h"
#include <stdio.h>
#include "../table.h"
int total, failed;

int testTable(int *, int *);

int main() {
	puts("Starting Tests...");

	testTable(&total, &failed);

	puts("Done. Seed the above output for more details.");
}

int testTable(int *total, int *failed) {
	/* Initialize */
	table t0, t1, t2;
	t0 = t1 = t2 = NULL;
	/* END Initialize */

	add_item(t0, )

	return 0;
}
