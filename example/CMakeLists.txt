set(TEST_PROJ_NAME "lib_test")

add_executable(${TEST_PROJ_NAME} main.c)
target_link_libraries(${TEST_PROJ_NAME} ${CMAKE_PROJECT_NAME})

install(
    TARGETS ${TEST_PROJ_NAME}
    RUNTIME DESTINATION ${GLUG_DIST_DIR}
)
