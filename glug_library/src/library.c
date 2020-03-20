#include <glug/library/library.h>
#include <glug/library/handle.h>
#include "library_platform.h"
#include "library_t.h"

#include <string.h>
#include <stdlib.h>

static void load_lazy_lib(struct glug_library *lib)
{
    free_lib(lib->dl);
    lib->dl = load_lib(lib->name);
    lib->loaded = true;
}

glug_bool glug_lib_exists(const char *name)
{
    so_handle_t dl = lazy_load_lib(name);
    if (dl)
        free_lib(dl);

    return dl != NULL;
}

const char *glug_lib_extension()
{
    return lib_extension();
}

static struct glug_library *make_struct(const char *name, glug_bool loaded, so_handle_t so)
{
    size_t name_len = strlen(name);
    struct glug_library *l = malloc(sizeof(struct glug_library));
    l->name = malloc(name_len + 1);
    strncpy(l->name, name, name_len + 1);
    l->loaded = loaded;
    l->dl = so;
    return l;
}

void glug_lib_load(struct glug_library **lib, const char *name)
{
    so_handle_t dl = load_lib(name);
    if (!dl) return;

    *lib = make_struct(name, true, dl);
}

void glug_lib_lazy(struct glug_library **lib, const char *name)
{
    so_handle_t dl = lazy_load_lib(name);
    if (!dl) return;

    *lib = make_struct(name, false, dl);
}

void glug_lib_free(struct glug_library **lib)
{
    struct glug_library *library = *lib;

    free(library->name);
    free_lib(library->dl);
    free(library);

    *lib = NULL;
}

glug_bool glug_lib_is_loaded(const struct glug_library *lib)
{
    return lib && lib->loaded;
}

size_t glug_lib_soname(const struct glug_library *lib, char *dst, size_t count)
{
    return lib_soname(dst, count, lib->dl);
}

glug_bool glug_lib_has_proc(const struct glug_library *lib, const char *proc)
{
    return glug_lib_proc(lib, proc) != NULL;
}

generic_fcn glug_lib_proc(const struct glug_library *lib, const char *proc)
{
    if (!lib)
        return NULL;

    if (!lib->loaded)
        load_lazy_lib((struct glug_library *)lib);

    return (generic_fcn)get_lib_proc(lib->dl, proc);
}

char **glug_lib_symbols(const struct glug_library *lib, alloc_t alloc)
{
    size_t nsymbols = 0;
    char **symbols = lib_symbols(lib->dl, &nsymbols, alloc);
    // always provide a "list" to iterate
    if (!symbols)
    {
        symbols = alloc(sizeof(char *));
        symbols[0] = NULL;
    }

    return symbols;
}

so_handle_t glug_lib_handle(const struct glug_library *lib)
{
    if (!lib)
        return NULL;

    if (!lib->loaded)
        load_lazy_lib((struct glug_library *)lib);

    return lib->dl;
}
