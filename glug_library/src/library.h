#include <glug/library/handle.h>

namespace glug
{

struct library
{
    const char *name;
    bool loaded;
    so_handle dl;
};

} // namespace glug
