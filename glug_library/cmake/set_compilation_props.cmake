# toggle source compilation
function (set_compilation_props SOURCES_LIST SHOULD_COMPILE)
    if (${SHOULD_COMPILE})
        set(DONT_COMPILE FALSE)
    else()
        set(DONT_COMPILE TRUE)
    endif()

    foreach(FILE ${SOURCES_LIST})
        set_source_files_properties(${FILE} PROPERTIES HEADER_FILE_ONLY ${DONT_COMPILE})
    endforeach()

endfunction()
