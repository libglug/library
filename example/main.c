#include <glug/library/library.h>
#include <glug/library/typedef_func.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef_func(say, void, const char *);

int main()
{
    const char *lib_name = "hello";
    const size_t name_len = strlen(lib_name);
    const size_t ext_len = strlen(lib_extension);
    const size_t full_len = name_len + ext_len;

    char *full_name = malloc(full_len + 1);
    strncpy(full_name, lib_name, name_len);
    strncpy(full_name + name_len, lib_extension, ext_len);

    int is_lib = lib_exists(full_name);
    printf("%s %s\n", full_name, is_lib ? "exists" : "doesn't exist");

    if (is_lib)
    {
        size_t symbols = 0;
        struct library *libhello = lazy_library(full_name);
        char **hello_symbols = lib_symbols(libhello, &symbols);

        printf("symbols:\n");
        for (;symbols--;)
            printf("%s\n", hello_symbols[symbols]);

        say say_hello = (say)get_proc(libhello, "say_hello");
        if (say_hello)
            say_hello("glub_lib");

        hello_symbols = free_lib_symbols(hello_symbols);
        free_library(libhello);
    }

    return 0;
}
