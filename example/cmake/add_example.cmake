# create simple example with simple dependencies
function (add_example)
    set(OPTIONS USE_WINMAIN)
    set(SINGLE_VALS TARGET_NAME LINK_LIB INSTALL_PATH)
    set(MULTI_VALS SRCS)
    cmake_parse_arguments(GLUG "${OPTIONS}" "${SINGLE_VALS}" "${MULTI_VALS}" ${ARGN})

    if (${GLUG_USE_WINMAIN})
        set(PLATFORM WIN32)
    endif()

    add_executable(${GLUG_TARGET_NAME} ${PLATFORM} ${GLUG_SRCS})
    target_link_libraries(${GLUG_TARGET_NAME} ${GLUG_LINK_LIB})

    set_target_properties(
        ${GLUG_TARGET_NAME}
        PROPERTIES
            INSTALL_RPATH "$ORIGIN;@loader_path"
            BUILD_WITH_INSTALL_RPATH TRUE
    )

endfunction()
