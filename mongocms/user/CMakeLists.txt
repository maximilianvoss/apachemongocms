include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

set(SOURCE_FILES
        userhandler.c
        userhandler.h
        logout.c logout.h login.c login.h user.c user.h session.c session.h profile.c profile.h registration.c registration.h password.c password.h)

if (NOT TARGET mod_mongocms_user )
    add_library(mod_mongocms_user SHARED ${SOURCE_FILES})
    target_link_libraries(mod_mongocms_user ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})
    install(TARGETS mod_mongocms_user LIBRARY DESTINATION ${installtarget} COMPONENT mod_mongocms_user)
endif()