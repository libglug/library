include(${PROJECT_SOURCE_DIR}/cmake/compile.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

set(LIBR_SRC_ROOT ${GLUG_LIBR_ROOT}/src)
set(LIBR_INC_ROOT ${LIBR_INCLUDE_ROOT}/glug)

set(LIBR_HEADERS
    ${LIBR_INC_ROOT}/library/handle.h
    ${LIBR_INC_ROOT}/library/library.h
    ${LIBR_INC_ROOT}/library/func_def.h
)

# Windows sources

set(WIN32_LIBR_ROOT ${LIBR_SRC_ROOT}/win32)
set(WIN32_SOURCES
    ${WIN32_LIBR_ROOT}/library.cpp
)
set_compilation_props("${WIN32_SOURCES}" GLUG_OS_WIN32)

# Unix-like sources

set(UNIX_LIBR_ROOT ${LIBR_SRC_ROOT}/unix)
set(UNIX_SOURCES
)
set_compilation_props("${UNIX_SOURCES}" GLUG_OS_MACOS)
set_compilation_props("${UNIX_SOURCES}" GLUG_OS_LINUX)

if(${GLUG_OS_WIN32})

    add_win32_exports()

else()

    add_linux_exports()

endif()

add_library(
    glug_library
    STATIC
    ${LIBR_HEADERS}
    ${WIN32_SOURCES}
    ${UNIX_SOURCES}
)
