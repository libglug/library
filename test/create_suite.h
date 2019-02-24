#ifndef CREATE_SUITE_H
#define CREATE_SUITE_H

#include <CUnit/Basic.h>

CU_pSuite create_suite(char *suite_name, CU_SetUpFunc before_each, CU_TearDownFunc after_each)
{
    CU_pSuite suite;

    /* initialize the CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS)
        return NULL;

    /* add suites to the registry */
    suite = CU_add_suite_with_setup_and_teardown(suite_name, NULL, NULL, before_each, after_each);
    if (!suite)
    {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

int run_tests(CU_BasicRunMode run_mode)
{
    int failures = 0;
    /* Run all tests using the console interface */
    CU_basic_set_mode(run_mode);
    if (CU_basic_run_tests() != CUE_SUCCESS)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    failures = (int)CU_get_number_of_tests_failed();
    CU_cleanup_registry();
    return failures;
}

#endif // CREATE_SUITE_H
