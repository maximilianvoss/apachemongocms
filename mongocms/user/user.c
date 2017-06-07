#include "user.h"
#include "../common/cookie.h"
#include "../mod_mongocms.h"
#include "userhandler.h"
#include "../common/logging.h"

apr_table_t *user_checkExistence(request_rec *request, const char *user, char *hashedPassword) {
	LOGGING_DEBUG_R(request, "START");

	apr_table_t *queryMap = apr_table_make(request->pool, 2);
	apr_table_t *userMap = apr_table_make(request->pool, 30);

	apr_table_set(queryMap, MONGO_PROPERTY_USERNAME, user);
	if ( hashedPassword != NULL ) {
		apr_table_set(queryMap, MONGO_PROPERTY_PASSWORD, hashedPassword);
	}

	mongo_cursor_t *cursor = mongo_query(&getModuleConfig()->user.database, pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		LOGGING_INFO_R(request, "User: %s found", user);
		mongo_bson2map(userMap, doc);
	} else {
		LOGGING_INFO_R(request, "User: %s not found", user);
	}
	mongo_destroyCursor(cursor);

	LOGGING_DEBUG_R(request, "DONE");
	return userMap;
}

uint8_t user_isLoginValid(request_rec *request) {
	LOGGING_DEBUG_R(request, "START");

	char *tokenId = cookie_getCookie(request, "tokenId");
	if ( tokenId == NULL ) {
		LOGGING_ERROR_R(request, "token is invalid");
		LOGGING_DEBUG_R(request, "DONE");
		return 0;
	}
	
	apr_table_t *queryMap = apr_table_make(request->pool, 1);
	apr_table_set(queryMap, "login-token#$elemMatch#id", tokenId);

	mongo_cursor_t *cursor = mongo_query(&getModuleConfig()->user.database, request->pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		LOGGING_DEBUG_R(request, "user for tokenId=%s found", tokenId);
		mongo_destroyCursor(cursor);
		LOGGING_DEBUG_R(request, "DONE");
		return 1;
	} else {
		LOGGING_DEBUG_R(request, "user for tokenId=%s not found", tokenId);
		mongo_destroyCursor(cursor);
		LOGGING_DEBUG_R(request, "DONE");
		return 0;
	}
}

apr_table_t *user_getUserMap(request_rec *request) {
	LOGGING_DEBUG_R(request, "START");

	apr_table_t *userMap = apr_table_make(request->pool, 30);

	char *tokenId = cookie_getCookie(request, "tokenId");
	apr_table_t *queryMap = apr_table_make(request->pool, 1);
	apr_table_set(queryMap, "login-token#$elemMatch#id", tokenId);

	mongo_cursor_t *cursor = mongo_query(&getModuleConfig()->user.database, request->pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		LOGGING_DEBUG_R(request, "user for tokenId=%s found", tokenId);
		mongo_bson2map(userMap, doc);
	} else {
		LOGGING_DEBUG_R(request, "user for tokenId=%s not found", tokenId);
	}
	mongo_destroyCursor(cursor);

	LOGGING_DEBUG_R(request, "DONE");
	return userMap;
}

char *user_getUserName(apr_table_t *userMap) {
	return (char *) apr_table_get(userMap, MONGO_PROPERTY_USERNAME);
}

char *user_getUserId(apr_table_t *userMap) {
	return (char *) apr_table_get(userMap, MONGO_PROPERTY_OID);
}