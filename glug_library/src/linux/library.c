#include <dlfcn.h>
#include <elf.h>
#include <link.h>

#include "../library_platform.h"
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include <stdlib.h>
#include <string.h>

const char *lib_extension = ".so";

static const ElfW(Dyn) *get_tag(const ElfW(Dyn) *dyn, const ElfW(Sxword) tag)
{
    for (const ElfW(Dyn) *entry = dyn; entry->d_tag != DT_NULL; ++entry)
        if (entry->d_tag == tag)
            return entry;

    return NULL;
}

static int is_public_fcn(const ElfW(Sym) *symbol, size_t maxoffset)
{
    return ELF32_ST_TYPE(symbol->st_info) == STT_FUNC &&   // only functions...
           ELF32_ST_BIND(symbol->st_info) == STB_GLOBAL && // that are exported (global)...
           symbol->st_size &&                              // that are defined in this lib...
           symbol->st_name < maxoffset;                    // and have a string in the strtab
}

char **lib_symbols(const so_handle so, size_t *count)
{
    char **symbols = NULL;
    struct link_map *map = (struct link_map*)so;
    ElfW(Dyn) *elf = map->l_ld;
    size_t strsz;
    uint8_t *pstrtab;
    const ElfW(Sym) *symbol, *psymtab;
    size_t nsym = 0;

    // get the size of the string table, pointers to the string and symbol tables
    strsz   = get_tag(elf, DT_STRSZ)->d_un.d_val;
    pstrtab = (uint8_t *)get_tag(elf, DT_STRTAB)->d_un.d_ptr;
    psymtab = (const ElfW(Sym) *)get_tag(map->l_ld, DT_SYMTAB)->d_un.d_ptr;

    // if strsz, strtab, or symtab are NULL, fail
    if (!strsz || !pstrtab || !psymtab) return symbols;

    // count relevant symbols between the STN_UNDEF symbols
    for(symbol = psymtab + 1; symbol && symbol->st_info != STN_UNDEF; ++symbol)
        if (is_public_fcn(symbol, strsz))
            ++nsym;

    // copy the relevant symbols' names
    symbols = malloc((nsym + 1) * sizeof(char *));
    nsym = 0;
    for(symbol = psymtab + 1; symbol && symbol->st_info != STN_UNDEF; ++symbol)
        if (is_public_fcn(symbol, strsz))
            symbols[nsym++] = strdup((const char *)pstrtab + symbol->st_name);

    // "return" the number of symbols
    if (count) *count = nsym;

    // set the last symbol entry as NULL to simplify iteration and return
    symbols[nsym] = NULL;
    return symbols;
}
