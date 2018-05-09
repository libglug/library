#include <glug/library/library.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace glug
{

struct library {
  HMODULE dll;
};

struct library *load_library(const char *name)
{
  HMODULE dll = LoadLibrary(TEXT(name));
  return dll ? new struct library({dll}) : nullptr;
}

void free_library(const struct library *lib)
{
  FreeLibrary(lib->dll);
  delete lib;
}

void *get_proc(const struct library *lib, const char *proc)
{
  return reinterpret_cast<void *>(GetProcAddress(lib->dll, proc));
}

int has_proc(const struct library *lib, const char *proc)
{
  return get_proc(lib, proc) != NULL;
}

} // namespace glug
