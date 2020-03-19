#include <library_platform.h>
#include "libs.mock.h"

#include <stdlib.h>

char **lib_symbols(const so_handle_t so, size_t *count)
{
    library_info *li = find_library_by_handle(so);
    char **syms;
    int i;

    if (!li) return NULL;
    if (!li->nfcn) return NULL;

    syms = malloc((li->nfcn + 1) * sizeof(char *));
    for(i = 0; i < li->nfcn; ++i)
        syms[i] = li->fcns[i].name;
    *count = li->nfcn;
    syms[i] = NULL;

    return syms;
}
