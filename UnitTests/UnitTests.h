#define TEST(name) int test_##name(int *total, int *failed)
#define CALL_TEST(name, total, failed) test_##name(&total, &failed);

TEST(Table);
TEST(Utils);
TEST(Instructions);
/**/
TEST(Code);
TEST(Code_GetOpCodeFunc);
TEST(Code_GetAddrType);
/**/