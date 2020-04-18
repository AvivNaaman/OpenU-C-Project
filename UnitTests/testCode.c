//
// Created by aviv on 18/04/2020.
//
#include "UnitTests.h"
#include "assertions.h"
#include "../code.h"

TEST(Code) {
	CALL_TEST(Code_GetOpCodeFunc, *total, *failed)
	CALL_TEST(Code_GetAddrType, *total, *failed)
	CALL_TEST(Code_GetCodeWord, *total, *failed);
	CALL_TEST(Code_ValidateOp, *total, *failed);

}

TEST(Code_GetOpCodeFunc) {

		opcode opcode_out;
		funct funct_out;
		/* Get opcode */
		get_opcode_func("mov", &opcode_out, &funct_out);
		assert_int(0,opcode_out,"get opcode mov", total, failed);
		get_opcode_func("cmp", &opcode_out, &funct_out);
		assert_int(1,opcode_out,"get opcode cmp", total, failed);
		get_opcode_func("add", &opcode_out, &funct_out);
		assert_int(2,opcode_out,"get opcode add", total, failed);
		assert_int(1,funct_out,"get opcode add", total, failed);
		get_opcode_func("sub", &opcode_out, &funct_out);
		assert_int(2,opcode_out,"get opcode sub", total, failed);
		assert_int(2,funct_out,"get opcode sub", total, failed);
		get_opcode_func("lea", &opcode_out, &funct_out);
		assert_int(4,opcode_out,"get opcode lea", total, failed);

		get_opcode_func("clr", &opcode_out, &funct_out);
		assert_int(5,opcode_out,"get opcode clr", total, failed);
		assert_int(1,funct_out,"get opcode clr", total, failed);
		get_opcode_func("not", &opcode_out, &funct_out);
		assert_int(5,opcode_out,"get opcode not", total, failed);
		assert_int(2,funct_out,"get opcode not", total, failed);
		get_opcode_func("inc", &opcode_out, &funct_out);
		assert_int(5,opcode_out,"get opcode inc", total, failed);
		assert_int(3,funct_out,"get opcode inc", total, failed);
		get_opcode_func("dec", &opcode_out, &funct_out);
		assert_int(5,opcode_out,"get opcode dec", total, failed);
		assert_int(4,funct_out,"get opcode dec", total, failed);
		get_opcode_func("jmp", &opcode_out, &funct_out);
		assert_int(9,opcode_out,"get opcode jmp", total, failed);
		assert_int(1,funct_out,"get opcode jmp", total, failed);
		get_opcode_func("bne", &opcode_out, &funct_out);
		assert_int(9,opcode_out,"get opcode bne", total, failed);
		assert_int(2,funct_out,"get opcode bne", total, failed);
		get_opcode_func("jsr", &opcode_out, &funct_out);
		assert_int(9,opcode_out,"get opcode jsr", total, failed);
		assert_int(3,funct_out,"get opcode jsr", total, failed);
		get_opcode_func("red", &opcode_out, &funct_out);
		assert_int(12,opcode_out,"get opcode red", total, failed);
		get_opcode_func("prn", &opcode_out, &funct_out);
		assert_int(13,opcode_out,"get opcode prn", total, failed);
		get_opcode_func("rts", &opcode_out, &funct_out);
		assert_int(14,opcode_out,"get opcode rts", total, failed);
		get_opcode_func("stop", &opcode_out, &funct_out);
		assert_int(15,opcode_out,"get opcode stop", total, failed);
}

TEST(Code_GetAddrType) {
	assert_int(REGISTER, get_addressing_type("r0"), "get addressing type r0", total, failed);
	assert_int(REGISTER, get_addressing_type("r1"), "get addressing type r1", total, failed);
	assert_int(REGISTER, get_addressing_type("r2"), "get addressing type r2", total, failed);
	assert_int(REGISTER, get_addressing_type("r3"), "get addressing type r3", total, failed);
	assert_int(REGISTER, get_addressing_type("r4"), "get addressing type r4", total, failed);
	assert_int(REGISTER, get_addressing_type("r5"), "get addressing type r5", total, failed);
	assert_int(REGISTER, get_addressing_type("r6"), "get addressing type r6", total, failed);
	assert_int(REGISTER, get_addressing_type("r7"), "get addressing type r7", total, failed);

	assert_int(IMMEDIATE, get_addressing_type("#0"),"get addressing type #0",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#1"),"get addressing type #1",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#236"),"get addressing type #236",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#-1"),"get addressing type #-1",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#-236"),"get addressing type #-236",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#4756"),"get addressing type #4756",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#-0"),"get addressing type #-0",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#+99"),"get addressing type #+0",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#+1"),"get addressing type #+0",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#+0"),"get addressing type #+0",total,failed);
	assert_int(IMMEDIATE, get_addressing_type("#-0"),"get addressing type #-0",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#--1"),"get addressing type #--1",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#.5"),"get addressing type #.5",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#4.8"),"get addressing type #4.8",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#ABC"),"get addressing type #ABC",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#123$"),"get addressing type #123$",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#$123"),"get addressing type #$123",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#$123"),"get addressing type $123",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("#"),"get addressing type #",total,failed);

	assert_int(RELATIVE, get_addressing_type("&labello"),"get addressing type &labello",total,failed);
	assert_int(RELATIVE, get_addressing_type("&hullo123"),"get addressing type &hullo123",total,failed);
	assert_int(RELATIVE, get_addressing_type("&hULLO23"),"get addressing type &hULLO23",total,failed);
	assert_int(RELATIVE, get_addressing_type("&Hullo123"),"get addressing type &Hullo123",total,failed);
	assert_int(RELATIVE, get_addressing_type("&labelllllllllllllllllllllllllll"),"get addressing type &labelllllllllllllllllllllllllll",total,failed);
	assert_int(RELATIVE, get_addressing_type("&R5"),"get addressing type &R5",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("&1label"),"get addressing type &1label",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("&labellllllllllllllllllllllllllll"),"get addressing type &labellllllllllllllllllllllllllll",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("&hul$lo"),"get addressing type &hul$lo",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("&hullo$"),"get addressing type &hullo$",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("&$hullo"),"get addressing type &$hullo",total,failed);
	assert_int(NONE_ADDR, get_addressing_type("&r5"),"get addressing type &r5",total,failed);

	assert_int(DIRECT, get_addressing_type("label"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("labelllllllllllllllllllllllllll"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("hello123"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("he110123"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("he110123"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("HELLO"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("HuLLO"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("H1"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("H"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("R0"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("$label"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("1label"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("labellllllllllllllllllllllllllll"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("lab%l"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("label$"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("H*"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("label:"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type(":label"), "get addressing type label", total, failed);
}

TEST(Code_GetCodeWord) {
	code_word *codeword;
	char **operands;
}


TEST(Code_ValidateOp) {
	assert_true(validate_op_addr(REGISTER, RELATIVE, 4,4,REGISTER,RELATIVE,IMMEDIATE,DIRECT,REGISTER,RELATIVE,IMMEDIATE,DIRECT),"validate operatrion register,relative,4,4",total,failed);
	assert_true(validate_op_addr(REGISTER,NONE_ADDR,4,0,REGISTER,RELATIVE,IMMEDIATE,DIRECT),"validate operation register 4,0,everything", total, failed);
	assert_true(validate_op_addr(REGISTER,NONE_ADDR,4,0,DIRECT,RELATIVE,IMMEDIATE,REGISTER),"validate operation addressing REGISTER TO ALL RESGISTER LAST",total,failed);
	assert_false(validate_op_addr(REGISTER,REGISTER,3,3,DIRECT,RELATIVE,IMMEDIATE,DIRECT,RELATIVE,IMMEDIATE),"validate operation addressing that is not here double REGISTER",total,failed);
	assert_false(validate_op_addr(RELATIVE,NONE_ADDR,2,0,IMMEDIATE,REGISTER),"",total,failed);
	assert_false(validate_op_addr(DIRECT,NONE_ADDR,3,0,IMMEDIATE,REGISTER,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(IMMEDIATE,NONE_ADDR,3,0,REGISTER,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(REGISTER,NONE_ADDR,3,0,IMMEDIATE,DIRECT,RELATIVE),"",total,failed);

	assert_false(validate_op_addr(REGISTER,REGISTER,3,3,IMMEDIATE,DIRECT,RELATIVE,IMMEDIATE,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(REGISTER,DIRECT,3,3,IMMEDIATE,DIRECT,RELATIVE,IMMEDIATE,REGISTER,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(REGISTER,RELATIVE,3,3,IMMEDIATE,DIRECT,RELATIVE,IMMEDIATE,DIRECT,REGISTER),"",total,failed);
	assert_false(validate_op_addr(REGISTER,IMMEDIATE,3,3,IMMEDIATE,DIRECT,RELATIVE,REGISTER,DIRECT,RELATIVE),"",total,failed);

	assert_false(validate_op_addr(DIRECT,REGISTER,3,3,IMMEDIATE,REGISTER,RELATIVE,IMMEDIATE,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(DIRECT,IMMEDIATE,3,3,IMMEDIATE,REGISTER,RELATIVE,REGISTER,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(DIRECT,DIRECT,3,3,IMMEDIATE,REGISTER,RELATIVE,IMMEDIATE,REGISTER,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(DIRECT,RELATIVE,3,3,IMMEDIATE,REGISTER,RELATIVE,IMMEDIATE,DIRECT,REGISTER),"",total,failed);

	assert_false(validate_op_addr(IMMEDIATE,REGISTER,3,3,DIRECT,REGISTER,RELATIVE,IMMEDIATE,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(IMMEDIATE,IMMEDIATE,3,3,DIRECT,REGISTER,RELATIVE,REGISTER,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(IMMEDIATE,DIRECT,3,3,DIRECT,REGISTER,RELATIVE,IMMEDIATE,REGISTER,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(IMMEDIATE,RELATIVE,3,3,DIRECT,REGISTER,RELATIVE,IMMEDIATE,DIRECT,REGISTER),"",total,failed);

	assert_false(validate_op_addr(RELATIVE,REGISTER,3,3,IMMEDIATE,REGISTER,DIRECT,IMMEDIATE,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(RELATIVE,IMMEDIATE,3,3,IMMEDIATE,REGISTER,DIRECT,REGISTER,DIRECT,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(RELATIVE,DIRECT,3,3,IMMEDIATE,REGISTER,DIRECT,IMMEDIATE,REGISTER,RELATIVE),"",total,failed);
	assert_false(validate_op_addr(RELATIVE,RELATIVE,3,3,IMMEDIATE,REGISTER,DIRECT,IMMEDIATE,DIRECT,REGISTER),"",total,failed);

}

