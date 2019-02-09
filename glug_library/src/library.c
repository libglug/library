#include <glug/library/library.h>
#include <glug/library/handle.h>
#include "library_platform.h"
#include "library_t.h"

#include <string.h>
#include <stdlib.h>

#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) < (y) ? (x) : (y)

static void load_lazy_lib(struct glug_library_t *lib)
{
    free_lib(lib->dl);
    lib->dl = load_lib(lib->name);
    lib->loaded = 1;
}

void glug_lib_make_name(char *ext_name, const char *name, size_t count)
{
    size_t maxchar = count - 1;
    const size_t name_len = strlen(name);
    maxchar = MIN(maxchar, name_len + strlen(lib_extension));

    strncpy(ext_name, name, MAX(count, 0));
    count -= MIN(name_len, count);
    strncpy(ext_name + name_len, lib_extension, count);
    ext_name[maxchar] = '\0'; // mark the last byte as null, in case the other strings don't fit
}

int glug_lib_exists(const char *name)
{
    so_handle dl = lazy_load_lib(name);
    if (dl)
        free_lib(dl);

    return dl != NULL;
}

static struct glug_library_t *make_struct(const char *name, int loaded, so_handle dl)
{
    size_t name_len = strlen(name);
    struct glug_library_t *l = malloc(sizeof(struct glug_library_t));
    l->name = malloc(name_len + 1);
    strncpy(l->name, name, name_len + 1);
    l->loaded = loaded;
    l->dl = dl;
    return l;
}

struct glug_library_t *glug_lib_load(const char *name)
{
    so_handle dl = load_lib(name);
    if (!dl) return NULL;

    return make_struct(name, 1, dl);
}

struct glug_library_t *glug_lib_lazy(const char *name)
{
    so_handle dl = lazy_load_lib(name);
    if (!dl) return NULL;

    return make_struct(name, 0, dl);
}

void glug_lib_free(struct glug_library_t *lib)
{
    if (lib)
    {
        free(lib->name);
        free_lib(lib->dl);
        free(lib);
    }
}

int lib_is_loaded(const struct glug_library_t *lib)
{
    return lib && lib->loaded;
}

int glug_lib_has_proc(const struct glug_library_t *lib, const char *proc)
{
    return glug_lib_proc(lib, proc) != NULL;
}

generic_fcn glug_lib_proc(const struct glug_library_t *lib, const char *proc)
{
    if (!lib)
        return NULL;

    if (!lib->loaded)
        load_lazy_lib((struct glug_library_t *)lib);

    return (generic_fcn)get_lib_proc(lib->dl, proc);
}

char **glug_lib_symbols(const struct glug_library_t *lib, size_t *count)
{
    char **symbols = lib_symbols(lib->dl, count);
    // always provide a "list" to iterate
    if (!symbols && count)
    {
        symbols = malloc(sizeof(char *));
        symbols[0] = NULL;
        *count = 0;
    }

    return symbols;
}

char **glug_lib_free_symbols(char **symbol_list)
{
    for (char **psym = symbol_list; *psym; ++psym)
        free(*psym);

    free(symbol_list);

    return NULL;
}

so_handle lib_handle(const struct glug_library_t *lib)
{
    if (!lib)
        return NULL;

    if (!lib->loaded)
        load_lazy_lib((struct glug_library_t *)lib);

    return lib->dl;
}
