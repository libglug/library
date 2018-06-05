project(glug_library)
cmake_minimum_required(VERSION 2.8)

include(cmake/os.cmake)

set(GLUG_LIBR_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME})
set(LIBR_INCLUDE_ROOT ${GLUG_LIBR_ROOT}/include)
include_directories(${LIBR_INCLUDE_ROOT})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common_headers/include)

set(GLUG_DIST_DIR ${CMAKE_BINARY_DIR}/dist)

add_subdirectory(glug_library/src)
add_subdirectory(example)
