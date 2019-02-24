project(example)
include(cmake/add_example.cmake)

set(LIB glug_library)
find_package(${LIB} REQUIRED)
set(GLUG_TARGET_NAME ${LIB}_test)
set(GLUG_TARGET_NAME++ ${LIB}_test++)

set(INSTALL_PATH example)

add_example(
    TARGET_NAME
        ${GLUG_TARGET_NAME}
    SRCS
        main.c
    LINK_LIB
        ${LIB}
    INSTALL_PATH
        ${INSTALL_PATH}
)

add_example(
    TARGET_NAME
        ${GLUG_TARGET_NAME++}
    SRCS
        main.cpp
    LINK_LIB
        ${LIB}
    INSTALL_PATH
        ${INSTALL_PATH}
)

# create of test libs to copy to the build dir
list(
    APPEND
    TEST_LIBS
    hello.dll
    hello.dylib
    hello.so
)

# copy the test libs to the build dir
foreach(TEST_LIB ${TEST_LIBS})
    add_custom_command(
        TARGET
            ${GLUG_TARGET_NAME} POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_LIB} ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach()
