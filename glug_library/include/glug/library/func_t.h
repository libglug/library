#include <glug/os.h>

#if GLUG_OS == GLUG_OS_WIN

    #define func_t(name, ret, ...) typedef ret(__stdcall *(name))(__VA_ARGS__)

#elif GLUG_OS == GLUG_OS_MAC || GLUG_OS == GLUG_OS_LIN || GLUG_OS == GLUG_OS_BSD

    #define func_t(name, ret, ...) typedef ret(*(name))(__VA_ARGS__)

#endif
