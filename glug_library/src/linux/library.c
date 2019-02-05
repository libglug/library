#include "../library_platform.h"
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include <stdlib.h>

const char *lib_extension = ".so";

char **lib_symbols(const so_handle so, size_t *count)
{
    char **symbols = malloc(sizeof(char *));

    symbols[0] = NULL;

    return symbols;
}
