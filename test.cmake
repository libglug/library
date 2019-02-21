enable_testing()
add_subdirectory(test/CUnit/CUnit EXCLUDE_FROM_ALL)
include(test/add_unit_test.cmake)

# integration test
set(TESTED_LIB glug_library)
find_package(${TESTED_LIB} REQUIRED)
add_executable(
    integration
    test/integration/main.c
)
target_include_directories(
    integration
    PRIVATE
        test/CUnit/CUnit/include
        glug_library/common_headers/include
        glug_library/include
)

target_link_libraries(
    integration
    CUnit
    ${TESTED_LIB}
)
add_test(integration integration)

# unit tests
set(MOCK_DIR test/unit/mocks)
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
    TARGET
        unit-makefilename
    SOURCES
        test/unit/makefilename.c
        glug_library/src/library.c
        ${MOCKS}
)

add_unit_test(
    TARGET
        unit-loadlibs
    SOURCES
        test/unit/loading.c
        glug_library/src/library.c
        ${MOCKS}
)

add_unit_test(
    TARGET
        unit-symbols
    SOURCES
        test/unit/symbols.c
        glug_library/src/library.c
        ${MOCKS}
)

# create a custom target which builds tests and their dependencies
# (bug in CMake? (https://gitlab.kitware.com/cmake/cmake/issues/8774))
add_custom_target(
    check
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
    test/integration/hello.dll
    test/integration/hello.dylib
    test/integration/hello.so
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
