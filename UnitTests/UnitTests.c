#include "assertions.h"
#include <stdio.h>
#include "../table.h"
#include "UnitTests.h"

int main() {
	int total, failed;
	total = failed = 0;
	puts("Starting Tests...");

	/* TESTS HERE. USER MACROS! */
	puts("\nTesting table.h");
	CALL_TEST(Table, total, failed);

	puts("\nTesting utils.h");
	CALL_TEST(Utils, total, failed);

	puts("\nTesting instruction_handler.h");
	CALL_TEST(Instructions, total, failed);

	puts("Testing code.h");
	CALL_TEST(Code, total, failed);

	/* END TESTS */

	printf("\nPassed %d/%d.\n", total-failed, total);
	puts("Done. See the above output for more details. Ignore non-assertion error ");

	return(!!failed);
}
