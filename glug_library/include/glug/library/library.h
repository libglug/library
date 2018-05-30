#ifndef GLUG_LIBRARY_H
#define GLUG_LIBRARY_H

#include <glug/extern.h>
#include <glug/namespace.h>

GLUG_NAMESPACE_START(glug)

struct library;

GLUG_EXTERN_START

typedef void (*generic_fcn)(void);
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
