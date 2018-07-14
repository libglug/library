set(TEST_PROJ_NAME "lib_test")

add_executable(${TEST_PROJ_NAME} main.c)
link_directories(${CMAKE_BINARY_DIR}/dist/lib)
target_link_libraries(${TEST_PROJ_NAME} ${CMAKE_PROJECT_NAME})

if (${GLUG_OS_WIN32})

    set(EXT_LIB "hello.dll")

elseif(${GLUG_OS_MACOS})

    set(EXT_LIB "hello.dylib")

elseif(${GLUG_OS_LINUX})

    set(EXT_LIB "hello.so")

endif()

install(
    FILES ${EXT_LIB}
    DESTINATION ${GLUG_XMPL_DIR}
)
