//
// Created by aviv on 16/04/2020.
//
#include <stdio.h>
#include "assertions.h"
#include <string.h>

int assert_true(int value, const char *description, int *total, int *failed) {
	(*total)++;
	if (!value) {
		printf("\nASSERTION FAILED! on %s. Expected TRUE but got FALSE.", description);
		(*failed)++;
	}
}
int assert_false(int value, const char *description, int *total, int *failed){
	(*total)++;
	if (value) {
		printf("\nASSERTION FAILED! on %s. Expected FALSE but got TRUE.", description);
		(*failed)++;
	}
}
int assert_int(int expected, int actual, const char *description, int *total, int *failed){
	(*total)++;
	if (expected != actual) {
		printf("\nASSERTION FAILED! on %s. Expected %d but got %d.", description, expected, actual);
		(*failed)++;
	}
}
int assert_double(double expected, double actual, const char *description, int *total, int *failed){
	(*total)++;
	if (expected != actual) {
		printf("\nASSERTION FAILED! on %s. Expected %f but got %f.", description, expected, actual);;
		(*failed)++;
	}
}
int assert_char(char expected, char actual, const char *description, int *total, int *failed){
	(*total)++;
	if (expected != actual) {
		printf("\nASSERTION FAILED! on %s. Expected %c(%d) but got %c(%d).", description, expected,expected, actual, actual);
		(*failed)++;
	}
}
int assert_str(char *expected, char *actual, const char *description, int *total, int *failed){
	(*total)++;
	if (strcmp(expected, actual) != 0) {
		printf("\nASSERTION FAILED! on %s. Expected '%s' but got '%s'.\n", description, expected, actual);
		(*failed)++;
	}
}

int assert_null(void *ptr, const char *description, int *total, int *failed) {
	(*total)++;
	if (ptr != NULL) {
		printf("\nASSERTION FAILED! on %s. Pointer *ptr should be NULL.\n", description);
		(*failed)++;
	}
}

int assert_word(char *expected, char *actual, const char *description, int *total, int *failed) {
	(*total)++;
	if (expected[0] != actual[0] || expected[1] != actual[1] || expected[2] != actual[2]) {
		printf("\nASSERTION FAILED! on %s. should be %d,%d,%d but got %d,%d,%d",description,expected[0],expected[1],expected[2],actual[0],actual[1],actual[2]);
		(*failed)++;
	}
}