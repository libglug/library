#include <glug/extern.h>
#include <glug/namespace.h>

GLUG_NAMESPACE_START(glug)

struct library;

GLUG_EXTERN_START

struct library *load_library(const char *name);
extern void     free_library(const struct library *lib);

int             has_proc(const struct library *lib, const char *name);
extern void    *get_proc(const struct library *lib, const char *name);

GLUG_EXTERN_END

GLUG_NAMESPACE_END
