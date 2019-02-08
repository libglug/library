#ifndef GLUG_STRUCT_LIBRARY_T_H
#define GLUG_STRUCT_LIBRARY_T_H

#include <glug/library/library_t.h>
#include <glug/library/handle.h>

struct glug_library_t
{
    char *name;
    int loaded;
    so_handle dl;
};

#endif // GLUG_STRUCT_LIBRARY_T_H
