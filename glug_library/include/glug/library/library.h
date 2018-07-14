#ifndef GLUG_LIBRARY_H
#define GLUG_LIBRARY_H

#include <stddef.h>

#include <glug/extern.h>
#include <glug/namespace.h>

#include "typedef_func.h"

GLUG_NAMESPACE_START(glug)

struct library;
typedef_func(generic_fcn, void*, void);

GLUG_EXTERN_START
extern const char *lib_extension;

int             lib_exists(const char *name);

struct library *load_library(const char *name);
struct library *lazy_library(const char *name);
void            free_library(const struct library *lib);
int             lib_is_loaded(const struct library *lib);

int             has_proc(const struct library *lib, const char *name);
generic_fcn     get_proc(const struct library *lib, const char *name);

char **         lib_symbols(const struct library *lib, size_t *count);
char **         free_lib_symbols(char **symbol_list);


GLUG_EXTERN_END

GLUG_NAMESPACE_END

#endif // GLUG_LIBRARY_H
