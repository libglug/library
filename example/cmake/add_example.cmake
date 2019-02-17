# create simple example with simple dependencies
function (add_example)
    set(OPTIONS USE_WINMAIN USE_WHOLE_ARCHIVE)
    set(SINGLE_VALS TARGET_NAME LINK_LIB)
    set(MULTI_VALS SRCS)
    cmake_parse_arguments(GLUG "${OPTIONS}" "${SINGLE_VALS}" "${MULTI_VALS}" ${ARGN})

    if (${GLUG_USE_WINMAIN})
        set(PLATFORM WIN32)
    endif()

    if (${GLUG_USE_WHOLE_ARCHIVE})
        set(GLUG_LINK_LIB -Wl,--whole-archive ${GLUG_LINK_LIB} -Wl,--no-whole-archive)
    endif()

    set(GLUG_XMPL_NAME ${GLUG_TARGET_NAME})

    add_executable(${GLUG_XMPL_NAME} ${PLATFORM} ${GLUG_SRCS})
    target_link_libraries(${GLUG_XMPL_NAME} ${GLUG_LINK_LIB})

    set_target_properties(
        ${GLUG_XMPL_NAME}
        PROPERTIES
            INSTALL_RPATH "$ORIGIN"
            BUILD_WITH_INSTALL_RPATH TRUE
    )

endfunction()
