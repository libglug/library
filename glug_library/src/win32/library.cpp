#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <DbgHelp.h>

// MinGW :(
#ifndef SLMFLAG_NO_SYMBOLS
#define SLMFLAG_NO_SYMBOLS 0x4
#endif

#include "../library.h"
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include <vector>

namespace glug
{

const char *lib_extension = ".dll";

void load_lazy_dll(struct library *lib)
{
  FreeLibrary(lib->dl);
  lib->dl = LoadLibrary(lib->name);
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
    FreeLibrary(lib->dl);
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

  if (!lib->loaded)
    load_lazy_dll(const_cast<struct library *>(lib));

  return reinterpret_cast<generic_fcn>(GetProcAddress(lib->dl, proc));
}

int has_proc(const struct library *lib, const char *proc)
{
  return get_proc(lib, proc) != nullptr;
}

static BOOL __stdcall enum_syms(PSYMBOL_INFO info, ULONG, PVOID context)
{
  std::vector<char *> *syms = reinterpret_cast<std::vector<char *> *>(context);
  char *symbol = new char[info->NameLen + 1]();
  memcpy_s(symbol, info->NameLen, info->Name, info->NameLen);
  symbol[info->NameLen] = '\0';
  syms->push_back(symbol);

  return TRUE;
}

char **lib_symbols(const struct library *lib, size_t *count)
{
  std::vector<char *> symbol_list;

  HANDLE proc = GetCurrentProcess();
  SymInitialize(proc, NULL, FALSE);
  DWORD64 base = SymLoadModuleEx(proc, NULL, lib->name, NULL, 0, 0, NULL, SLMFLAG_NO_SYMBOLS);
  SymEnumSymbols(proc, base, NULL, enum_syms, &symbol_list);
  SymUnloadModule(proc, base);
  SymCleanup(proc);

  size_t list_len = symbol_list.size();
  char **symbols = new char*[list_len + 1]();
  for (int i = 0; i < list_len; ++i)
    symbols[i] = symbol_list[i];
  symbols[list_len] = nullptr;

  if (count) *count = symbol_list.size();

  return symbols;
}

char **free_lib_symbols(char **symbol_list)
{
  for (char **psym = symbol_list; *psym; ++psym)
    delete *psym;

  delete symbol_list;

  return nullptr;
}

so_handle lib_handle(const struct library *lib)
{
  if (!lib)
    return nullptr;

  if (!lib->loaded)
    load_lazy_dll(const_cast<struct library *>(lib));

  return lib->dl;
}

} // namespace glug
