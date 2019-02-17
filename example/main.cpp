#include <glug/library/library.h>
#include <glug/library/typedef_func.h>

#include <iostream>

using namespace std;

typedef_func(say, void, const char *);

int main()
{
    const char *lib_name = "hello";
    size_t full_len = glug_lib_make_filename(nullptr, lib_name, 0);

    char *full_name = new char[full_len]();
    glug_lib_make_filename(full_name, lib_name, full_len);

    bool is_lib = glug_lib_exists(full_name);
    cout << full_name << ' ' << (is_lib ? "exists" : "doesn't exist") << endl;

    if (is_lib)
    {
        size_t symbols = 0;
        struct glug_library_t *libhello = glug_lib_lazy(full_name);
        char **psym, **hello_symbols = glug_lib_symbols(libhello, &symbols);

        // get lib name length and malloc big enough buffer (w/ trailing NULL)
        size_t namelen = glug_lib_soname(nullptr, 0, libhello);
        char *lib_soname = new char[namelen]();
        glug_lib_soname(lib_soname, namelen, libhello);
        cout << "lib so name: " << lib_soname << endl;;

        cout << symbols << " symbol(s):" << endl;
        for (psym = hello_symbols; *psym; ++psym)
            cout << "  " << *psym << endl;

        say say_hello = reinterpret_cast<say>(glug_lib_proc(libhello, "say_hello"));
        if (say_hello)
            say_hello("glub_lib");

        delete[] lib_soname;
        hello_symbols = glug_lib_free_symbols(hello_symbols);
        glug_lib_free(libhello);
    }

    delete[] full_name;
    return 0;
}
