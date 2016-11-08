#include "userhandler.h"
#include "logout.h"
#include "login.h"
#include "profile.h"
#include "../common/stringutil.h"
#include "../common/mongo.h"
#include "../common/entryhelper.h"
#include "../mod_mongocms.h"

int user_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename);

// handle the actual request
int user_handler(request_rec *request, char *filename) {
	switch ( request->method_number ) {
		case M_POST:
			if ( !strcmp(filename, USER_LOGIN_FILENAME) )
				return login_doLogin(request);
			if ( !strcmp(filename, USER_PROFILE_FILENAME) )
				return profile_set(request);
			if ( !strcmp(filename, USER_LOGOUT_FILENAME) )
				return logout_destroyUserSession(request);
			break;
		case M_GET:
			if ( stringutil_startsWith(filename, "list.") )
				return user_getEntryList(&getModuleConfig()->user.database, request, filename);
			if ( !strcmp(filename, USER_LOGOUT_FILENAME) )
				return logout_destroyUserSession(request);
			if ( !strcmp(filename, USER_PROFILE_FILENAME) )
				return profile_get(request);
			break;
		default:
			return HTTP_FORBIDDEN;
	}
	return DECLINED;
}

int user_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename) {
	return entryhelper_getEntryList(request, mongoConfig, getModuleConfig()->user.queryList, getModuleConfig()->user.propWhitelistOut, getModuleConfig()->user.propMappingOut, filename);
}