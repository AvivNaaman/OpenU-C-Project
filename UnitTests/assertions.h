
#define SUCCESS 1
#define FAILURE 0

int assert_true(int value, const char *description, int *total, int *failed);
int assert_false(int value, const char *description, int *total, int *failed);
int assert_int(int expected, int actual, const char *description, int *total, int *failed);
int assert_double(double expected, double actual, const char *description, int *total, int *failed);
int assert_char(char expected, char actual, const char *description, int *total, int *failed);
int assert_str(char *expected, char *actual, const char *description, int *total, int *failed);
