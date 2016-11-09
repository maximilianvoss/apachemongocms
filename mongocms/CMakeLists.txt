cmake_minimum_required(VERSION 3.0)
#project(mod_mongocms)

include(includes.cmake)
include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

add_subdirectory(module)
add_subdirectory(libs)
add_subdirectory(common)
add_subdirectory(user)
add_subdirectory(assets)
add_subdirectory(documents)

set(SOURCE_FILES
        config.h includes.h mod_mongocms.c mod_mongocms.h constants.h)

link_directories(module common assets documents user libs/json2map)

set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-rpath,/usr/local/publisher/lib")
add_library(${installname} MODULE ${SOURCE_FILES})
target_link_libraries(${installname} ${installname}_module ${installname}_common ${installname}_assets ${installname}_documents ${installname}_libs_json2map ${installname}_user ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES}
        ${IMAGEMAGICK_LIBRARIES})
install(TARGETS ${installname} LIBRARY DESTINATION ${installtarget} COMPONENT ${installname})
