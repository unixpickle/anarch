#ifndef __ANARCH_TEST_ASSERT_H__
#define __ANARCH_TEST_ASSERT_H__

#define TEST_ASSERT_STRX(x) #x
#define TEST_ASSERT_STR(x) TEST_ASSERT_STRX(x)
#define Assert(x) (void)((x) ?: (__Assert("Assertion failure: " #x " at " __FILE__ ":" TEST_ASSERT_STR(__LINE__))));

#ifdef __cplusplus
extern "C" {
#endif

int __Assert(const char * msg);

#ifdef __cplusplus
}
#endif

#endif
