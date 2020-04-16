//
// Created by aviv on 16/04/2020.
//
#include <stdlib.h>
#include "assertions.h"
#include "UnitTests.h"
#include "../instructions.h"

char *build_string_from_memory(char data_img[], int dc, int length);

TEST(Instructions) {
	instruction_type dat, ext, ent, str, non;
	dat = DATA;
	ext = EXTERN;
	ent = ENTRY;
	str = STRING;
	non = NONE;
	char data_img[250];
	char *temp_str;
	int temp, i, j;
	/* find instruction */
	assert_int(dat, find_instruction_from_index("LABEL: .data", 6), "find instruction LABEL: .data", total, failed);
	assert_int(ext, find_instruction_from_index("LABEL: .extern", 6), "find instruction LABEL: .extern", total, failed);
	assert_int(str, find_instruction_from_index("LABEL: .string", 6), "find instruction LABEL: .string", total, failed);
	assert_int(ent, find_instruction_from_index("LABEL: .entry", 6), "find instruction LABEL: .entry", total, failed);
	assert_int(dat, find_instruction_from_index("  .data  ", 2), "find instruction   .data  ", total, failed);
	assert_int(non, find_instruction_from_index("data", 0), "find instruction data", total, failed);
	assert_int(non, find_instruction_from_index(". data", 0), "find instruction . data", total, failed);
	assert_int(non, find_instruction_from_index(".dat a", 0), "find instruction .dat a", total, failed);
	assert_int(non, find_instruction_from_index(".dataa", 0), "find instruction .dat a", total, failed);
	assert_int(non, find_instruction_from_index("..data", 0), "find instruction .dat a", total, failed);

	/* process .string instruction */
	temp = process_string_instruction("MYDATA: .string \"Hullo, World!\"", 16, data_img, 12);
	assert_int(13, temp, "process .string instruction count for \"Hullo, World!\"", total, failed);
	assert_str("Hullo, World!", build_string_from_memory(data_img, 12, temp), "process .string instruction split succeeded for Hullo, World!",total,failed);

	temp = process_string_instruction(".string \"Nice Tests, Indeed. I like good unit testing!\"", 8, data_img, 2);
	assert_int(45, temp, "process .string instruction count for \"Nice Tests, Indeed. I like good unit testing!\"", total, failed);
	assert_str("Nice Tests, Indeed. I like good unit testing!", build_string_from_memory(data_img, 2, temp), "process .string instruction split succeeded for Hullo, World!",total,failed);

	/* process .data instruction */
	temp = process_data_instruction("LABEL: .data 0,-5,2,76234", 12, data_img, 4); // write 0,-5,2,76 across 4 words of 24bit and return 4:
	assert_int(4, temp, "process .data instruction count for \"LABEL: .data 0,-5,2,76\"", total, failed);
	assert_int(0, build_int_from_memory(data_img, 4), "process .data instruction value for \"LABEL: .data 0,-5,2,76\"", total, failed);
	assert_int(-5, build_int_from_memory(data_img, 5), "process .data instruction value for \"LABEL: .data 0,-5,2,76\"", total, failed);
	assert_int(2, build_int_from_memory(data_img, 6), "process .data instruction value for \"LABEL: .data 0,-5,2,76\"", total, failed);
	assert_int(76234, build_int_from_memory(data_img, 7), "process .data instruction value for \"LABEL: .data 0,-5,2,76\"", total, failed);
}

char *build_string_from_memory(char data_img[], int dc, int length) {
	int i,j;
	dc *= 3;
	length *= 3;
	i = j = 0;
	char *str = (char *)malloc(length+1);
	for (i = dc, j=0; i < length + dc; i += 3,j++) {
		str[j] = data_img[i+2];
	}
	str[j] = '\0'; /* EOS */
	return str;
}
/* TODO: Check why doesn't work */
int build_int_from_memory(char data_img[], int dc) {
	unsigned char *_data_img;
	int to_return = 0;
	_data_img = data_img;
	dc *= 3;
	to_return |= (_data_img[dc] << 16);
	to_return |= (_data_img[dc+1] << 8);
	to_return |= (_data_img[dc+2]);
}