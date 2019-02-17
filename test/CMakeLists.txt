project(glug_library_tests VERSION 1.0)
add_subdirectory(cunit/CUnit)

set(TESTED_LIB glug_library)

add_executable(
    ${PROJECT_NAME}
    # add test files here
)

find_package(${TESTED_LIB} REQUIRED)
target_link_libraries(
    ${PROJECT_NAME}
    CUnit
    ${PROJECT_NAME}
)

# copy the executable to this build directory (MSVC likes to hide it in Debug/Release folders)
add_custom_command(
    TARGET
        ${PROJECT_NAME} POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}
)

# copy the CUnit library to the same directory
add_custom_command(
    TARGET
        ${PROJECT_NAME} POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:CUnit> ${CMAKE_CURRENT_BINARY_DIR}
)

# also copy the library to the same directory
add_custom_command(
    TARGET
        ${PROJECT_NAME} POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${TESTED_LIB}> ${CMAKE_CURRENT_BINARY_DIR}
)
