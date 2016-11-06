#include "login.h"
#include "session.h"
#include "../common/cookie.h"
#include "user.h"
#include "../common/requesthelper.h"
#include "../common/md5.h"
#include "../mod_mongocms.h"

#ifdef USER_LOGIN_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "login", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "login")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

char *login_hashPassword(apr_pool_t *pool, const char *user, const char *password) {
	char *buffer = apr_palloc(pool, sizeof(char) * strlen(user) + strlen(password) + 2);
	sprintf(buffer, "%s=%s", user, password);
	return md5_str2md5(pool, buffer);
}


int login_doLogin(request_rec *request) {
	DEBUG_PUT("%s_doLogin([request_rec *])...");
	long currentTimeStamp = (long) time(NULL);
	long expireTimeStamp = currentTimeStamp + getModuleConfig()->user.sessionExpirationTime * 3600;

	apr_table_t *requestMap = requesthelper_getPostMap(request, NULL, NULL);
	const char *user = apr_table_get(requestMap, "username");
	const char *password = apr_table_get(requestMap, "password");
	if ( user == NULL || password == NULL ) {
		return HTTP_FORBIDDEN;
	}

	char *hashedPassword = login_hashPassword(request->pool, user, password);

	apr_table_t *usermap = user_checkExistence(request->pool, user, hashedPassword);
	if ( apr_is_empty_table(usermap) ) {
		ap_rputs("{\"Status\":\"invalid user\"}", request);
		return HTTP_FORBIDDEN;
	}

	session_terminateExpiredSessions(request->pool, usermap, currentTimeStamp);

	if ( session_getSessionCount(request->pool, usermap) + 2 > getModuleConfig()->user.sessionMaxCount ) {
		ap_rputs("{\"Status\":\"too many logins\"}", request);
		return HTTP_FORBIDDEN;
	}

	char *loginToken = session_createUserSession(request->pool, usermap, currentTimeStamp, expireTimeStamp);
	if ( loginToken == NULL ) {
		ap_rputs("{\"Status\":\"unable to create session\"}", request);
		return HTTP_FORBIDDEN;
	}
	cookie_setCookie(request, "tokenId", loginToken, (time_t) expireTimeStamp, NULL, "/");
	ap_rprintf(request, "{ \"tokenId\": \"%s\" }", loginToken);

	DEBUG_PUT("%s_doLogin([request_rec *])... DONE");
	return OK;
}