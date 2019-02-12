#ifndef GLUG_LIBRARY_PLATFORM_H
#define GLUG_LIBRARY_PLATFORM_H

#include <glug/library/handle.h>

#include <stddef.h>

so_handle load_lib     (const char *name);
so_handle lazy_load_lib(const char *name);
size_t    lib_name     (char *dst, size_t count, const so_handle so);
void *    get_lib_proc (const so_handle so, const char *name);
char **   lib_symbols  (const so_handle so, size_t *count);
void      free_lib     (so_handle so);

#endif // GLUG_LIBRARY_PLATFORM_H
