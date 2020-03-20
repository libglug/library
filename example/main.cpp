#include <glug/library/library.h>
#include <glug/library/func_t.h>

#include <cstdlib>
#include <iostream>

using namespace std;

glug_func_t(say, void, const char *);

int main()
{
    std::string full_name;
    full_name += "hello";
    full_name += glug_lib_extension();

    bool is_lib = glug_lib_exists(full_name.c_str());
    cout << full_name << ' ' << (is_lib ? "exists" : "doesn't exist") << endl;

    if (is_lib)
    {
        size_t symbols = 0;
        struct glug_library *libhello;
        glug_lib_lazy(&libhello, full_name.c_str());
        char **psym, **hello_symbols = glug_lib_symbols(libhello, (alloc_t)malloc);

        // get lib name length and malloc big enough buffer (w/ trailing NULL)
        size_t namelen = glug_lib_soname(libhello, nullptr, 0);
        char *lib_soname = new char[namelen]();
        glug_lib_soname(libhello, lib_soname, namelen);
        cout << "lib so name: " << lib_soname << endl;;

        cout << symbols << " symbol(s):" << endl;
        for (psym = hello_symbols; *psym; ++psym)
            cout << "  " << *psym << endl;

        say say_hello = reinterpret_cast<say>(glug_lib_proc(libhello, "say_hello"));
        if (say_hello)
            say_hello("glub_lib");

        delete[] lib_soname;
        free(hello_symbols);
        glug_lib_free(&libhello);
    }

    return 0;
}
