#include <glug/library/library.h>
#include <glug/library/func_def.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef_func(BOOL, GCP, LPPOINT);

int main()
{
  struct library *dll = load_library("User32");
  GCP getCursorPos = (GCP)get_proc(dll, "GetCursorPos");

  POINT p;
  getCursorPos(&p);
  printf("(%d, %d)\n", p.x, p.y);
}
