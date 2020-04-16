#include "assertions.h"
#include <stdio.h>
#include "../table.h"
#include "UnitTests.h"

int main() {
	int total, failed;
	total = failed = 0;
	puts("Starting Tests...");

	/* TESTS HERE. USER MACROS! */
	puts("Testing table.h");
	CALL_TEST(Table, total, failed);

	puts("Testing utils.h");
	CALL_TEST(Utils, total, failed);

	/* END TESTS */

	printf("\nPassed %d/%d.\n", total-failed, total);
	puts("Done. See the above output for more details.");
}
