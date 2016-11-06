find_path(APACHE_INCLUDE_DIR
        NAMES httpd.h
        PATH_SUFFIXES httpd apache apache2
        )

if (NOT DEFINED APACHE_MODULE_DIR)
    find_program(APXS_BIN NAMES apxs apxs2
            PATH_SUFFIXES httpd apache apache2
            )

    if (APXS_BIN)
        EXEC_PROGRAM(${APXS_BIN}
                ARGS -q LIBEXECDIR
                OUTPUT_VARIABLE APACHE_MODULE_DIR)
    endif (APXS_BIN)
endif (NOT DEFINED APACHE_MODULE_DIR)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set APACHE_FOUND to TRUE if 
# all listed variables are TRUE
find_package_handle_standard_args(APACHE DEFAULT_MSG APACHE_INCLUDE_DIR)
mark_as_advanced(APACHE_INCLUDE_DIR)
