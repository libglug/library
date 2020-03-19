#ifndef GLUG_STRUCT_LIBRARY_T_H
#define GLUG_STRUCT_LIBRARY_T_H

#include <glug/bool.h>

#include <glug/library/library_t.h>
#include <glug/library/handle.h>

struct glug_library
{
    char *name;
    glug_bool loaded;
    so_handle_t dl;
};

#endif // GLUG_STRUCT_LIBRARY_T_H
