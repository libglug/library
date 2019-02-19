#include <glug/library/library.h>
#include <CUnit/Basic.h>
#include <CUnit/Assert.h>

#include <string.h>

#include "create_suite.h"

const char *lib_extension;

void before_each(void)
{
    lib_extension = ".ext";
}

void make_filename(void)
{
    char filename[100], exp[100] = {0};
    size_t count = 0, expcount;
    lib_extension = ".libext";
    strcat(exp, "testlib");
    strcat(exp, lib_extension);
    expcount = strlen(exp) + 1; // counts trailing null
    count = glug_lib_make_filename(filename, "testlib", 100);

    CU_ASSERT_STRING_EQUAL(filename, exp);
    CU_ASSERT_EQUAL(count, expcount);
}

void filename_len(void)
{
    size_t count = 0, exp;
    exp = strlen("testing");
    exp += strlen(lib_extension);
    exp += 1; // trailing null is counted

    count = glug_lib_make_filename(NULL, "testing", 0);

    CU_ASSERT_EQUAL(count, exp);
}

void lacking_space(void)
{
    char filename[5];
    glug_lib_make_filename(filename, "testing", 5);

    CU_ASSERT_STRING_EQUAL(filename, "test");
}

int main(void)
{
    int failures = 0;
    CU_pSuite suite = create_suite("make_filename", before_each, NULL);
    if (!suite) return CU_get_error();

    CU_add_test(suite, "make filename", make_filename);
    CU_add_test(suite, "get filename length", filename_len);
    CU_add_test(suite, "create filename in too small buffer", lacking_space);

    failures = run_tests(CU_BRM_VERBOSE);
    CU_cleanup_registry();

    return failures;
}
