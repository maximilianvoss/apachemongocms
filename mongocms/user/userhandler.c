#include "userhandler.h"
#include "logout.h"
#include "login.h"
#include "profile.h"
#include "../common/stringutil.h"
#include "../common/mongo.h"
#include "../common/entryhelper.h"
#include "../mod_mongocms.h"
#include "registration.h"
#include "../common/logging.h"

static int user_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename);

// handle the actual request
int user_handler(request_rec *request, char *filename) {
	LOGGING_DEBUG_R(request, "START");

	int retVal = DECLINED;
	switch ( request->method_number ) {
		case M_POST:
			LOGGING_TRACE_R(request, "POST");
			if ( !strcmp(filename, FILENAME_USER_LOGIN) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_USER_LOGIN);
				retVal = login_doLogin(request);
			} else if ( !strcmp(filename, FILENAME_USER_PROFILE) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_USER_PROFILE);
				retVal = profile_set(request);
			} else if ( !strcmp(filename, FILENAME_USER_REGISTRATION) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_USER_REGISTRATION);
				retVal = registration_user(request);
			} else if ( !strcmp(filename, FILENAME_ALL_DELETE) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_ALL_DELETE);
				retVal = registration_removeUser(request);
			} else if ( !strcmp(filename, FILENAME_USER_LOGOUT) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_USER_LOGOUT);
				retVal = logout_destroyUserSession(request);
			}
			break;
		case M_GET:
			LOGGING_TRACE_R(request, "GET");
			if ( stringutil_startsWith(filename, "list.") ) {
				LOGGING_TRACE_R(request, "list.");
				retVal = user_getEntryList(&getModuleConfig()->user.database, request, filename);
			} else if ( !strcmp(filename, FILENAME_USER_LOGOUT) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_USER_LOGOUT);
				retVal = logout_destroyUserSession(request);
			} else if ( !strcmp(filename, FILENAME_USER_PROFILE) ) {
				LOGGING_TRACE_R(request, "%s", FILENAME_USER_PROFILE);
				retVal = profile_get(request);
			}
			break;
		default:
			LOGGING_TRACE_R(request, "OTHERS");
			retVal = HTTP_FORBIDDEN;
	}
	LOGGING_DEBUG_R(request, "retVal=%d", retVal);
	LOGGING_DEBUG_R(request, "DONE");
	return retVal;
}

static int user_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename) {
	LOGGING_DEBUG_R(request, "START");
	int retVal = entryhelper_getEntryList(request, mongoConfig, getModuleConfig()->user.queryList, getModuleConfig()->user.propWhitelistIn, getModuleConfig()->user.propWhitelistOut, getModuleConfig()
			->user.propMappingIn, getModuleConfig()->user.propMappingOut, filename);
	LOGGING_DEBUG_R(request, "retVal=%d", retVal);
	LOGGING_DEBUG_R(request, "DONE");
	return retVal;
}