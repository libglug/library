#include <dlfcn.h>

#include <glug/library/library.h>
#include <glug/library/handle.h>

#include "../library.h"

namespace glug
{

#if defined(GLUG_OS_LIN)
    const char *lib_extension = ".so";
#elif defined(GLUG_OS_MAC)
    const char *lib_extension = ".dylib";
#endif

void load_lazy_lib(struct library *lib)
{
    dlclose(lib->dl);
    lib->dl = dlopen(lib->name, RTLD_NOW);
    lib->loaded = true;
}

int lib_exists(const char *name)
{
    void *so = dlopen(name, RTLD_LAZY);
    if (so) dlclose(so);
    return so != nullptr;
}

struct library *load_library(const char *name)
{
    void *so = dlopen(name, RTLD_NOW);
    return so ? new struct library({name, true, so}) : nullptr;
}

struct library *lazy_library(const char *name)
{
    void *so = dlopen(name, RTLD_LAZY);
    return so ? new struct library({name, false, so}) : nullptr;
}

void free_library(const struct library *lib)
{
    if (lib)
    {
        dlclose(lib->dl);
        delete lib;
    }
}

int lib_is_loaded(const struct library *lib)
{
    return lib && lib->loaded;
}

generic_fcn get_proc(const struct library *lib, const char *proc)
{
    if (!lib)
        return nullptr;

    const_cast<struct library *>(lib)->loaded = true;
    return reinterpret_cast<generic_fcn>(dlsym(lib->dl, proc));
}

int has_proc(const struct library *lib, const char *proc)
{
    return get_proc(lib, proc) != nullptr;
}

so_handle lib_handle(const struct library *lib)
{
    if (!lib)
        return nullptr;

    if (!lib->loaded)
        load_lazy_lib(const_cast<struct library *>(lib));

    return lib->dl;
}

} // namespace glug
