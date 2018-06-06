include(${PROJECT_SOURCE_DIR}/cmake/compile.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

set(LIBR_SRC_ROOT ${GLUG_LIBR_ROOT}/src)
set(LIBR_INC_ROOT ${LIBR_INCLUDE_ROOT}/glug)

set(LIBR_HEADERS
    ${LIBR_INC_ROOT}/library/handle.h
    ${LIBR_INC_ROOT}/library/library.h
    ${LIBR_INC_ROOT}/library/typedef_func.h
)

set(LIBR_SOURCES
    ${LIBR_SRC_ROOT}/library.h
)

# Windows sources

set(WIN32_LIBR_ROOT ${LIBR_SRC_ROOT}/win32)
set(WIN32_SOURCES
    ${WIN32_LIBR_ROOT}/library.cpp
)

# Unix-like sources

set(UNIX_LIBR_ROOT ${LIBR_SRC_ROOT}/unix)
set(UNIX_SOURCES
    ${UNIX_LIBR_ROOT}/library.cpp
)

# change compilation rules based on target os
set_compilation_props("${WIN32_SOURCES}" GLUG_OS_WIN32)
set_compilation_props("${UNIX_SOURCES}" GLUG_OS_UNIXLIKE)

if(${GLUG_OS_WIN32})

    add_win32_exports()

else()

    add_linux_exports()
    set(EXT_LIBS dl)

endif()

add_library(
    ${CMAKE_PROJECT_NAME}
    STATIC
    ${LIBR_HEADERS}
    ${LIBR_SOURCES}
    ${WIN32_SOURCES}
    ${UNIX_SOURCES}
)

if (DEFINED EXT_LIBS)

    target_link_libraries(${CMAKE_PROJECT_NAME} ${EXT_LIBS})

endif()

install(
    TARGETS ${CMAKE_PROJECT_NAME}
    DESTINATION ${GLUG_DIST_DIR}/lib
)

install(
    DIRECTORY ${LIBR_INC_ROOT}
    DESTINATION ${GLUG_DIST_DIR}/include
)
