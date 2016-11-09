#include "user.h"
#include "../common/cookie.h"
#include "../mod_mongocms.h"
#include "userhandler.h"

#ifdef LOGIN_USER_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "user", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "user")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

apr_table_t *user_checkExistence(apr_pool_t *pool, const char *username, char *hashedPassword) {
	DEBUG_MSG("%s_checkExistence([apr_pool_t *], %s)...", username);

	apr_table_t *queryMap = apr_table_make(pool, 2);
	apr_table_t *userMap = apr_table_make(pool, 30);

	apr_table_set(queryMap, MONGO_PROPERTY_USERNAME, username);
	if ( hashedPassword != NULL ) {
		apr_table_set(queryMap, MONGO_PROPERTY_PASSWORD, hashedPassword);
	}

	mongo_cursor_t *cursor = mongo_query(&getModuleConfig()->user.database, pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		DEBUG_MSG("%s_checkExistence([apr_pool_t *], %s): User found", username);
		mongo_bson2map(userMap, doc);
	} else {
		DEBUG_MSG("%s_checkExistence([apr_pool_t *], %s): User not found", username);
	}
	mongo_destroyCursor(cursor);

	DEBUG_MSG("%s_checkExistence([apr_pool_t *], %s)... DONE", username);
	return userMap;
}

uint8_t user_isLoginValid(request_rec *request) {
	DEBUG_PUT("%s_isLoginValid([request_rec *])...");

	char *tokenId = cookie_getCookie(request, "tokenId");
	if ( tokenId == NULL ) {
		DEBUG_PUT("%s_isLoginValid([request_rec *]): token is invalid");
		return 0;
	}
	
	apr_table_t *queryMap = apr_table_make(request->pool, 1);
	apr_table_set(queryMap, "login-token#$elemMatch#id", tokenId);

	mongo_cursor_t *cursor = mongo_query(&getModuleConfig()->user.database, request->pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		DEBUG_PUT("%s_isLoginValid([request_rec *]): User found");
		mongo_destroyCursor(cursor);
		DEBUG_PUT("%s_isLoginValid([request_rec *])... DONE");
		return 1;
	} else {
		DEBUG_PUT("%s_isLoginValid([request_rec *]): User not found");
		mongo_destroyCursor(cursor);
		DEBUG_PUT("%s_isLoginValid([request_rec *])... DONE");
		return 0;
	}
}

apr_table_t *user_getUserMap(request_rec *request) {
	DEBUG_PUT("%s_isLoginValid([request_rec *])...");

	apr_table_t *userMap = apr_table_make(request->pool, 30);

	char *tokenId = cookie_getCookie(request, "tokenId");
	apr_table_t *queryMap = apr_table_make(request->pool, 1);
	apr_table_set(queryMap, "login-token#$elemMatch#id", tokenId);

	mongo_cursor_t *cursor = mongo_query(&getModuleConfig()->user.database, request->pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		DEBUG_PUT("%s_isLoginValid([request_rec *]): User found");
		mongo_bson2map(userMap, doc);
	} else {
		DEBUG_PUT("%s_isLoginValid([request_rec *]): User not found");
	}
	mongo_destroyCursor(cursor);

	DEBUG_PUT("%s_isLoginValid([request_rec *])... DONE");
	return userMap;
}

char *user_getUserName(apr_table_t *userMap) {
	DEBUG_PUT("%s_getUserName([apr_table_t *])...");
	DEBUG_PUT("%s_getUserName([apr_table_t *])... DONE");
	return (char *) apr_table_get(userMap, MONGO_PROPERTY_USERNAME);
}

char *user_getUserId(apr_table_t *userMap) {
	DEBUG_PUT("%s_getUserId([apr_table_t *])...");
	DEBUG_PUT("%s_getUserId([apr_table_t *])... DONE");
	return (char *) apr_table_get(userMap, MONGO_PROPERTY_OID);
}