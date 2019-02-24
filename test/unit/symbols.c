#include <glug/library/library.h>
#include <CUnit/Basic.h>
#include <CUnit/Assert.h>

#include "../create_suite.h"

void test_empty_symbols(void)
{
    size_t count = (size_t)-1;
    struct glug_library_t *lib = glug_lib_load("empty");
    char **syms;
    CU_ASSERT_PTR_NOT_NULL(lib);

    syms = glug_lib_symbols(lib, &count);

    CU_ASSERT_EQUAL(count, 0);
    CU_ASSERT_PTR_NOT_NULL(syms);
    CU_ASSERT_PTR_NULL(syms[0]);
}

void test_lib_symbols(void)
{
    size_t count = (size_t)-1;
    struct glug_library_t *lib = glug_lib_load("testlib1");
    char **syms = glug_lib_symbols(lib, &count);

    CU_ASSERT_EQUAL(count, 2);
    CU_ASSERT_STRING_EQUAL(syms[0], "testf");
    CU_ASSERT_STRING_EQUAL(syms[1], "testf2");
    CU_ASSERT_PTR_NULL(syms[2]);
}

int main(void)
{
    CU_pSuite suite = create_suite("library symbols", NULL, NULL);
    if (!suite) return CU_get_error();

    CU_add_test(suite, "empty symbol list", test_empty_symbols);
    CU_add_test(suite, "get symbol list", test_lib_symbols);

    return run_tests(CU_BRM_VERBOSE);
}
