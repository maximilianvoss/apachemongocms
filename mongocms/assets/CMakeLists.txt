include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -fopenmp -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16")

set(SOURCE_FILES
        assetshandler.c
        assetshandler.h
        image/rendition.c
        image/rendition.h metadata/extractor.c metadata/extractor.h image/upload.c image/upload.h)

if (NOT TARGET mod_mongocms_assets)
    add_library(mod_mongocms_assets SHARED ${SOURCE_FILES})
    target_link_libraries(mod_mongocms_assets ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES} ${IMAGEMAGICK_LIBRARIES})
    install(TARGETS mod_mongocms_assets LIBRARY DESTINATION ${installtarget} COMPONENT mod_mongocms_assets)
endif()