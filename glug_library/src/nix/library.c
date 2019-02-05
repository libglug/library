#include <dlfcn.h>

#include "../library_t.h"
#include <glug/library/library.h>


so_handle load_lib(const char *name)
{
    return dlopen(name, RTLD_NOW);
}

so_handle lazy_load_lib(const char *name)
{
    return dlopen(name, RTLD_LAZY);
}

void *get_lib_proc(const so_handle so, const char *proc)
{
    return dlsym(so, proc);
}

void free_lib(so_handle so)
{
    dlclose(so);
}
