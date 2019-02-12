#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../library_t.h"
#include "../library_platform.h"
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include <stdlib.h>

const char *lib_extension = ".dll";

static int get_exports(const so_handle so, PBYTE *base, IMAGE_EXPORT_DIRECTORY *exp)
{
    IMAGE_DOS_HEADER *doshdr;
    IMAGE_FILE_HEADER fhdr;
    IMAGE_OPTIONAL_HEADER opthdr;
    IMAGE_SECTION_HEADER shdr;
    PBYTE ntaddr, optaddr, shaddr;
    DWORD exprva, sig;
    size_t i = 0;

    // verify this is a valid PE by checking signatures
    doshdr = (IMAGE_DOS_HEADER *)so;
    if (doshdr->e_magic != IMAGE_DOS_SIGNATURE) return 1;
    *base = (PBYTE)doshdr;
    ntaddr = *base + doshdr->e_lfanew;
    memcpy(&sig, ntaddr, sizeof(DWORD));
    if (sig != IMAGE_NT_SIGNATURE) return 1;

    // read out the file header and optional headers
    memcpy(&fhdr, ntaddr + sizeof(DWORD), sizeof(IMAGE_FILE_HEADER));
    optaddr = ntaddr + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER);
    memcpy(&opthdr, optaddr, sizeof(IMAGE_OPTIONAL_HEADER));

    // set base section header address and store the export rva
    shaddr = optaddr + fhdr.SizeOfOptionalHeader;
    exprva = opthdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    // find the section header that corresponds to the export rva
    do memcpy(&shdr, shaddr + i * sizeof(IMAGE_SECTION_HEADER), sizeof(IMAGE_SECTION_HEADER));
    while(shdr.VirtualAddress <= exprva && exprva < shdr.VirtualAddress + shdr.SizeOfRawData);

    // copy the export directory
    memcpy(exp, *base + exprva, sizeof(IMAGE_EXPORT_DIRECTORY));

    return 0;
}

so_handle load_lib(const char *name)
{
    return LoadLibrary(TEXT(name));
}

so_handle lazy_load_lib(const char *name)
{
    return LoadLibraryEx(TEXT(name), NULL, DONT_RESOLVE_DLL_REFERENCES);
}

size_t lib_name(char *dst, size_t count, const so_handle so)
{
    PBYTE base;
    IMAGE_EXPORT_DIRECTORY exp;
    if (get_exports(so, &base, &exp)) return 0;

    strncpy(dst, (char *)(base + exp.Name), count);
    if (count) dst[count - 1] = '\0'; // mark the last byte as null, in case the string didn't fit
    return strlen((char *)(base + exp.Name));
}

void *get_lib_proc(const so_handle so, const char *proc)
{
    return (void *)GetProcAddress(so, TEXT(proc));
}

void free_lib(so_handle so)
{
    FreeLibrary(so);
}

char **lib_symbols(const so_handle so, size_t *count)
{
    char **symbols = NULL;
    DWORD *nameaddrs;
    PBYTE base;
    IMAGE_EXPORT_DIRECTORY exp;
    size_t i = 0;

    if (get_exports(so, &base, &exp)) return NULL;

    // allocate symbols array
    nameaddrs = malloc(exp.NumberOfNames * sizeof(DWORD));
    memcpy(nameaddrs, base + exp.AddressOfNames, exp.NumberOfNames * sizeof(DWORD));
    symbols = malloc((exp.NumberOfNames + 1) * sizeof(char *));

    // copy strings into the symbols array
    for (i = 0; i < exp.NumberOfNames; ++i)
        symbols[i] = _strdup((char *)(base + nameaddrs[i]));
    free(nameaddrs);

    // "return" the number of symbols
    if (count) *count = exp.NumberOfNames;

    // set the last symbol entry as NULL to simplify iteration and return
    symbols[exp.NumberOfNames] = NULL;
    return symbols;
}
