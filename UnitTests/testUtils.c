//
// Created by aviv on 16/04/2020.
//
#include "UnitTests.h"
#include "../utils.h"
#include "assertions.h"

#define RESET_STR(name) name[0] = '\0'

TEST(Utils) {
	char *temp;
	char buff[20];

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
	write_word(buff, 0, 0x50,0x50,0x50);
	assert_char(0x50, buff[0], "Write word 0x505050 to buff[0]", total, failed);
	assert_char(0x50, buff[1], "Write word 0x505050 to buff[0]", total, failed);
	assert_char(0x50, buff[2], "Write word 0x505050 to buff[0]", total, failed);
	write_word(buff, 3, 0x60,0x60,0x60);
	assert_char(0x50, buff[0], "Write word 0x505050 to buff[0], 0x606060 to buff[3]", total, failed);
	assert_char(0x50, buff[1], "Write word 0x505050 to buff[0], 0x606060 to buff[3]", total, failed);
	assert_char(0x50, buff[2], "Write word 0x505050 to buff[0], 0x606060 to buff[3]", total, failed);
	assert_char(0x60, buff[3], "Write word 0x505050 to buff[0], 0x606060 to buff[3]", total, failed);
	assert_char(0x60, buff[4], "Write word 0x505050 to buff[0], 0x606060 to buff[3]", total, failed);
	assert_char(0x60, buff[5], "Write word 0x505050 to buff[0], 0x606060 to buff[3]", total, failed);

	/* is_int */
	assert_true(is_int("1"), "is int 1", total, failed);
	assert_true(is_int("12345"), "is int 12345", total, failed);
	assert_true(is_int("5432"), "is int 5432", total, failed);
	assert_true(is_int("9687"), "is int 9687", total, failed);
	assert_true(is_int("4243"), "is int 4243", total, failed);
	assert_false(is_int("101a"), "is int 101a", total, failed);
	assert_false(is_int("a101"), "is int a101", total, failed);
	assert_false(is_int("1.01"), "is int 1.01", total, failed);
	assert_false(is_int(""), "is int ", total, failed);

	/* int to word */
	temp = int_to_word(50);
	assert_char(temp[0], 0, "50 int to 24-bit word [0]",total,failed);
	assert_char(temp[1], 0, "50 int to 24-bit word [1]",total,failed);
	assert_char(temp[2], 50, "50 int to 24-bit word [2]",total,failed);

	temp = int_to_word(0x704376);
	assert_char(temp[0], 0x70, "0x704376 int to 24-bit word [0]",total,failed);
	assert_char(temp[1], 0x43, "0x704376 int to 24-bit word [1]",total,failed);
	assert_char(temp[2], 0x76, "0x704376 int to 24-bit word [2]",total,failed);


	temp = int_to_word(0xFFFFF);
	assert_char(temp[1], 0xFF, "0xFFFFF int to 24-bit word [1]",total,failed);
	assert_char(temp[2], 0xFF, "0xFFFFF int to 24-bit word [2]",total,failed);
}