include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(SOURCE_FILES
        directives.c
        directives.h requesthandler.c requesthandler.h)

if (NOT TARGET mod_mongocms_module)
    add_library(mod_mongocms_module SHARED ${SOURCE_FILES})
    target_link_libraries(mod_mongocms_module)
    install(TARGETS mod_mongocms_module LIBRARY DESTINATION ${installtarget} COMPONENT mod_mongocms_module)
endif()