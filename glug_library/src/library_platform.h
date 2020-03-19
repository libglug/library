#ifndef GLUG_LIBRARY_PLATFORM_H
#define GLUG_LIBRARY_PLATFORM_H

#include <glug/import.h>
#include <glug/library/handle.h>

#include <stddef.h>
GLUG_LIB_LOCAL const char * lib_extension(void);
GLUG_LIB_LOCAL so_handle_t  load_lib     (const char *name);
GLUG_LIB_LOCAL so_handle_t  lazy_load_lib(const char *name);
GLUG_LIB_LOCAL size_t       lib_soname   (char *dst, size_t count, const so_handle_t so);
GLUG_LIB_LOCAL void *       get_lib_proc (const so_handle_t so, const char *name);
GLUG_LIB_LOCAL char **      lib_symbols  (const so_handle_t so, size_t *count);
GLUG_LIB_LOCAL void         free_lib     (so_handle_t so);

#endif // GLUG_LIBRARY_PLATFORM_H
