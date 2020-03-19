#include <library_platform.h>
#include "libs.mock.h"

so_handle_t lazy_load_lib(const char *name)
{
    const library_info *lib_info = find_library_by_name(name);
    if (lib_info)
        return lib_info->handle;

    return NULL;
}

so_handle_t load_lib(const char *name)
{
    library_info *lib_info = find_library_by_name(name);
    if (lib_info)
        return lib_info->handle;

    return NULL;
}

void free_lib(so_handle_t so)
{
    (void) so;
}

void *get_lib_proc(const so_handle_t so, const char *name)
{
    return find_library_symbol(so, name);
}

