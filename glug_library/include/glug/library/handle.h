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

#elif defined(GLUG_OS_LINUX) || defined(GLUG_OS_MAC)



#endif

GLUG_EXTERN_END
GLUG_NAMESPACE_END

#endif // GLUG_LIB_HANDLE_H
