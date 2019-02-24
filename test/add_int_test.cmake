function(add_int_test)
    set(SINGLE_VALS TARGETNAME)
    set(MULTI_VALS SOURCES INCLUDE_DIRS LINK_LIBS)
    cmake_parse_arguments(IT "" "${SINGLE_VALS}" "${MULTI_VALS}" "${ARGN}")

    add_executable(
        ${IT_TARGETNAME}
        ${IT_SOURCES}
    )
    target_compile_definitions(
        ${IT_TARGETNAME}
        PRIVATE
            _CRT_SECURE_NO_WARNINGS # clear the secure warnings for things like strcat
    )

    target_include_directories(
        ${IT_TARGETNAME}
        PRIVATE
            CUnit/CUnit/include
            ${IT_INCLUDE_DIRS}
    )

    target_link_libraries(
        ${IT_TARGETNAME}
        ${IT_LINK_LIBS}
    )

    set_target_properties(
        ${IT_TARGETNAME}
        PROPERTIES
            INSTALL_RPATH "$ORIGIN;@loader_path"
            BUILD_WITH_INSTALL_RPATH TRUE
    )
    add_test(${IT_TARGETNAME} ${IT_TARGETNAME})
endfunction()
