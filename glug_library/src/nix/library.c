#include <dlfcn.h>

#include "../library_platform.h"
#include <glug/library/library.h>


so_handle_t load_lib(const char *name)
{
    return dlopen(name, RTLD_NOW);
}

so_handle_t lazy_load_lib(const char *name)
{
    return dlopen(name, RTLD_LAZY);
}

void *get_lib_proc(const so_handle_t so, const char *proc)
{
    return dlsym(so, proc);
}

void free_lib(so_handle_t so)
{
    dlclose(so);
}
