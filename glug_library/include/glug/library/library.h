#ifndef GLUG_LIBRARY_H
#define GLUG_LIBRARY_H

#include <stddef.h>

#include <glug/import.h>
#include <glug/extern.h>
#include <glug/bool.h>

#include <glug/library/library_t.h>
#include <glug/library/func_t.h>

#include <stddef.h>

glug_func_t(generic_fcn, void *, void);
glug_func_t(alloc_t, void *, size_t);

GLUG_EXTERN_START

GLUG_LIB_API glug_bool      glug_lib_exists(const char *name);
GLUG_LIB_API const char *   glug_lib_extension(void);

GLUG_LIB_API void           glug_lib_load(struct glug_library **lib, const char *name);
GLUG_LIB_API void           glug_lib_lazy(struct glug_library **lib, const char *name);
GLUG_LIB_API void           glug_lib_free(struct glug_library **lib);
GLUG_LIB_API glug_bool      glug_lib_is_loaded(const struct glug_library *lib);

GLUG_LIB_API size_t         glug_lib_soname(const struct glug_library *lib, char *dst, size_t count);
GLUG_LIB_API glug_bool      glug_lib_has_proc(const struct glug_library *lib, const char *proc);
GLUG_LIB_API generic_fcn    glug_lib_proc(const struct glug_library *lib, const char *proc);

GLUG_LIB_API char **        glug_lib_symbols(const struct glug_library *lib, alloc_t alloc);

GLUG_EXTERN_END

#endif // GLUG_LIBRARY_H
