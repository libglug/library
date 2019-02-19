#include <glug/library/library.h>
#include <glug/library/typedef_func.h>

#include <CUnit/Assert.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <string.h>

typedef_func(say, void, const char *);

void test_load_hello(void)
{
    const char *lib_name = "hello";
    int is_lib = 0;
    char *full_name, *lib_soname;
    size_t full_len = glug_lib_make_filename(NULL, lib_name, 0);
    char **hello_symbols;
    size_t symbols = 0, namelen = 0;
    struct glug_library_t *libhello;
    say say_hello;

    full_name = malloc(full_len);
    glug_lib_make_filename(full_name, lib_name, full_len);
    CU_ASSERT_TRUE(!strncmp(full_name, lib_name, 5)); // compare the file name w/o extension

    // lib should exist
    is_lib = glug_lib_exists(full_name);
    CU_ASSERT_TRUE_FATAL(is_lib);

    // lib shouldn't be loaded since it's loaded lazily
    libhello = glug_lib_lazy(full_name);
    CU_ASSERT_FALSE(glug_lib_is_loaded(libhello));

    // getting the lib's symbols: [say_hello]
    hello_symbols = glug_lib_symbols(libhello, &symbols);
    CU_ASSERT_EQUAL(symbols, 1);
    CU_ASSERT_STRING_EQUAL(hello_symbols[0], "say_hello");
    CU_ASSERT_PTR_NULL(hello_symbols[1]);

    // get lib name length and malloc big enough buffer (w/ trailing NULL)
    namelen = glug_lib_soname(NULL, 0, libhello);
    lib_soname = malloc(namelen * sizeof(char));
    glug_lib_soname(lib_soname, namelen, libhello);
    CU_ASSERT_EQUAL(strncmp(lib_soname, "hello", 5), 0);

    // getting proc should load the library
    say_hello = (say)glug_lib_proc(libhello, "say_hello");
    CU_ASSERT_TRUE(glug_lib_is_loaded(libhello));
    CU_ASSERT_PTR_NOT_NULL(say_hello);

    free(lib_soname);
    hello_symbols = glug_lib_free_symbols(hello_symbols);
    glug_lib_free(libhello);
    free(full_name);
}

int main()
{
    unsigned int failures;
    CU_pSuite test_suite;
    /* initialize the CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    /* add suites to the registry */
    test_suite = CU_add_suite("test testing", NULL, NULL);
    if (test_suite == NULL)
    {
       CU_cleanup_registry();
       return CU_get_error();
    }

    CU_add_test(test_suite, "load and iterate \"hello\"", test_load_hello);

    /* Run all tests using the console interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    if (CU_basic_run_tests() != CUE_SUCCESS)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    failures = CU_get_number_of_tests_failed();
    CU_cleanup_registry();
    return (int)failures;
}
