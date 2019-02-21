![](https://ci.appveyor.com/api/projects/status/github/libglug/library?branch=master&svg=true)
# library
Shared object loading library

### Building

#### Fetching Dependencies
```
git submodule update --init --recursive
```

#### Building (library only)
```
mkdir <build/dir> && cd <build/dir>
cmake [-G"<Generator>"] <path/to/repo>
cmake --build . --target glug_library
```

#### Building (library, example, and tests)
```
mkdir <build/dir> && cd <build/dir>
cmake [-G"<Generator>"] <path/to/repo>
cmake --build .

```

#### Building and Running Tests
```
mkdir <build/dir> && cd <build/dir>
cmake [-G"<Generator>"] <path/to/repo>
cmake --build . --target check
ctest [-V]
```
