#include "userhandler.h"
#include "../common/fileutil.h"
#include "logout.h"
#include "login.h"
#include "profile.h"

// handle the actual request
int user_handler(request_rec *request) {
	switch ( request->method_number ) {
		case M_POST:
			if ( !strcmp(fileutil_getFilename(request->filename), USER_LOGIN_FILENAME) )
				return login_doLogin(request);
			if ( !strcmp(fileutil_getFilename(request->filename), USER_PROFILE_FILENAME) )
				return profile_set(request);
			if ( !strcmp(fileutil_getFilename(request->filename), USER_LOGOUT_FILENAME) )
				return logout_destroyUserSession(request);
			break;
		case M_GET:
			if ( !strcmp(fileutil_getFilename(request->filename), USER_LOGOUT_FILENAME) )
				return logout_destroyUserSession(request);
			if ( !strcmp(fileutil_getFilename(request->filename), USER_PROFILE_FILENAME) )
				return profile_get(request);
			break;
		default:
			return HTTP_FORBIDDEN;
	}
	return DECLINED;
}



