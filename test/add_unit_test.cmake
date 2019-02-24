function(add_unit_test)
    set(SINGLE_VALS TARGETNAME)
    set(MULTI_VALS SOURCES INCLUDE_DIRS LINK_LIBS)
    cmake_parse_arguments(UT "" "${SINGLE_VALS}" "${MULTI_VALS}" "${ARGN}")

    add_executable(
        ${UT_TARGETNAME}
        ${UT_SOURCES}
    )

    target_compile_definitions(
        ${UT_TARGETNAME}
        PRIVATE
            _CRT_SECURE_NO_WARNINGS # clear the secure warnings for things like strcat
    )

    target_include_directories(
        ${UT_TARGETNAME}
        PRIVATE
            ${UT_INCLUDE_DIRS}
    )

    target_link_libraries(
        ${UT_TARGETNAME}
        ${UT_LINK_LIBS}
    )

    target_compile_definitions(
        ${UT_TARGETNAME}
        PRIVATE
            GLUG_LIB_LOCAL=
            GLUG_LIB_API=
    )
    add_test(${UT_TARGETNAME} ${UT_TARGETNAME})
endfunction()
