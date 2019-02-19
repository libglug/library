function(add_unit_test)
    set(SINGLE_VALS TARGET)
    set(MULTI_VALS SOURCES)
    cmake_parse_arguments(UT "" "${SINGLE_VALS}" "${MULTI_VALS}" "${ARGN}")

    add_executable(
        ${UT_TARGET}
        ${UT_SOURCES}
    )
    target_compile_definitions(
        ${UT_TARGET}
        PRIVATE
            _CRT_SECURE_NO_WARNINGS # clear the secure warnings for things like strcat
    )
    target_include_directories(
        ${UT_TARGET}
        PRIVATE
            test/CUnit/CUnit/include
            glug_library/include
            glug_library/src
            glug_library/common_headers/include
    )
    target_link_libraries(
        ${UT_TARGET}
        CUnit
    )
    add_test(${UT_TARGET} ${UT_TARGET})
endfunction()
