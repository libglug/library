#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>
#include <glug/library/library.h>
#include <glug/library/handle.h>

#include "../library.h"

#include <cstring>
#include <vector>

namespace glug
{

const char *lib_extension = ".dylib";

#if defined(__LP64__)
    using mach_header_t = mach_header_64;
    using segment_command_t = segment_command_64;
#else
    using mach_header_t = mach_header;
    using segment_command_t = segment_command;
#endif

static uint32_t get_lib_index(const struct library * lib)
{
    for (int i = _dyld_image_count(); i--;)
    {
        void *dl = dlopen(_dyld_get_image_name(i), RTLD_NOW);
        dlclose(dl);
        if (dl == lib->dl)
            return i;
    }

    return -1;
}

char **lib_symbols(const struct library *lib, size_t *count)
{
    std::vector<char *> symbol_list;
    uint32_t i = get_lib_index(lib);
    if (count) *count = 0;
    if (i == -1)
        return nullptr;

    const mach_header_t *head = reinterpret_cast<const mach_header_t *>(_dyld_get_image_header(i));
    intptr_t pload_cmd = reinterpret_cast<intptr_t>(head) + sizeof(mach_header_t);

    const symtab_command *symtab = nullptr;
    const segment_command_t *segtext = nullptr, *seglink = nullptr;
    const load_command *load_cmd;
    for (int i = 0; i < head->ncmds; ++i, pload_cmd += load_cmd->cmdsize)
    {
        load_cmd = reinterpret_cast<const load_command *>(pload_cmd);
        switch (load_cmd->cmd)
        {
        case LC_SYMTAB:
            symtab = reinterpret_cast<const symtab_command *>(load_cmd);
            break;
        case LC_SEGMENT:
        case LC_SEGMENT_64:
            const segment_command_t *segtab = reinterpret_cast<const segment_command_t *>(load_cmd);
            if (!strcmp(segtab->segname, SEG_TEXT))
                segtext = segtab;
            else if (!strcmp(segtab->segname, SEG_LINKEDIT))
                seglink = segtab;
            break;
        }

    }

    if (!symtab || !segtext || !seglink)
        return nullptr;

    intptr_t phead = reinterpret_cast<intptr_t>(head);
    intptr_t offset = seglink->vmaddr - segtext->vmaddr - seglink->fileoff;
    intptr_t strs = phead + symtab->stroff + offset;
    nlist_64 *sym = reinterpret_cast<nlist_64 *>(phead + symtab->symoff + offset);

    for (int i = 0; i < symtab->nsyms; ++i, ++sym)
        if ((sym->n_type & N_EXT) == N_EXT && sym->n_value)
            symbol_list.push_back(reinterpret_cast<char *>(strs + sym->n_un.n_strx + 1));

    size_t list_len = symbol_list.size();
    char **symbols = new char*[list_len + 1]();
    for (int i = 0; i < list_len; ++i)
        symbols[i] = strdup(symbol_list[i]);
    symbols[list_len] = nullptr;

    if (count) *count = symbol_list.size();

    return symbols;
}

char **free_lib_symbols(char **symbol_list)
{
    if (symbol_list)
    {
        for (char **psym = symbol_list; *psym; ++psym)
            delete *psym;

        delete symbol_list;
    }

    return nullptr;
}

} // namespace glug
