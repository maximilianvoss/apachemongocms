include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(SOURCE_FILES
        documents.c
        documents.h
        )

add_library(${installname}_documents SHARED ${SOURCE_FILES})
target_link_libraries(${installname}_documents ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})
install(TARGETS ${installname}_documents LIBRARY DESTINATION ${installtarget} COMPONENT ${installname}_documents)
