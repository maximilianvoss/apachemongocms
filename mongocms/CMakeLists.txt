cmake_minimum_required(VERSION 3.0)

include(includes.cmake)
include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

add_subdirectory(libs/csafestring)
add_subdirectory(libs/ctemplate)
add_subdirectory(libs/json2map)

project(mod_mongocms)
add_subdirectory(module)
add_subdirectory(common)
add_subdirectory(user)
add_subdirectory(assets)
add_subdirectory(documents)

set(SOURCE_FILES
        config.h includes.h mod_mongocms.c mod_mongocms.h constants.h)

link_directories(module common assets documents user json2map)

set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-rpath,/usr/local/publisher/lib")

if (NOT TARGET mod_mongocms)
    add_library(mod_mongocms MODULE ${SOURCE_FILES})
    target_link_libraries(mod_mongocms mod_mongocms_module mod_mongocms_common mod_mongocms_assets mod_mongocms_documents mod_mongocms_user json2map ${APR_LIBRARIES} ${BSON_LIBRARIES} 
            ${MONGOC_LIBRARIES}
            ${IMAGEMAGICK_LIBRARIES})
    install(TARGETS mod_mongocms LIBRARY DESTINATION ${installtarget} COMPONENT mod_mongocms)
endif()