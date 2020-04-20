//
// Created by aviv on 18/04/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include "UnitTests.h"
#include "assertions.h"
#include "../code.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define SEP_COLOR GRN
#define RESET "\x1B[0m"
#define SEPERATOR(color) printf(color "=======================\n" RESET);

TEST(Code) {
	CALL_TEST(Code_GetOpCodeFunc, *total, *failed)
	CALL_TEST(Code_GetAddrType, *total, *failed)
	CALL_TEST(Code_GetCodeWord, *total, *failed);
	CALL_TEST(Code_ValidateOp, *total, *failed);
	CALL_TEST(Code_BuilDataWord, *total, *failed);
	CALL_TEST(Code_Process, *total, *failed);
}

TEST(Code_GetOpCodeFunc) {

	opcode opcode_out;
	funct funct_out;
	/* Get opcode */
	get_opcode_func("mov", &opcode_out, &funct_out);
	assert_int(0, opcode_out, "get opcode mov", total, failed);
	get_opcode_func("cmp", &opcode_out, &funct_out);
	assert_int(1, opcode_out, "get opcode cmp", total, failed);
	get_opcode_func("add", &opcode_out, &funct_out);
	assert_int(2, opcode_out, "get opcode add", total, failed);
	assert_int(1, funct_out, "get opcode add", total, failed);
	get_opcode_func("sub", &opcode_out, &funct_out);
	assert_int(2, opcode_out, "get opcode sub", total, failed);
	assert_int(2, funct_out, "get opcode sub", total, failed);
	get_opcode_func("lea", &opcode_out, &funct_out);
	assert_int(4, opcode_out, "get opcode lea", total, failed);

	get_opcode_func("clr", &opcode_out, &funct_out);
	assert_int(5, opcode_out, "get opcode clr", total, failed);
	assert_int(1, funct_out, "get opcode clr", total, failed);
	get_opcode_func("not", &opcode_out, &funct_out);
	assert_int(5, opcode_out, "get opcode not", total, failed);
	assert_int(2, funct_out, "get opcode not", total, failed);
	get_opcode_func("inc", &opcode_out, &funct_out);
	assert_int(5, opcode_out, "get opcode inc", total, failed);
	assert_int(3, funct_out, "get opcode inc", total, failed);
	get_opcode_func("dec", &opcode_out, &funct_out);
	assert_int(5, opcode_out, "get opcode dec", total, failed);
	assert_int(4, funct_out, "get opcode dec", total, failed);
	get_opcode_func("jmp", &opcode_out, &funct_out);
	assert_int(9, opcode_out, "get opcode jmp", total, failed);
	assert_int(1, funct_out, "get opcode jmp", total, failed);
	get_opcode_func("bne", &opcode_out, &funct_out);
	assert_int(9, opcode_out, "get opcode bne", total, failed);
	assert_int(2, funct_out, "get opcode bne", total, failed);
	get_opcode_func("jsr", &opcode_out, &funct_out);
	assert_int(9, opcode_out, "get opcode jsr", total, failed);
	assert_int(3, funct_out, "get opcode jsr", total, failed);
	get_opcode_func("red", &opcode_out, &funct_out);
	assert_int(12, opcode_out, "get opcode red", total, failed);
	get_opcode_func("prn", &opcode_out, &funct_out);
	assert_int(13, opcode_out, "get opcode prn", total, failed);
	get_opcode_func("rts", &opcode_out, &funct_out);
	assert_int(14, opcode_out, "get opcode rts", total, failed);
	get_opcode_func("stop", &opcode_out, &funct_out);
	assert_int(15, opcode_out, "get opcode stop", total, failed);
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

	assert_int(IMMEDIATE, get_addressing_type("#0"), "get addressing type #0", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#1"), "get addressing type #1", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#236"), "get addressing type #236", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#-1"), "get addressing type #-1", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#-236"), "get addressing type #-236", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#4756"), "get addressing type #4756", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#-0"), "get addressing type #-0", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#+99"), "get addressing type #+0", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#+1"), "get addressing type #+0", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#+0"), "get addressing type #+0", total, failed);
	assert_int(IMMEDIATE, get_addressing_type("#-0"), "get addressing type #-0", total, failed);
	SEPERATOR(SEP_COLOR)
	assert_int(NONE_ADDR, get_addressing_type("#--1"), "get addressing type #--1", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#.5"), "get addressing type #.5", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#4.8"), "get addressing type #4.8", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#ABC"), "get addressing type #ABC", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#123$"), "get addressing type #123$", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#$123"), "get addressing type #$123", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#$123"), "get addressing type $123", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("#"), "get addressing type #", total, failed);
	SEPERATOR(SEP_COLOR)
	assert_int(RELATIVE, get_addressing_type("&labello"), "get addressing type &labello", total, failed);
	assert_int(RELATIVE, get_addressing_type("&hullo123"), "get addressing type &hullo123", total, failed);
	assert_int(RELATIVE, get_addressing_type("&hULLO23"), "get addressing type &hULLO23", total, failed);
	assert_int(RELATIVE, get_addressing_type("&Hullo123"), "get addressing type &Hullo123", total, failed);
	assert_int(RELATIVE, get_addressing_type("&labelllllllllllllllllllllllllll"),
	           "get addressing type &labelllllllllllllllllllllllllll", total, failed);
	assert_int(RELATIVE, get_addressing_type("&R5"), "get addressing type &R5", total, failed);
	SEPERATOR(SEP_COLOR)
	assert_int(NONE_ADDR, get_addressing_type("&1label"), "get addressing type &1label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("&labellllllllllllllllllllllllllll"),
	           "get addressing type &labellllllllllllllllllllllllllll", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("&hul$lo"), "get addressing type &hul$lo", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("&hullo$"), "get addressing type &hullo$", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("&$hullo"), "get addressing type &$hullo", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("&r5"), "get addressing type &r5", total, failed);
	SEPERATOR(SEP_COLOR)
	assert_int(DIRECT, get_addressing_type("label"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("labelllllllllllllllllllllllllll"), "get addressing type label", total,
	           failed);
	assert_int(DIRECT, get_addressing_type("hello123"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("he110123"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("he110123"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("HELLO"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("HuLLO"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("H1"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("H"), "get addressing type label", total, failed);
	assert_int(DIRECT, get_addressing_type("R0"), "get addressing type label", total, failed);
	SEPERATOR(SEP_COLOR)
	assert_int(NONE_ADDR, get_addressing_type("$label"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("1label"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("labellllllllllllllllllllllllllll"), "get addressing type label", total,
	           failed);
	assert_int(NONE_ADDR, get_addressing_type("lab%l"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("label$"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("H*"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type("label:"), "get addressing type label", total, failed);
	assert_int(NONE_ADDR, get_addressing_type(":label"), "get addressing type label", total, failed);
	SEPERATOR(SEP_COLOR)
}

char *get_word(int *arr, int indx) {
	if (arr == NULL) return NULL;
	char *ptr = calloc(3, sizeof(char));
	unsigned int val = arr[indx];
	*ptr = (val & (0xFF << 16)) >> 16;
	*(ptr + 1) = (val & (0xFF << 8)) >> 8;
	*(ptr + 2) = val & (0xFF);
	return ptr;
}

TEST(Code_GetCodeWord) {
	int *codeword;
	int finals[21] = {
			0b000000110011101000000100,
			0b000000000000100000000100,
			0b000001000001111000000100,
			0b000001010000000000000100,
			0b000010111001101000010100,
			0b000010000001111100010100,
			0b000010111010100000001100,
			0b000010010000100000001100,
			0b000100010001101100000100,
			0b000101000000100000010100,
			0b000101000001111100001100,
			0b000101000001111000011100,
			0b000101000000100000100100,
			0b001001000001000000001100,
			0b001001000000100000010100,
			0b001001000000100000011100,
			0b001001000001000000011100,
			0b001100000001101100000100,
			0b001101000000100000000100,
			0b001110000000000000000100,
			0b001111000000000000000100,
	};
	/* mov r1,r2 */
	char *(operands[2]) = {"r1", "r2"};
	codeword = (int *) get_code_word(MOV_OP, NONE_FUNCT, 2,  operands);
	assert_word(get_word(codeword, 0), get_word(finals, 0), "MOV r1,r2 get code word", total, failed);
	/* mov #5, label */
	operands[0] = "#5";
	operands[1] = "label";
	codeword = (int *) get_code_word(MOV_OP, NONE_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 1), "mov #5, label get code word", total, failed);
	/* cmp #-8,r6 */
	operands[0] = "#-8";
	operands[1] = "r6";
	codeword = (int *) get_code_word(CMP_OP, NONE_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 2), "cmp #-8, r6 get code word", total, failed);
	/* cmp label, #+2 */
	operands[0] = "label";
	operands[1] = "#+2";
	codeword = (int *) get_code_word(CMP_OP, NONE_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 3), "cmp label, #+2 get code word", total, failed);
	/* sub r4,r2 */
	operands[0] = "r4";
	operands[1] = "r2";
	codeword = (int *) get_code_word(SUB_OP, SUB_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 4), "sub r4, r2 get code word", total, failed);
	/* sub #45,r7 */
	operands[0] = "#45";
	operands[1] = "r7";
	codeword = (int *) get_code_word(SUB_OP, SUB_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 5), "sub #45, r7 get code word", total, failed);
	/* add r5,label */
	operands[0] = "r5";
	operands[1] = "label";
	codeword = (int *) get_code_word(ADD_OP, ADD_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 6), "add r5, label get code word", total, failed);
	/* add label,label */
	operands[0] = "label";
	operands[1] = "label";
	codeword = (int *) get_code_word(ADD_OP, ADD_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 7), "add label, label get code word", total, failed);
	/* lea label,r3 */
	operands[0] = "label";
	operands[1] = "r3";
	codeword = (int *) get_code_word(LEA_OP, NONE_FUNCT, 2, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 8), "lea label,r3 get code word", total, failed);
	operands[1] = "";

	/* not label */
	operands[0] = "label";
	codeword = (int *) get_code_word(NOT_OP, NOT_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 9), "not label get code word", total, failed);
	/* clr r7 */
	operands[0] = "r7";
	codeword = (int *) get_code_word(CLR_OP, CLR_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 10), "clr r7 get code word", total, failed);
	/* inc r6 */
	operands[0] = "r6";
	codeword = (int *) get_code_word(INC_OP, INC_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 11), "inc r6 get code word", total, failed);
	/* dec label */
	operands[0] = "label";
	codeword = (int *) get_code_word(DEC_OP, DEC_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 12), "dec label get code word", total, failed);
	/* jmp &label */
	operands[0] = "&label";
	codeword = (int *) get_code_word(JMP_OP, JMP_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 13), "jmp &label get code word", total, failed);
	/* bne label */
	operands[0] = "label";
	codeword = (int *) get_code_word(BNE_OP, BNE_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 14), "bne label get code word", total, failed);
	/* jsr label */
	operands[0] = "label";
	codeword = (int *) get_code_word(JSR_OP, JSR_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 15), "jsr label get code word", total, failed);
	/* jsr &label */
	operands[0] = "&label";
	codeword = (int *) get_code_word(JSR_OP, JSR_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 16), "jsr &label get code word", total, failed);
	/* red r3 */
	operands[0] = "r3";
	codeword = (int *) get_code_word(RED_OP, NONE_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 17), "red r3 get code word", total, failed);
	/* prn &label */
	operands[0] = "label";
	codeword = (int *) get_code_word(PRN_OP, NONE_FUNCT, 1, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 18), "prn &label get code word", total, failed);

	operands[0] = "";
	codeword = (int *) get_code_word(RTS_OP, NONE_FUNCT, 0, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 19), "rts get code word", total, failed);
	codeword = (int *) get_code_word(STOP_OP, NONE_FUNCT, 0, (char **) (operands));
	assert_word(get_word(codeword, 0), get_word(finals, 20), "stop get code word", total, failed);
}

TEST(Code_BuilDataWord) {
	int *dw;
	int finals[20] = {
			0b0100,
			0b1100,
			0b111111111111111111111001,
			0b111111111111111111001100,
			0b0111000100,
			0b011111111111111111111100,
			0b100000000000000000001100,
			0b011111111111111111111001,
			0b100000000000000000001001
	};
	assert_word(get_word((int *) build_data_word(IMMEDIATE, 0), 0), get_word(finals, 0),
	            "build data word of 0 Immediate addressing", total, failed);
	assert_word(get_word((int *) build_data_word(RELATIVE, 1), 0), get_word(finals, 1),
	            "build data word of 1 Relative addressing", total, failed);
	assert_word(get_word((int *) build_data_word(DIRECT, -1), 0), get_word(finals, 2),
	            "build data word of -1 Direct addressing", total, failed);

	assert_word(get_word((int *) build_data_word(IMMEDIATE, -7), 0), get_word(finals, 3),
	            "build data word of -7 Immediate addressing", total, failed);
	assert_word(get_word((int *) build_data_word(IMMEDIATE, 56), 0), get_word(finals, 4),
	            "build data word of 56 Immediate addressing", total, failed);
	assert_word(get_word((int *) build_data_word(IMMEDIATE, 1048575), 0), get_word(finals, 5),
	            "build data word of 1048575 Immediate addressing", total, failed);
	assert_word(get_word((int *) build_data_word(IMMEDIATE, -1048575), 0), get_word(finals, 6),
	            "build data word of -1048575 Immediate addressing", total, failed);
	assert_word(get_word((int *) build_data_word(DIRECT, 1048575), 0), get_word(finals, 7),
	            "build data word of 1048575 Immediate addressing", total, failed);
	assert_word(get_word((int *) build_data_word(DIRECT, -1048575), 0), get_word(finals, 8),
	            "build data word of -1048575 Immediate addressing", total, failed);
}

TEST(Code_ValidateOp) {
	assert_true(validate_op_addr(REGISTER, RELATIVE, 4, 4, REGISTER, RELATIVE, IMMEDIATE, DIRECT, REGISTER, RELATIVE,
	                             IMMEDIATE, DIRECT), "validate operatrion register,relative,4,4", total, failed);
	assert_true(validate_op_addr(REGISTER, NONE_ADDR, 4, 0, REGISTER, RELATIVE, IMMEDIATE, DIRECT),
	            "validate operation register 4,0,everything", total, failed);
	assert_true(validate_op_addr(REGISTER, NONE_ADDR, 4, 0, DIRECT, RELATIVE, IMMEDIATE, REGISTER),
	            "validate operation addressing REGISTER TO ALL RESGISTER LAST x2", total, failed);
	assert_true(validate_op_addr(REGISTER, REGISTER, 4, 4, DIRECT, RELATIVE, IMMEDIATE, REGISTER, DIRECT, RELATIVE,
	                             IMMEDIATE, REGISTER), "validate operation addressing REGISTER TO ALL RESGISTER LAST",
	            total, failed);
	assert_true(validate_op_addr(REGISTER, REGISTER, 1, 1, REGISTER, REGISTER),
	            "validate operation addressing REGISTER REGISTER to REGISTER REGISTER", total, failed);

	assert_false(validate_op_addr(REGISTER, REGISTER, 3, 3, DIRECT, RELATIVE, IMMEDIATE, DIRECT, RELATIVE, IMMEDIATE),
	             "validate operation addressing that is not here double REGISTER", total, failed);
	SEPERATOR(SEP_COLOR)
	assert_false(validate_op_addr(RELATIVE, NONE_ADDR, 2, 0, IMMEDIATE, REGISTER), "", total, failed);
	assert_false(validate_op_addr(DIRECT, NONE_ADDR, 3, 0, IMMEDIATE, REGISTER, RELATIVE), "", total, failed);
	assert_false(validate_op_addr(IMMEDIATE, NONE_ADDR, 3, 0, REGISTER, DIRECT, RELATIVE), "", total, failed);
	assert_false(validate_op_addr(REGISTER, NONE_ADDR, 3, 0, IMMEDIATE, DIRECT, RELATIVE), "", total, failed);

	assert_false(validate_op_addr(REGISTER, REGISTER, 3, 3, IMMEDIATE, DIRECT, RELATIVE, IMMEDIATE, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(REGISTER, DIRECT, 3, 3, IMMEDIATE, DIRECT, RELATIVE, IMMEDIATE, REGISTER, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(REGISTER, RELATIVE, 3, 3, IMMEDIATE, DIRECT, RELATIVE, IMMEDIATE, DIRECT, REGISTER),
	             "", total, failed);
	assert_false(validate_op_addr(REGISTER, IMMEDIATE, 3, 3, IMMEDIATE, DIRECT, RELATIVE, REGISTER, DIRECT, RELATIVE),
	             "", total, failed);

	assert_false(validate_op_addr(DIRECT, REGISTER, 3, 3, IMMEDIATE, REGISTER, RELATIVE, IMMEDIATE, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(DIRECT, IMMEDIATE, 3, 3, IMMEDIATE, REGISTER, RELATIVE, REGISTER, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(DIRECT, DIRECT, 3, 3, IMMEDIATE, REGISTER, RELATIVE, IMMEDIATE, REGISTER, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(DIRECT, RELATIVE, 3, 3, IMMEDIATE, REGISTER, RELATIVE, IMMEDIATE, DIRECT, REGISTER),
	             "", total, failed);

	assert_false(validate_op_addr(IMMEDIATE, REGISTER, 3, 3, DIRECT, REGISTER, RELATIVE, IMMEDIATE, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(IMMEDIATE, IMMEDIATE, 3, 3, DIRECT, REGISTER, RELATIVE, REGISTER, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(IMMEDIATE, DIRECT, 3, 3, DIRECT, REGISTER, RELATIVE, IMMEDIATE, REGISTER, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(IMMEDIATE, RELATIVE, 3, 3, DIRECT, REGISTER, RELATIVE, IMMEDIATE, DIRECT, REGISTER),
	             "", total, failed);

	assert_false(validate_op_addr(RELATIVE, REGISTER, 3, 3, IMMEDIATE, REGISTER, DIRECT, IMMEDIATE, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(RELATIVE, IMMEDIATE, 3, 3, IMMEDIATE, REGISTER, DIRECT, REGISTER, DIRECT, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(RELATIVE, DIRECT, 3, 3, IMMEDIATE, REGISTER, DIRECT, IMMEDIATE, REGISTER, RELATIVE),
	             "", total, failed);
	assert_false(validate_op_addr(RELATIVE, RELATIVE, 3, 3, IMMEDIATE, REGISTER, DIRECT, IMMEDIATE, DIRECT, REGISTER),
	             "", total, failed);
	SEPERATOR(SEP_COLOR)
}

TEST(Code_Process) {
	char codeImg[500];
	int ci = 100;
	int finals[30] = {
			0b000000110011101000000100,
			0b000000000001110100000100,
			0b111111111111111001001100,
			0b000000110000100000000100,
			0b000001110001100100000100,
			0b000001000000000000000100,
			0b000000000000000000101100,
			0b000000000000000001000100,
			0b000001000000100000000100,
			0b000000000000000001111100,
			0b000001010000100000000100,
			0b000010000000100000001100,
			0b000000000000010001010100,
			0b000010000000100000010100,
			0b111111111111111111111100,
			0b000010110001100000010100,

			0b000100010001111100000100,
			0b000101000001100000001100,
			0b000101000001111000010100,
			0b000101000000100000011100,
			0b000101000000100000100100,
			0b001001000001000000001100,
			0b001001000000100000010100,
			0b001001000001000000011100,
			0b001100000001110100000100,
			0b001101000001110000000100,
			0b001101000000000000000100,
			0b001110000000000000000100,
			0b001111000000000000000100,
	};
	assert_false(process_code("\t mov \t r1\t , \tr2", 0, &ci, codeImg), "mov r1,r2", total, failed);
	assert_int(101,ci,"mov r1,r2 ci",total,failed);
	assert_word(get_word(finals,0),&codeImg[(ci-1)*3],"mov r1,r2 machine code",total,failed);

	assert_false(process_code("\t mov \t #-55\t , \tr5", 0, &ci, codeImg), "mov #-55,r5", total, failed);
	assert_int(103,ci,"mov r1,r2 ci",total,failed);
	assert_word(get_word(finals,1),&codeImg[(ci-2)*3],"mov r1,r2 machine code",total,failed);
	assert_word(get_word(finals,2),&codeImg[(ci-1)*3],"mov r1,r2 machine code data",total,failed);


	assert_false(process_code("mov r0,label", 0, &ci, codeImg), "mov r0,label", total, failed);
	assert_int(105,ci,"mov r0,label ci",total,failed);
	assert_word(get_word(finals,3),&codeImg[(ci-2)*3],"mov r0,label machine code",total,failed);

	assert_false(process_code("cmp r0,r1",0,&ci,codeImg),"cmp r0,r1",total,failed);
	assert_int(106,ci,"cmp r0,r1 ci",total,failed);
	assert_word(get_word(finals,4),&codeImg[(ci-1)*3],"cmp r0,r1 machine code",total,failed);

	assert_false(process_code("cmp #5,#8",0,&ci,codeImg),"cmp #5,#8",total,failed);
	assert_int(109,ci,"cmp #5,#8 ci",total,failed);
	assert_word(get_word(finals,5),&codeImg[(ci-3)*3],"cmp #5,#8 machine code",total,failed);
	assert_word(get_word(finals,6),&codeImg[(ci-2)*3],"cmp #5,#8 machine code data",total,failed);
	assert_word(get_word(finals,7),&codeImg[(ci-1)*3],"cmp #5,#8 machine code data",total,failed);

	assert_false(process_code("cmp #+15,label",0,&ci,codeImg),"cmp #+15,label",total,failed);
	assert_int(112,ci,"cmp #+15,label ci",total,failed);
	assert_word(get_word(finals,8),&codeImg[(ci-3)*3],"cmp #+15,label machine code",total,failed);
	assert_word(get_word(finals,9),&codeImg[(ci-2)*3],"cmp #+15,label machine code data",total,failed);


	assert_false(process_code("LABEL0: cmp label,label",7,&ci,codeImg),"LABEL0: cmp label,label",total,failed);
	assert_int(115,ci,"LABEL0: cmp label,label ci",total,failed);
	assert_word(get_word(finals,10),&codeImg[(ci-3)*3],"LABEL0: cmp label,label machine code",total,failed);

	assert_false(process_code("add #138,label",0,&ci,codeImg),"add #138,label",total,failed);
	assert_int(118,ci,"add #138,label ci",total,failed);
	assert_word(get_word(finals,11),&codeImg[(ci-3)*3],"add #138,label machine code",total,failed);
	assert_word(get_word(finals,12),&codeImg[(ci-2)*3],"add #138,label machine code data",total,failed);

	assert_false(process_code("sub #-1,label",0,&ci,codeImg),"sub #-1,label",total,failed);
	assert_int(121,ci,"sub #-1,label ci",total,failed);
	assert_word(get_word(finals,13),&codeImg[(ci-3)*3],"sub #-1,label machine code",total,failed);
	assert_word(get_word(finals,14),&codeImg[(ci-2)*3],"sub #-1,label machine code data",total,failed);


	assert_false(process_code("sub r0,r0",0,&ci,codeImg),"sub r0,r0",total,failed);
	assert_int(122,ci,"sub r0,r0 ci",total,failed);
	assert_word(get_word(finals,15),&codeImg[(ci-1)*3],"sub r0,r0 machine code",total,failed);

	assert_false(process_code("lea label,r7",0,&ci,codeImg),"lea label,r7",total,failed);
	assert_int(124,ci,"lea label,r7",total,failed);
	assert_word(get_word(finals,16),&codeImg[(ci-2)*3],"lea label,r7",total,failed);

	assert_false(process_code("clr r0",0,&ci,codeImg),"clr r0",total,failed);
	assert_int(125,ci,"clr r0",total,failed);
	assert_word(get_word(finals,17),&codeImg[(ci-1)*3],"clr r0",total,failed);

	assert_false(process_code("not r6",0,&ci,codeImg),"not r6",total,failed);
	assert_int(126,ci,"not r6",total,failed);
	assert_word(get_word(finals,18),&codeImg[(ci-1)*3],"not r6",total,failed);

	assert_false(process_code("inc label",0,&ci,codeImg),"inc label",total,failed);
	assert_int(128,ci,"inc label",total,failed);
	assert_word(get_word(finals,19),&codeImg[(ci-2)*3],"inc label",total,failed);

	assert_false(process_code("dec label",0,&ci,codeImg),"dec label",total,failed);
	assert_int(130,ci,"dec label",total,failed);
	assert_word(get_word(finals,20),&codeImg[(ci-2)*3],"dec label",total,failed);

	assert_false(process_code("jmp &label",0,&ci,codeImg),"jmp &label",total,failed);
	assert_int(132,ci,"jmp &label",total,failed);
	assert_word(get_word(finals,21),&codeImg[(ci-2)*3],"jmp &label",total,failed);

	assert_false(process_code("bne label",0,&ci,codeImg),"bne label",total,failed);
	assert_int(134,ci,"bne label",total,failed);
	assert_word(get_word(finals,22),&codeImg[(ci-2)*3],"bne label",total,failed);

	assert_false(process_code("jsr &label",0,&ci,codeImg),"jsr &label",total,failed);
	assert_int(136,ci,"jsr &label",total,failed);
	assert_word(get_word(finals,23),&codeImg[(ci-2)*3],"jsr &label",total,failed);

	assert_false(process_code("red r5",0,&ci,codeImg),"red r5",total,failed);
	assert_int(137,ci,"red r5",total,failed);
	assert_word(get_word(finals,24),&codeImg[(ci-1)*3],"red r5",total,failed);

	assert_false(process_code("prn r4",0,&ci,codeImg),"prn r4",total,failed);
	assert_int(138,ci,"prn r4",total,failed);
	assert_word(get_word(finals,25),&codeImg[(ci-1)*3],"prn r4",total,failed);

	assert_false(process_code("prn #5",0,&ci,codeImg),"prn #5",total,failed);
	assert_int(140,ci,"prn #5",total,failed);
	assert_word(get_word(finals,26),&codeImg[(ci-2)*3],"prn #5",total,failed);

	assert_false(process_code("rts",0,&ci,codeImg),"rts",total,failed);
	assert_int(141,ci,"rts",total,failed);
	assert_word(get_word(finals,27),&codeImg[(ci-1)*3],"rts",total,failed);

	assert_false(process_code("stop",0,&ci,codeImg),"stop",total,failed);
	assert_int(142,ci,"stop",total,failed);
	assert_word(get_word(finals,28),&codeImg[(ci-1)*3],"stop",total,failed);
	/* Invalids */
	{
		int curr_ci = ci;
		SEPERATOR(SEP_COLOR)
		assert_true(process_code("stop #5", 0, &ci, codeImg), "stop #5", total, failed); /* fails */
		assert_true(process_code("jsr #5", 0, &ci, codeImg), "jsr #5", total, failed);
		/* Invalid addressing or operand count */
		assert_true(process_code("mov r0,#5", 0, &ci, codeImg), "mov r0,#5", total, failed);
		assert_true(process_code("mov r0,&label", 0, &ci, codeImg), "mov r0,&label", total, failed);
		assert_true(process_code("cmp r0,&label", 0, &ci, codeImg), "cmp r0,&label", total, failed);
		assert_true(process_code("cmp &label,r0", 0, &ci, codeImg), "cmp &label,r0", total, failed);
		assert_true(process_code("cmp &label,&label", 0, &ci, codeImg), "cmp &label,&label", total, failed);
		assert_true(process_code("add &label,r0", 0, &ci, codeImg), "add &label,r0", total, failed);
		assert_true(process_code("add r0, &label", 0, &ci, codeImg), "add r0, &label", total, failed);
		assert_true(process_code("add r0, #6", 0, &ci, codeImg), "add r0, #6", total, failed);
		assert_true(process_code("sub &label,r0", 0, &ci, codeImg), "sub &label,r0", total, failed);
		assert_true(process_code("sub r0, &label", 0, &ci, codeImg), "sub r0, &label", total, failed);
		assert_true(process_code("sub r0, #6", 0, &ci, codeImg), "sub r0, #6", total, failed);
		assert_true(process_code("lea #5,&label", 0, &ci, codeImg), "lea #5,&label", total, failed);
		assert_true(process_code("lea #5, #7", 0, &ci, codeImg), "lea #5, #7", total, failed);
		assert_true(process_code("lea #7, r0", 0, &ci, codeImg), "lea #7, r0", total, failed);
		assert_true(process_code("lea label, #55", 0, &ci, codeImg), "lea label, #55", total, failed);
		assert_true(process_code("lea label, &label", 0, &ci, codeImg), "lea label, &label", total, failed);
		assert_true(process_code("clr #5, label", 0, &ci, codeImg), "clr #5, label", total, failed);
		assert_true(process_code("clr #5", 0, &ci, codeImg), "clr #5", total, failed);
		assert_true(process_code("clr &label", 0, &ci, codeImg), "clr &label", total, failed);
		assert_true(process_code("not #5, label", 0, &ci, codeImg), "not #5, label", total, failed);
		assert_true(process_code("not #5", 0, &ci, codeImg), "not #5", total, failed);
		assert_true(process_code("not &label", 0, &ci, codeImg), "not &label", total, failed);
		assert_true(process_code("inc #5, label", 0, &ci, codeImg), "inc #5, label", total, failed);
		assert_true(process_code("inc #5", 0, &ci, codeImg), "inc #5", total, failed);
		assert_true(process_code("inc &label", 0, &ci, codeImg), "inc &label", total, failed);
		assert_true(process_code("dec #5, label", 0, &ci, codeImg), "dec #5, label", total, failed);
		assert_true(process_code("dec #5", 0, &ci, codeImg), "dec #5", total, failed);
		assert_true(process_code("dec &label", 0, &ci, codeImg), "dec &label", total, failed);
		assert_true(process_code("red #5, label", 0, &ci, codeImg), "red #5, label", total, failed);
		assert_true(process_code("red #5", 0, &ci, codeImg), "red #5", total, failed);
		assert_true(process_code("red &label", 0, &ci, codeImg), "red &label", total, failed);
		assert_true(process_code("jmp r0", 0, &ci, codeImg), "jmp r0", total, failed);
		assert_true(process_code("jmp #7", 0, &ci, codeImg), "jmp #7", total, failed);
		assert_true(process_code("jmp &label, r0", 0, &ci, codeImg), "jmp &label, r0", total, failed);
		assert_true(process_code("bne r0", 0, &ci, codeImg), "bne r0", total, failed);
		assert_true(process_code("bne #7", 0, &ci, codeImg), "bne #7", total, failed);
		assert_true(process_code("bne &label, r0", 0, &ci, codeImg), "bne &label, r0", total, failed);
		assert_true(process_code("jsr r0", 0, &ci, codeImg), "jsr r0", total, failed);
		assert_true(process_code("jsr #7", 0, &ci, codeImg), "jsr #7", total, failed);
		assert_true(process_code("jsr &label, r0", 0, &ci, codeImg), "jsr &label, r0", total, failed);
		assert_true(process_code("prn r0,r1", 0, &ci, codeImg), "prn r0,r1", total, failed);
		assert_true(process_code("prn #9,#10", 0, &ci, codeImg), "prn #9,#10", total, failed);
		assert_true(process_code("prn &label", 0, &ci, codeImg), "prn &label", total, failed);
		assert_true(process_code("prn #99,&label", 0, &ci, codeImg), "prn #99,&label", total, failed);
		assert_true(process_code("rts label", 0, &ci, codeImg), "rts label", total, failed);
		assert_true(process_code("rts r0", 0, &ci, codeImg), "rts r0", total, failed);
		assert_true(process_code("rts &label", 0, &ci, codeImg), "rts &label", total, failed);
		assert_true(process_code("rts #3", 0, &ci, codeImg), "rts #3", total, failed);
		assert_true(process_code("stop label", 0, &ci, codeImg), "stop label", total, failed);
		assert_true(process_code("stop r0", 0, &ci, codeImg), "stop r0", total, failed);
		assert_true(process_code("stop &label", 0, &ci, codeImg), "stop &label", total, failed);
		assert_true(process_code("stop #3", 0, &ci, codeImg), "stop #3", total, failed);
		/* Invalid syntax */
		assert_true(process_code("mov ,r0", 0, &ci, codeImg), "mov ,r0", total, failed);
		assert_true(process_code("mov ,r0,r0", 0, &ci, codeImg), "mov ,r0,r0", total, failed);
		assert_true(process_code("mov ,r0,r0,", 0, &ci, codeImg), "mov ,r0,r0,", total, failed);
		assert_true(process_code("mov r0,r0,", 0, &ci, codeImg), "mov r0,r0,", total, failed);
		assert_true(process_code("mov r0,,", 0, &ci, codeImg), "mov r0,,", total, failed);
		assert_true(process_code("mov r0, \t    \t   , \t    ", 0, &ci, codeImg), "mov r0,  \t ,", total, failed);
		assert_true(process_code("mov r0                r1", 0, &ci, codeImg), "mov r0          r1", total, failed);
		assert_true(process_code("mov r0r1", 0, &ci, codeImg), "mov r0r1", total, failed);
		assert_true(process_code("", 0, &ci, codeImg), "", total, failed);
		assert_true(process_code("clr r 0", 0, &ci, codeImg), "clr r 0", total, failed);
		assert_true(process_code("clr r         0", 0, &ci, codeImg), "clr r    0", total, failed);
		assert_true(process_code("clr ,r0", 0, &ci, codeImg), "clr ,r0", total, failed);
		assert_true(process_code("clr r0,", 0, &ci, codeImg), "clr r0,", total, failed);
		assert_true(process_code("clr ,r0,", 0, &ci, codeImg), "clr ,r0,", total, failed);
		assert_true(process_code("clr r0,,", 0, &ci, codeImg), "clr r0,,", total, failed);
		SEPERATOR(SEP_COLOR)
		assert_int(curr_ci, ci, "2 invalid operations",total, failed);
	}
}
