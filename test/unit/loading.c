#include <glug/library/library.h>
#include <CUnit/Basic.h>
#include <CUnit/Assert.h>

#include "../create_suite.h"

const char *lib_extension()
{
    return ".ext";
}

void test_libs_exist(void)
{
    int exists = glug_lib_exists("library1");
    CU_ASSERT_FALSE(exists);

    exists = glug_lib_exists("testlib1");
    CU_ASSERT_TRUE(exists);

    exists = glug_lib_exists("otherlib");
    CU_ASSERT_FALSE(exists);

    exists = glug_lib_exists("opengl");
    CU_ASSERT_TRUE(exists);
}

void test_load(void)
{
    struct glug_library *lib;
    glug_lib_lazy(&lib, "testlib1");
    CU_ASSERT_FALSE(glug_lib_is_loaded(lib));

    glug_lib_load(&lib, "opengl");
    CU_ASSERT_TRUE(glug_lib_is_loaded(lib));

    glug_lib_load(&lib, "garbage");
    CU_ASSERT_FALSE(glug_lib_is_loaded(lib));
}

void test_getproc(void)
{
    generic_fcn proc = NULL;
    struct glug_library *lib;
    glug_lib_lazy(&lib, "testlib2");
    CU_ASSERT_FALSE(glug_lib_is_loaded(lib));

    CU_ASSERT_FALSE(glug_lib_has_proc(lib, "testf2"))
    proc = glug_lib_proc(lib, "ftest");
    CU_ASSERT_PTR_NOT_NULL(proc);
    CU_ASSERT_TRUE(glug_lib_is_loaded(lib));

    glug_lib_load(&lib, "testlib1");
    CU_ASSERT_TRUE(glug_lib_is_loaded(lib));

    CU_ASSERT_TRUE(glug_lib_has_proc(lib, "testf2"));
    proc = glug_lib_proc(lib, "testf2");
    CU_ASSERT_PTR_NOT_NULL(proc);
    CU_ASSERT_TRUE(glug_lib_is_loaded(lib));
}

int main(void)
{
    CU_pSuite suite = create_suite("load libraries", NULL, NULL);
    if (!suite) return CU_get_error();

    CU_add_test(suite, "libs exist", test_libs_exist);
    CU_add_test(suite, "load libs", test_load);
    CU_add_test(suite, "load lib when getting proc", test_getproc);

    return run_tests(CU_BRM_VERBOSE);
}
