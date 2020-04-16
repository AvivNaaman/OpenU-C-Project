//
// Created by aviv on 16/04/2020.
//
#include "UnitTests.h"
#include "../utils.h"
#include "assertions.h"

#define RESET_STR(name) name[0] = '\0'

TEST(Utils) {
	char *temp;
	/* Parse Symbol */
	temp = parse_symbol(" SYMBOL: ");
	assert_str("SYMBOL", temp, "utils - parse symbol ' SYMBOL: ' from 1", total, failed);
	temp = parse_symbol("LAMBADA:");
	assert_str("LAMBADA", temp, "utils - parse symbol 'LAMBADA:' from 0", total, failed);
	temp = parse_symbol("HULLO");
	assert_null(temp, "utils - parse symbol 'HULLO' from 0", total, failed);
	temp = parse_symbol("GOODBYE");
	assert_null(temp, "utils - parse symbol 'GOODBYE' from 0", total, failed);
	temp = parse_symbol("  \tVALIDLABEL:");
	assert_str("VALIDLABEL", temp, "utils - parse symbol '  \tVALIDLABEL:",total,failed);
	temp = parse_symbol("123INVALID");
	assert_null(temp, "utils - parse symbol '123INVALID'",total, failed);

	/* Write word */

}