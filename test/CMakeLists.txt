add_subdirectory(CUnit/CUnit EXCLUDE_FROM_ALL)
include(add_int_test.cmake)
include(add_unit_test.cmake)

set(SRC_ROOT ../glug_library)

list(
    APPEND
    INCLUDE_DIRS
    CUnit/CUnit/include
    ${SRC_ROOT}/common_headers/include
    ${SRC_ROOT}/include
)

# integration test
set(TESTED_LIB glug_library)
find_package(${TESTED_LIB} REQUIRED)
add_int_test(
    TARGETNAME
        integration
    SOURCES
        integration/main.c
        create_suite.h
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    LINK_LIBS
        CUnit
        ${TESTED_LIB}
)

# unit tests
set(MOCK_DIR unit/mocks)
list(
    APPEND
    INCLUDE_DIRS
    ${SRC_ROOT}/src
)
list(
    APPEND
    MOCKS
    ${MOCK_DIR}/libs.mock.c
    ${MOCK_DIR}/libs.mock.h
    ${MOCK_DIR}/extension.mock.c
    ${MOCK_DIR}/load.mock.c
    ${MOCK_DIR}/soname.mock.c
    ${MOCK_DIR}/symbols.mock.c
)
add_unit_test(
    TARGETNAME
        unit-makefilename
    SOURCES
        unit/makefilename.c
        ${SRC_ROOT}/src/library.c
        create_suite.h
        ${MOCKS}
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    LINK_LIBS
        CUnit
)

add_unit_test(
    TARGETNAME
        unit-loadlibs
    SOURCES
        unit/loading.c
        ${SRC_ROOT}/src/library.c
        create_suite.h
        ${MOCKS}
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    LINK_LIBS
        CUnit
)

add_unit_test(
    TARGETNAME
        unit-symbols
    SOURCES
        unit/symbols.c
        ${SRC_ROOT}/src/library.c
        create_suite.h
        ${MOCKS}
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    LINK_LIBS
        CUnit
)

# create a custom target which builds tests and their dependencies
# (bug in CMake? (https://gitlab.kitware.com/cmake/cmake/issues/8774))
add_custom_target(
    check ALL
    DEPENDS
        # add other tests executables here
        CUnit integration unit-makefilename unit-loadlibs unit-symbols ${TESTED_LIB}
)

# copy the CUnit library to the same directory
add_custom_command(
    TARGET
        check POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:CUnit> ${CMAKE_CURRENT_BINARY_DIR}
)

# copy the libraries used in the integration tests
list(
    APPEND
    TEST_LIBS
    integration/hello.dll
    integration/hello.dylib
    integration/hello.so
)

foreach(TEST_LIB ${TEST_LIBS})
    add_custom_command(
        TARGET
            check POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_LIB} ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach()
