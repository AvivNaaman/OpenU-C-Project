///
/// Basic automated Unit tests
/// (c) Aviv Naaman, 2020, under MIT License
///
#define SUCCESS 1
#define FAILURE 0

int assert_true(int value, const char *description, int *total, int *failed);
int assert_false(int value, const char *description, int *total, int *failed);
int assert_int(int expected, int actual, const char *description, int *total, int *failed);
int assert_double(double expected, double actual, const char *description, int *total, int *failed);
int assert_char(char expected, char actual, const char *description, int *total, int *failed);
int assert_str(char *expected, char *actual, const char *description, int *total, int *failed);
int assert_null(void *ptr, const char *description, int *total, int *failed);
/* Asserts a 24-bit word, represented by char array, as little endian */
int assert_word(char expected[3], char *actual, const char *description, int *total, int *failed);
