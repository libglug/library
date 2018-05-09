#include <glug/os.h>

#ifdef GLUG_OS_WIN

  #define typedef_func(r, x, ...) typedef r(__stdcall *(x))(__VA_ARGS__)

#elif

  #define typedef_func(r, x, ...) typedef r(*(x))(__VA_ARGS__)

#endif
