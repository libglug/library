#include <glug/library/handle.h>
#include "libs.mock.h"

#include <string.h>

library_info libraries[LIB_COUNT] = {
    {
        (so_handle)0x911881c,
        "testlib1",
        2,
        {
            { "testf", (void *)0x1234 },
            { "testf2", (void *)0x2345}
        }
    },
    {
        (so_handle)0x8a7fa08,
        "testlib2",
        1,
        {
            { "ftest", (void *)0x4321 }
        }
    },
    {
        (so_handle)0x3c77404,
        "opengl",
        2,
        {
            { "glTest", (void *)0x1111 },
            { "glTest2", (void *)0x2222 }
        }
    },
    {
        (so_handle)0x2210b88,
        "empty",
        0,
        { { "", 0 } }
    }
};

library_info *find_library_by_name(const char *name)
{
    int i;
    for (i = 0; i < LIB_COUNT; ++i)
        if (!strcmp(libraries[i].name, name))
            return &libraries[i];

    return NULL;
}

library_info *find_library_by_handle(const so_handle handle)
{
    int i;
    for (i = 0; i < LIB_COUNT; ++i)
        if (libraries[i].handle == handle)
            return &libraries[i];

    return NULL;
}

void *find_library_symbol(const so_handle so, const char *name)
{
    int i = 0;
    library_info *lib_info = find_library_by_handle(so);
        if (!lib_info)
            return NULL;

    for (i = 0; i < lib_info->nfcn; ++i)
        if (!strcmp(lib_info->fcns[i].name, name))
            return lib_info->fcns[i].addr;

    return NULL;
}
