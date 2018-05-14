#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glug/library/library.h>
#include <glug/library/handle.h>

#include "../library.h"

namespace glug
{

const char *lib_extension = ".dll";

void load_lazy_dll(struct library *lib)
{
  FreeLibrary(lib->dll);
  lib->dll = LoadLibrary(lib->name);
  lib->loaded = true;
}

int lib_exists(const char *name)
{
  HMODULE dll = LoadLibraryEx(TEXT(name), NULL, DONT_RESOLVE_DLL_REFERENCES);
  FreeLibrary(dll);
  return dll != NULL;
}

struct library *load_library(const char *name)
{
  HMODULE dll = LoadLibrary(TEXT(name));
  return dll ? new struct library({name, true, dll}) : nullptr;
}

struct library *lazy_library(const char *name)
{
  HMODULE dll = LoadLibraryEx(TEXT(name), NULL, DONT_RESOLVE_DLL_REFERENCES);
  return dll ? new struct library({name, false, dll}) : nullptr;
}

void free_library(const struct library *lib)
{
  if (lib)
  {
    FreeLibrary(lib->dll);
    delete lib;
  }
}

int lib_is_loaded(const struct library *lib)
{
  return lib && lib->loaded;
}

generic_fcn get_proc(const struct library *lib, const char *proc)
{
  if (!lib)
    return nullptr;

  if (lib->loaded)
    load_lazy_dll(const_cast<struct library *>(lib));

  return reinterpret_cast<generic_fcn>(GetProcAddress(lib->dll, proc));
}

int has_proc(const struct library *lib, const char *proc)
{
  return get_proc(lib, proc) != nullptr;
}

so_handle lib_handle(const struct library *lib)
{
  if (!lib)
    return nullptr;

  if (lib->loaded)
    load_lazy_dll(const_cast<struct library *>(lib));

  return lib->dll;
}

} // namespace glug
