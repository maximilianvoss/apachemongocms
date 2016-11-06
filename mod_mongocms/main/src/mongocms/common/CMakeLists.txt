include("../cmake/include.cmake")

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

add_library(${installname}_common SHARED ${SOURCE_FILES})
target_link_libraries(${installname}_common ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES} ${IMAGEMAGICK_LIBRARIES})
install(TARGETS ${installname}_common LIBRARY DESTINATION ${installtarget} COMPONENT ${installname}_common)

