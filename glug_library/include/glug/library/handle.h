#ifndef GLUG_LIB_HANDLE_H
#define GLUG_LIB_HANDLE_H

#include <glug/import.h>
#include <glug/extern.h>
#include <glug/os.h>

#include <glug/library/library_t.h>

GLUG_EXTERN_START

#if defined(GLUG_OS_WIN)

    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    typedef HMODULE so_handle_t;

#elif defined(GLUG_OS_LIN) || defined(GLUG_OS_MAC)

    typedef void *so_handle_t;

#endif

GLUG_LIB_API so_handle_t glug_lib_handle(const struct glug_library *lib);

GLUG_EXTERN_END

#endif // GLUG_LIB_HANDLE_H
