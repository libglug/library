set(BIN_PROJ_NAME "lib_test")

add_executable(${BIN_PROJ_NAME} main.c)
target_link_libraries(${BIN_PROJ_NAME} ${CMAKE_PROJECT_NAME})
