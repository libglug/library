cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0054 NEW)
project(glug_library_suite)

set(GLUG_LIB_TARGET glug_library)
option(BUILD_XMPLS "Build the examples?")
option(BUILD_TESTS "Build tests?")

# disable find_package in any subproject for finding this lib
macro(find_package)
    if (NOT "${ARGV0}" STREQUAL ${GLUG_LIB_TARGET})
        _find_package(${ARGV})
    endif()
endmacro()

# add the library subproject
add_subdirectory(${GLUG_LIB_TARGET})

# build and install examples
if (${BUILD_XMPLS} STREQUAL "ON")
    add_subdirectory(example)

    # copy the glug lib to the example build dir if it's shared
    if (NOT ${BUILD_STATIC})
        add_custom_command(
            OUTPUT
                copy_lib POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E copy
                    $<TARGET_FILE:${GLUG_LIB_TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/example
            DEPENDS
                ${GLUG_LIB_TARGET}
        )
        add_custom_target(
            copy_lib_example
            ALL
            DEPENDS
                copy_lib
        )
    endif()

endif()


if (${BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()
