#ifndef GLUG_LIBRARY_H
#define GLUG_LIBRARY_H

#include <glug/extern.h>
#include <glug/namespace.h>

#include "typedef_func.h"

GLUG_NAMESPACE_START(glug)

struct library;
typedef_func(generic_fcn, void*, void);
extern const char *lib_extension;

GLUG_EXTERN_START

int             lib_exists(const char *name);

struct library *load_library(const char *name);
struct library *lazy_library(const char *name);
void            free_library(const struct library *lib);
int             lib_is_loaded(const struct library *lib);

int             has_proc(const struct library *lib, const char *name);
generic_fcn     get_proc(const struct library *lib, const char *name);

GLUG_EXTERN_END

GLUG_NAMESPACE_END

#endif // GLUG_LIBRARY_H
