#include <glug/library/library.h>
#include <glug/library/typedef_func.h>
#include <glug/library/handle.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef_func(say, void, const char *);

int main()
{
  const char *lib_name = "hello";
  const int name_len = strlen(lib_name);
  const int ext_len = strlen(lib_extension);
  const int full_len = name_len + ext_len;

  char *full_name = malloc(full_len + 1);
  strcpy(full_name, lib_name);
  strcpy(full_name + name_len, lib_extension);

  int is_lib = lib_exists(full_name);
  printf("%s %s\n", full_name, is_lib ? "exists" : "doesn't exist");

  if (is_lib)
  {
    struct library *lib = lazy_library(full_name);
    say say_hello = (say)get_proc(lib, "say_hello");

    if (say_hello)
      say_hello("glub_lib");

    free_library(lib);
  }

  return 0;
}
