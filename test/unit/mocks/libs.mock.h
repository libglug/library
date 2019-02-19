#ifndef MOCKLIBS_H
#define MOCKLIBS_H

#include <glug/library/handle.h>

#define LIB_COUNT 4

typedef struct
{
    char name[52];
    void *addr;
} fcn_info;

typedef struct
{
    so_handle handle;
    char name[52];
    int nfcn;
    fcn_info fcns[10];
} library_info;

extern library_info libraries[LIB_COUNT];
library_info *find_library_by_name(const char *name);
library_info *find_library_by_handle(const so_handle handle);

void *find_library_symbol(const so_handle so, const char *name);

#endif // MOCKLIBS_H
