include(${PROJECT_SOURCE_DIR}/cmake/compile.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

set(LIBR_SRC_ROOT ${GLUG_LIBR_ROOT}/src)
set(LIBR_INC_ROOT ${LIBR_INCLUDE_ROOT}/glug)

set(LIBR_HEADERS
    ${LIBR_INC_ROOT}/library/library.h
    ${LIBR_INC_ROOT}/library/func_def.h
)

# Windows sources

set(WIN32_LIBR_ROOT ${LIBR_SRC_ROOT}/win32)
set(WIN32_SOURCES
    ${WIN32_LIBR_ROOT}/library.cpp
)
set_compilation_props("${WIN32_SOURCES}" GLUG_OS_WIN32)

# Mac OS sources

set(MACOS_LIBR_ROOT ${DISP_SRC_ROOT}/macos)
set(MACOS_SOURCES
)
set_compilation_props("${MACOS_SOURCES}" GLUG_OS_MACOS)

# X11 sources

set(X11_LIBR_ROOT ${DISP_SRC_ROOT}/x11)
set(X11_SOURCES
)
set_compilation_props("${X11_SOURCES}" GLUG_OS_LINUX)

if(${GLUG_OS_WIN32})

    add_win32_exports()

elseif(${GLUG_OS_MACOS})

    add_linux_exports()

elseif(${GLUG_OS_LINUX})

    add_linux_exports()

endif()

add_library(
    glug_library
    STATIC
    ${LIBR_HEADERS}
    ${WIN32_SOURCES}
    ${MACOS_SOURCES}
    ${X11_SOURCES}
)
