#ifndef GLUG_LIB_HANDLE_H
#define GLUG_LIB_HANDLE_H

#include <glug/extern.h>
#include <glug/namespace.h>
#include <glug/os.h>

GLUG_NAMESPACE_START(glug)
GLUG_EXTERN_START

#if defined(GLUG_OS_WIN)

  #include <windef.h>
  typedef HMODULE so_handle;

#elif defined(GLUG_OS_LIN) || defined(GLUG_OS_MAC)

  typedef void* so_handle;

#endif

extern so_handle lib_handle(const struct library *lib);

GLUG_EXTERN_END
GLUG_NAMESPACE_END

#endif // GLUG_LIB_HANDLE_H
