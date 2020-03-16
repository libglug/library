cmake_minimum_required(VERSION 3.0)

set(VER_MAJOR 1)
set(VER_MINOR 0)
set(VER_PATCH 0)
project(glug_library VERSION ${VER_MAJOR}.${VER_MINOR}.${VER_PATCH})
include(cmake/add_gluglib.cmake)

option(BUILD_STATIC "Build as a static library?" OFF)

set(SRC_ROOT src)
set(INC_ROOT include/glug)

list(
    APPEND
    COMMON_SOURCE
    ${INC_ROOT}/library/func_t.h
    ${INC_ROOT}/library/handle.h
    ${INC_ROOT}/library/library.h
    ${INC_ROOT}/library/library_t.h
    ${SRC_ROOT}/library_t.h
    ${SRC_ROOT}/library.c
    ${SRC_ROOT}/library_platform.h
)

# Windows sources
list(
    APPEND
    WIN32_SOURCE
    ${COMMON_SOURCE}
    ${SRC_ROOT}/win32/library.c
)

# Mac OS sources
list(
    APPEND
    MACOS_SOURCE
    ${COMMON_SOURCE}
    ${SRC_ROOT}/nix/library.c
    ${SRC_ROOT}/macos/library.c
)

# linux sources
list(
    APPEND
    LINUX_SOURCE
    ${COMMON_SOURCE}
    ${SRC_ROOT}/nix/library.c
    ${SRC_ROOT}/linux/library.c
)

add_gluglib(
    TARGET_NAME  ${PROJECT_NAME}
    STATIC_BUILD ${BUILD_STATIC}
    WIN32_SOURCE ${WIN32_SOURCE}
    MACOS_SOURCE ${MACOS_SOURCE}
    LINUX_SOURCE ${LINUX_SOURCE}
    WIN32_LIBS
    MACOS_LIBS dl
    LINUX_LIBS dl
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common_headers/include>
    INTERFACE
        $<INSTALL_INTERFACE:include>
)

# install the library
install(
    TARGETS
        ${PROJECT_NAME}
    EXPORT
        Find${PROJECT_NAME}
    DESTINATION
        lib
)

#install the lib's headers
install(
    DIRECTORY
        include/
    DESTINATION
        include
)

# install the common_headers
install(
    DIRECTORY
        common_headers/include/
    DESTINATION
        include
    FILES_MATCHING PATTERN
        "*.h"
)

# export the package to be included in other projects
export(PACKAGE ${PROJECT_NAME})
export(
    TARGETS
        ${PROJECT_NAME}
    FILE
        ${PROJECT_NAME}-config.cmake
)

# install export file
install(
    EXPORT
        Find${PROJECT_NAME}
    DESTINATION
        cmake
)
