include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(SOURCE_FILES
        fileutil.c
        fileutil.h
        mongo.c
        mongo.h
        requesthelper.c
        requesthelper.h
        jsonhandling.c
        jsonhandling.h
        stringutil.c
        stringutil.h maputil.c maputil.h logging.h jsonmapfilter.c jsonmapfilter.h cookie.c cookie.h md5.c md5.h entryhelper.c entryhelper.h)

if (NOT TARGET mod_mongocms_common)
    add_library(mod_mongocms_common SHARED ${SOURCE_FILES})
    target_link_libraries(mod_mongocms_common ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES} ${IMAGEMAGICK_LIBRARIES})
    install(TARGETS mod_mongocms_common LIBRARY DESTINATION ${installtarget} COMPONENT mod_mongocms_common)
endif()
