set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

include("../includes.cmake")
include("../cmake/include.cmake")

add_executable(bson2json bson2json.c tools.h)
target_link_libraries(bson2json ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})

add_executable(json2bson json2bson.c tools.h)
target_link_libraries(json2bson ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})

link_directories(../libs/json2map)
add_executable(map2json map2json.c tools.h)
target_link_libraries(map2json ${installname}_libs_json2map)