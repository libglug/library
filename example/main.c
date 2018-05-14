#include <glug/library/library.h>
#include <glug/library/typedef_func.h>

#include <stdio.h>

#include <glug/os.h>

#if defined(GLUG_OS_WIN)

typedef struct point {
  int x;
  int y;
} point;

typedef_func(gcp, int, point*);

int main()
{
  int is_lib = lib_exists("User32");
  printf("User32.dll %s\n", is_lib ? "exists" : "doesn't exist");

  if (is_lib)
  {
    struct library *dll = lazy_library("User32");
    gcp cursor_pos = (gcp)get_proc(dll, "GetCursorPos");

    point p;
    cursor_pos(&p);
    printf("(%d, %d)\n", p.x, p.y);

    free_library(dll);
  }

  return 0;
}

#elif defined(GLUG_OS_MAC)

#elif defined(GLUG_OS_LIN)

int main()
{
  const char *lib_name = "libX11.so";
  int is_lib = lib_exists(lib_name);
  printf("%s %s\n", lib_name, is_lib ? "exists" : "doesn't exist");

  if (is_lib)
  {
    struct library *so = load_library(lib_name);
    generic_fcn cursor_pos = get_proc(so, "XQueryPointer");

    free_library(so);
  }

  return 0;
}

#endif
