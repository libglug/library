#include <glug/os.h>

#ifdef GLUG_OS_WIN

    #define func_t(name, ret, ...) typedef ret(__stdcall *(name))(__VA_ARGS__)

#else

    #define func_t(name, ret, ...) typedef ret(*(name))(__VA_ARGS__)

#endif
