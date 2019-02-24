#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>

#include "../library_platform.h"
#include "../library_t.h"
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include <string.h>
#include <stdlib.h>

#if defined(__LP64__)
    typedef struct mach_header_64 mach_header_t;
    typedef struct segment_command_64 segment_command_t;
#else
    typedef struct mach_header mach_header_t;
    typedef struct segment_command segment_command_t;
#endif

typedef int (*cmd_enum_callback)(struct load_command *, const uint8_t *pcmd, void *);

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

static const mach_header_t *get_lib_head(const so_handle so)
{
    uint32_t i = get_lib_index(so);

    if (i == (uint32_t)-1) return 0;

    return (const mach_header_t *)_dyld_get_image_header(i);
}

static void enumerate_commands(const mach_header_t *head, cmd_enum_callback cb, void *ctx)
{
    struct load_command load_cmd;
    const uint8_t *pload_cmd;

    // point to the first cmd
    pload_cmd = (const uint8_t *)head + sizeof(mach_header_t);

    // loop through the commands and call the callback for each
    for (uint32_t i = 0; i < head->ncmds; ++i, pload_cmd += load_cmd.cmdsize)
    {
        memcpy(&load_cmd, pload_cmd, sizeof(struct load_command));
        if (cb(&load_cmd, pload_cmd, ctx)) return;
    }
}

static int find_id_dylib(struct load_command *cmd, const uint8_t *pcmd, const uint8_t **pdylib)
{
    if (cmd->cmd == LC_ID_DYLIB)
    {
        *pdylib = pcmd;
        return 1;
    }

    return 0;
}

const char *lib_extension()
{
    return ".dylib";
}

size_t lib_soname(char *dst, size_t count, const so_handle so)
{
    const mach_header_t *head = get_lib_head(so);
    const uint8_t *pdylib = 0;
    struct dylib_command dylib;

    // can't find "so", return early
    if (!head) return 0;

    // find the lib ident; if not, return early
    enumerate_commands(head, (cmd_enum_callback)find_id_dylib, &pdylib);
    if (!pdylib) return 0;

    // copy out the dylib_command struct and then string
    memcpy(&dylib, pdylib, sizeof(struct dylib_command));
    strncpy(dst, (const char *)(pdylib + dylib.dylib.name.offset), count);
    if (count) dst[count - 1] = '\0'; // mark the last byte as null, in case the string didn't fit
    return strlen((const char *)(pdylib + dylib.dylib.name.offset)) + 1;
}

static int find_symbol_segments(struct load_command *cmd, const uint8_t *pcmd, const uint8_t **segs[3])
{
    segment_command_t segtab;

    if (cmd->cmd == LC_SYMTAB)
        *segs[0] = pcmd;
    else if (cmd->cmd == LC_SEGMENT || cmd->cmd == LC_SEGMENT_64)
    {
        memcpy(&segtab, pcmd, sizeof(segment_command_t));
        if (!strcmp(segtab.segname, SEG_TEXT))
            *segs[1] = pcmd;
        else if (!strcmp(segtab.segname, SEG_LINKEDIT))
            *segs[2] = pcmd;
    }

    if (*segs[0] && *segs[1] && *segs[2]) return 1;

    return 0;
}

char **lib_symbols(const so_handle so, size_t *count)
{
    char **symbols = NULL;
    const mach_header_t *head = get_lib_head(so);
    size_t i, nsym = 0;
    const uint8_t *psymtab = 0, *psegtext = 0, *pseglink = 0;
    const uint8_t **psegs[3] = { &psymtab, &psegtext, &pseglink };
    struct symtab_command symtab;
    segment_command_t segtext, seglink;
    uintptr_t phead, offset, strs;
    struct nlist_64 *sym;

    // can't find "so", return early
    if (!head) return symbols;

    // find all the segments needed to parse symbol names
    enumerate_commands(head, (cmd_enum_callback)find_symbol_segments, psegs);

    // if we don't have all we need, early out
    if (!psymtab || !psegtext || !pseglink) return symbols;

    // copy the tables and segments
    memcpy(&symtab,  psymtab,  sizeof(struct symtab_command));
    memcpy(&segtext, psegtext, sizeof(segment_command_t));
    memcpy(&seglink, pseglink, sizeof(segment_command_t));

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
