#include <dlfcn.h>

#include <glug/library/library.h>
#include <glug/library/handle.h>

#include "../library.h"

namespace glug
{

#if defined(GLUG_OS_LIN)
  const char *lib_extension = ".so";
#elif defined(GLUG_OS_MAC)
  const char *lib_extension = ".dylib";
#endif

int lib_exists(const char *name)
{
  void *so = dlopen(name, RTLD_LAZY);
  dlclose(so);
  return so != nullptr;
}

struct library *load_library(const char *name)
{
  void *so = dlopen(name, RTLD_NOW);
  return so ? new struct library({name, true, so}) : nullptr;
}

void free_library(const struct library *lib)
{
  if (lib)
  {
    dlclose(lib->dll);
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

  return reinterpret_cast<generic_fcn>(dlsym(lib->dll, proc));
}

int has_proc(const struct library *lib, const char *proc)
{
  return get_proc(lib, proc) != nullptr;
}

so_handle lib_handle(const struct library *lib)
{
  if (!lib)
    return nullptr;

  return lib->dll;
}

} // namespace glug
