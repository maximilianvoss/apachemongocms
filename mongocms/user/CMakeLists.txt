include("../cmake/include.cmake")

set(SOURCE_FILES
        userhandler.c
        userhandler.h
        logout.c logout.h login.c login.h user.c user.h session.c session.h profile.c profile.h registration.c registration.h password.c password.h)

add_library(${installname}_user SHARED ${SOURCE_FILES})
target_link_libraries(${installname}_user ${APR_LIBRARIES} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})
install(TARGETS ${installname}_user LIBRARY DESTINATION ${installtarget} COMPONENT ${installname}_user)
