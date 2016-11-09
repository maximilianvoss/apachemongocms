include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(SOURCE_FILES
        directives.c
        directives.h requesthandler.c requesthandler.h)

add_library(${installname}_module SHARED ${SOURCE_FILES})
target_link_libraries(${installname}_module)
install(TARGETS ${installname}_module LIBRARY DESTINATION ${installtarget} COMPONENT ${installname}_module)
