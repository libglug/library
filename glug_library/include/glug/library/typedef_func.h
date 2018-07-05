#include <glug/os.h>

#ifdef GLUG_OS_WIN

    #define typedef_func(name, ret, ...) typedef ret(__stdcall *(name))(__VA_ARGS__)

#else

    #define typedef_func(name, ret, ...) typedef ret(*(name))(__VA_ARGS__)

#endif
