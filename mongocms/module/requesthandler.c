#include "requesthandler.h"
#include "../common/fileutil.h"
#include "../user/userhandler.h"
#include "../common/stringutil.h"
#include "../assets/assetshandler.h"
#include "../documents/documents.h"
#include "../user/user.h"
#include "../constants.h"
#include "../common/logging.h"

int request_handler(request_rec *request) {
	LOGGING_DEBUG_R(request, "START");
	if ( strcmp(request->handler, "mongocms") ) {
		LOGGING_DEBUG_R(request, "DONE");
		return DECLINED;
	}

	char *filename = fileutil_getFilename(request->filename);

	// check for valid user credentials for doing a post
	if ( request->method_number == M_POST || request->method_number == M_PUT ) {
		if ( ( !( !strcmp(filename, FILENAME_USER_LOGIN) || !strcmp(filename, FILENAME_USER_REGISTRATION) ) && !user_isLoginValid(request) ) ) {
			LOGGING_INFO_R(request, "no valid user credentials");
			LOGGING_DEBUG_R(request, "DONE");
			return DECLINED;
		}
	}
	
	int retVal;
	// check for user/login/profile requests
	if ( !strcmp(filename, FILENAME_USER_LOGIN) || !strcmp(filename, FILENAME_USER_LOGOUT) || !strcmp(filename, FILENAME_USER_PROFILE) || !strcmp(filename, FILENAME_USER_REGISTRATION) ||
	     stringutil_startsWith(request->uri, "/user") ) {
		LOGGING_INFO_R(request, "request for user profile");
		retVal = user_handler(request, filename);
		LOGGING_DEBUG_R(request, "DONE");
		return retVal;
	}

	if ( stringutil_startsWith(request->uri, "/dam") ) {
		LOGGING_INFO_R(request, "request for dam");
		retVal = assets_handler(request, filename);
		LOGGING_DEBUG_R(request, "DONE");
		return retVal;
	}

	LOGGING_INFO_R(request, "request for document");
	retVal = documents_handler(request, filename);
	LOGGING_DEBUG_R(request, "DONE");
	return retVal;
}