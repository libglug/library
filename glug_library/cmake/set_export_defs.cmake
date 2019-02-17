# set symbol export defs
function (set_export_defs TARGET STATIC_BUILD WINDOWS)
    set(LIB_EXPORT)
    set(LIB_LOCAL)

    if (NOT ${STATIC_BUILD})
        if (WINDOWS)
            # add dllexport defs

            set(LIB_EXPORT "__declspec (dllexport)")
            set(LIB_LOCAL)

        else()
            # add visibility defs

            set(LIB_EXPORT "__attribute__ ((visibility (\"default\")))")
            set(LIB_LOCAL  "__attribute__ ((visibility (\"hidden\")))")

        endif()
    endif()

    target_compile_definitions(
        ${TARGET}
        PUBLIC
            GLUG_LIB_API=${LIB_EXPORT}
            GLUG_LIB_LOCAL=${LIB_LOCAL}
    )
endfunction()
