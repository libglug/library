# find and link to each library for TARGET
function (link_libs TARGET LIBS)
    foreach(LIB ${LIBS})
        find_library(FOUND_LIB ${LIB})
        if (NOT FOUND_LIB)
            message(FATAL_ERROR "${LIB} not found")
        endif()
        list(
            APPEND
            FOUND_LIBS
            ${FOUND_LIB}
        )
        unset(FOUND_LIB CACHE)
    endforeach()
    target_link_librarIES(
        ${TARGET}
        ${FOUND_LIBS}
    )
endfunction()
