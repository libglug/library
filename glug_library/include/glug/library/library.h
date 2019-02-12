#ifndef GLUG_LIBRARY_H
#define GLUG_LIBRARY_H

#include <stddef.h>

#include <glug/extern.h>

#include <glug/library/library_t.h>
#include <glug/library/typedef_func.h>

typedef_func(generic_fcn, void*, void);

GLUG_EXTERN_START
extern const char *lib_extension;

size_t                 glug_lib_make_filename(char *dst, const char *name, size_t count);
int                    glug_lib_exists(const char *name);
size_t                 glug_lib_soname(char *dst, size_t count, const struct glug_library_t *lib);

struct glug_library_t *glug_lib_load(const char *name);
struct glug_library_t *glug_lib_lazy(const char *name);
void                   glug_lib_free(struct glug_library_t *lib);
int                    glug_lib_is_loaded(const struct glug_library_t *lib);

int                    glug_lib_has_proc(const struct glug_library_t *lib, const char *proc);
generic_fcn            glug_lib_proc(const struct glug_library_t *lib, const char *proc);

char **                glug_lib_symbols(const struct glug_library_t *lib, size_t *count);
char **                glug_lib_free_symbols(char **symbol_list);

GLUG_EXTERN_END

#endif // GLUG_LIBRARY_H
