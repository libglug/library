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
cmake [-G"<Generator>"] [-DBUILD_STATIC=ON] <path/to/repo>
cmake --build . --target glug_library
```

#### Building and Running Tests
```
mkdir <build/dir> && cd <build/dir>
cmake [-G"<Generator>"] -DBUILD_TESTS <path/to/repo>
cmake --build . --target check
ctest [-V]
```

#### Building (library, example, and tests)
```
mkdir <build/dir> && cd <build/dir>
cmake [-G"<Generator>"] [options] <path/to/repo>
cmake --build .

```
##### Build Options
| Option         | Description               | Default |
|----------------|---------------------------|---------|
| -DBUILD_STATIC | Build as a static library | OFF     |
| -DBUILD_TESTS  | Build tests               | OFF     |
| -DBUILD_XMPLS  | Build the examples        | OFF     |

### Installing (library only)
```
mkdir <build/dir> && cd <build/dir>
cmake [-G"<Generator>"] [-DBUILD_STATIC=ON] <path/to/repo>
cmake --build . --target install
```
