#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>

#include "../library_platform.h"
#include "../library_t.h"
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include <string.h>
#include <stdlib.h>

const char *lib_extension = ".dylib";

#if defined(__LP64__)
    typedef struct mach_header_64 mach_header_t;
    typedef struct segment_command_64 segment_command_t;
#else
    typedef struct mach_header mach_header_t;
    typedef struct segment_command segment_command_t;
#endif

static uint32_t get_lib_index(const so_handle so)
{
    for (uint32_t i = _dyld_image_count(); i--;)
    {
        so_handle dl = dlopen(_dyld_get_image_name(i), RTLD_NOW);
        dlclose(dl);
        if (dl == so)
            return i;
    }

    return (uint32_t)-1;
}

char **lib_symbols(const so_handle so, size_t *count)
{
    char **symbols = NULL;
    uint32_t i = get_lib_index(so);
    const mach_header_t *head;
    uintptr_t pload_cmd;
    uintptr_t psymtab = 0, psegtext = 0, pseglink = 0;
    const struct load_command *load_cmd;
    struct symtab_command symtab;
    segment_command_t segtext, seglink;
    uintptr_t phead, offset, strs;
    struct nlist_64 *sym;
    size_t nsym = 0;

    // can't find "so", return early
    if (i == (uint32_t)-1) return symbols;

    head = (const mach_header_t *)_dyld_get_image_header(i);
    pload_cmd = (uintptr_t)head + sizeof(mach_header_t);

    // loop and find the symbol table, text segment, link edit segment pointers
    for (uint32_t i = 0; i < head->ncmds; ++i, pload_cmd += load_cmd->cmdsize)
    {
        load_cmd = (const struct load_command *)pload_cmd;
        switch (load_cmd->cmd)
        {
        case LC_SYMTAB:
            psymtab = (uintptr_t)load_cmd;
            break;
        case LC_SEGMENT:
        case LC_SEGMENT_64:
        {
            segment_command_t segtab;
            memcpy(&segtab, load_cmd, sizeof(segment_command_t));
            if (!strcmp(segtab.segname, SEG_TEXT))
                psegtext = (uintptr_t)load_cmd;
            else if (!strcmp(segtab.segname, SEG_LINKEDIT))
                pseglink = (uintptr_t)load_cmd;
            break;
        }
        }

    }

    // if we don't have all we need, early out
    if (!psymtab || !psegtext || !pseglink) return symbols;

    // copy the tables and segments
    memcpy(&symtab,  (void *)psymtab, sizeof(struct symtab_command));
    memcpy(&segtext, (void *)psegtext, sizeof(segment_command_t));
    memcpy(&seglink, (void *)pseglink, sizeof(segment_command_t));

    // set up some helper pointers
    phead = (uintptr_t)head;
    offset = (uintptr_t)(seglink.vmaddr - segtext.vmaddr - seglink.fileoff);
    strs = phead + symtab.stroff + offset;

    // count the exported symbols
    sym = (struct nlist_64 *)(phead + symtab.symoff + offset);
    for (i = 0; i < symtab.nsyms; ++i, ++sym)
        if ((sym->n_type & N_EXT) == N_EXT && sym->n_value)
             ++nsym;

    // copy strings into the symbols array
    symbols = realloc(symbols, (nsym + 1) * sizeof(char *));
    nsym = 0;
    sym = (struct nlist_64 *)(phead + symtab.symoff + offset);
    for (i = 0; i < symtab.nsyms; ++i, ++sym)
        if ((sym->n_type & N_EXT) == N_EXT && sym->n_value)
            symbols[nsym++] = strdup((char *)(strs + sym->n_un.n_strx + 1));

    // "return" the number of symbols
    if (count) *count = nsym;

    // set the last symbol entry as NULL to simplify iteration and return
    symbols[nsym] = NULL;
    return symbols;
}
