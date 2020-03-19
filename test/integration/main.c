#include <glug/library/library.h>
#include <glug/library/func_t.h>

#include <CUnit/Assert.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <string.h>

#include "../create_suite.h"

func_t(say, void, const char *);

char *make_filename(const char *lib_name)
{
    const char *extension = glug_lib_extension();
    const size_t ext_len = strlen(extension);
    const size_t name_len = strlen(lib_name);
    const size_t full_len = ext_len + name_len + 1;
    char *full_name = malloc(ext_len + name_len + 1);

    strncpy(full_name, lib_name, name_len);
    strncpy(full_name + name_len, extension, ext_len);
    full_name[full_len - 1] = '\0';

    return full_name;
}

void test_load_hello(void)
{
    const char *lib_name = "hello";
    int is_lib = 0;
    char *lib_soname;
    char *full_name = make_filename(lib_name);
    char **hello_symbols;
    size_t symbols = 0, namelen = 0;
    struct glug_library *libhello;
    say say_hello;

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
    CU_pSuite suite = create_suite("load libraries", NULL, NULL);
    if (!suite) return CU_get_error();

    CU_add_test(suite, "load and iterate \"hello\"", test_load_hello);

    /* Run all tests using the console interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    if (CU_basic_run_tests() != CUE_SUCCESS)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return run_tests(CU_BRM_VERBOSE);
}
