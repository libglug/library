#include <glug/library/library.h>
#include <glug/library/func_def.h>

#include <stdio.h>

typedef struct point {
  int x;
  int y;
} point;

typedef_func(int, GCP, point*);

int main()
{
  struct library *dll = load_library("User32");
  GCP getCursorPos = (GCP)get_proc(dll, "GetCursorPos");

  point p;
  getCursorPos(&p);
  printf("(%d, %d)\n", p.x, p.y);
}
