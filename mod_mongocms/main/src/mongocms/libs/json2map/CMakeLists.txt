set(SOURCE_FILES
        jsmn.c
        jsmn.h
        json2map.c
        json2map.h
        map2json.c
        map2json.h
        stringlib.c
        stringlib.h
        debugging.h
        config.h)

add_library(${installname}_libs_json2map SHARED ${SOURCE_FILES})

install(TARGETS ${installname}_libs_json2map LIBRARY DESTINATION ${installtarget} COMPONENT ${installname}_libs_json2map)
