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
	LOGGING_ERROR("huhu");
	if ( strcmp(request->handler, "mongocms") ) {
		return DECLINED;
	}

	char *filename = fileutil_getFilename(request->filename);

	// check for valid user credentials for doing a post
	if ( request->method_number == M_POST || request->method_number == M_PUT ) {
		if ( ( !( !strcmp(filename, FILENAME_USER_LOGIN) || !strcmp(filename, FILENAME_USER_REGISTRATION) ) && !user_isLoginValid(request) ) ) {
			return DECLINED;
		}
	}

	// check for user/login/profile requests
	if ( !strcmp(filename, FILENAME_USER_LOGIN) || !strcmp(filename, FILENAME_USER_LOGOUT) || !strcmp(filename, FILENAME_USER_PROFILE) || !strcmp(filename, FILENAME_USER_REGISTRATION) ||
	     stringutil_startsWith(request->uri, "/user") ) {
		return user_handler(request, filename);
	}

	if ( stringutil_startsWith(request->uri, "/dam") ) {
		return assets_handler(request, filename);
	}

	return documents_handler(request, filename);
}