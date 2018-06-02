#ifndef GLUG_LIB_HANDLE_H
#define GLUG_LIB_HANDLE_H

#include <glug/extern.h>
#include <glug/namespace.h>
#include <glug/os.h>

GLUG_NAMESPACE_START(glug)
GLUG_EXTERN_START

#if defined(GLUG_OS_WIN)

  #include <windef.h>
  HMODULE lib_handle(const struct library *lib);
  const char *extension = "dll";

#elif defined(GLUG_OS_LIN) || defined(GLUG_OS_MAC)

  #if defined(GLUG_OS_MAC)

    const char *extension = "dylib";

  #else

    const char *extension = "so";

  #endif
#endif

GLUG_EXTERN_END
GLUG_NAMESPACE_END

#endif // GLUG_LIB_HANDLE_H
