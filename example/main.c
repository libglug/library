#include <glug/library/library.h>
#include <glug/library/typedef_func.h>
#include <glug/library/handle.h>

#include <stdio.h>

#include <glug/os.h>

typedef_func(say, void, const char *);

int main()
{
  const char *lib_name = "hello";
  int is_lib = lib_exists(lib_name);
  printf("%s %s\n", lib_name, is_lib ? "exists" : "doesn't exist");

  if (is_lib)
  {
    struct library *lib = lazy_library(lib_name);
    say say_hello = (say)get_proc(lib, "say_hello");

    if (say_hello)
      say_hello("glub_lib");

    free_library(lib);
  }

  return 0;
}
