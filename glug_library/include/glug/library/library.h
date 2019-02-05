#ifndef GLUG_LIBRARY_H
#define GLUG_LIBRARY_H

#include <stddef.h>

#include <glug/extern.h>
#include "typedef_func.h"

#include <glug/library/library_t.h>

typedef_func(generic_fcn, void*, void);

GLUG_EXTERN_START
extern const char *lib_extension;

void            glug_lib_make_name(char *ext_name, const char *name, size_t count);
int             glug_lib_exists(const char *name);

struct library *glug_lib_load(const char *name);
struct library *glug_lib_lazy(const char *name);
void            glug_lib_free(struct library *lib);
int             glug_lib_is_loaded(const struct library *lib);

int             glug_lib_has_proc(const struct library *lib, const char *proc);
generic_fcn     glug_lib_proc(const struct library *lib, const char *proc);

char **         glug_lib_symbols(const struct library *lib, size_t *count);
char **         glug_lib_free_symbols(char **symbol_list);

GLUG_EXTERN_END

#endif // GLUG_LIBRARY_H
