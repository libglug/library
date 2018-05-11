#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glug/library/library.h>
#include <glug/library/handle.h>

namespace glug
{

// TODO: add more fields or just remove
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

HMODULE lib_handle(const struct library *lib)
{
  return lib->dll;
}

} // namespace glug
