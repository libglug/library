#include <glug/library/library.h>
#include <glug/library/typedef_func.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef_func(say, void, const char *);

int main()
{
    const char *lib_name = "hello";
    int is_lib = 0;
    const size_t name_len = strlen(lib_name);
    const size_t ext_len = strlen(lib_extension);
    const size_t full_len = name_len + ext_len + 1; // +1 for NULL

    char *full_name = malloc(full_len);
    glug_lib_make_name(full_name, lib_name, full_len);

    is_lib = glug_lib_exists(full_name);
    printf("%s %s\n", full_name, is_lib ? "exists" : "doesn't exist");

    if (is_lib)
    {
        size_t symbols = 0;
        say say_hello;
        size_t namelen = 0;
        char *lib_soname;
        struct glug_library_t *libhello = glug_lib_lazy(full_name);
        char **psym, **hello_symbols = glug_lib_symbols(libhello, &symbols);

        // get lib name length and malloc big enough buffer (w/ trailing NULL)
        namelen = glug_lib_name(NULL, 0, libhello) + 1;
        lib_soname = malloc(namelen * sizeof(char));
        glug_lib_name(lib_soname, namelen, libhello);
        printf("lib so name: %s\n", lib_soname);

        printf("%zd symbol(s):\n", symbols);
        for (psym = hello_symbols; *psym; ++psym)
            printf("  %s\n", *psym);

        say_hello = (say)glug_lib_proc(libhello, "say_hello");
        if (say_hello)
            say_hello("glub_lib");

        free(lib_soname);
        hello_symbols = glug_lib_free_symbols(hello_symbols);
        glug_lib_free(libhello);
    }

    free(full_name);
    return 0;
}
