include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(SOURCE_FILES
        documents.c
        documents.h
        )

if (NOT TARGET mod_mongocms_documents)
    add_library(mod_mongocms_documents SHARED ${SOURCE_FILES})
    target_link_libraries(mod_mongocms_documents ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})
    install(TARGETS mod_mongocms_documents LIBRARY DESTINATION ${installtarget} COMPONENT mod_mongocms_documents)
endif()