set(installname "mod_mongocms")
set(installtarget "/usr/local/mongocms/lib")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

if (DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -shared --std=gnu99 -pipe -Wall -g -O2 -fstack-protector-strong -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2 -DLINUX -D_REENTRANT -D_GNU_SOURCE")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -shared --std=gnu99 -pipe -Wall -O2 -fstack-protector-strong -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2 -DLINUX -D_REENTRANT -D_GNU_SOURCE")
endif ()
if (DEBUG)
    add_definitions(-DDEBUG)
endif ()

