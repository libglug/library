set(TEST_PROJ_NAME "lib_test")

add_executable(${TEST_PROJ_NAME} main.c)
target_link_libraries(${TEST_PROJ_NAME} ${CMAKE_PROJECT_NAME})

install(
    TARGETS ${TEST_PROJ_NAME}
    RUNTIME DESTINATION ${GLUG_DIST_DIR}
)

if (${GLUG_OS_WIN32})

    set(EXT_LIB "hello.dll")

elseif(${GLUG_OS_MACOS})

elseif(${GLUG_OS_LINUX})

    set(EXT_LIB "./libhello.so" "./hello")

endif()

install(
    FILES ${EXT_LIB}
    DESTINATION ${GLUG_DIST_DIR}
)
