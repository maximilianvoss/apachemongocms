#include "registration.h"
#include "../common/requesthelper.h"
#include "user.h"
#include "userhandler.h"
#include "password.h"
#include "../mod_mongocms.h"

#ifdef USER_REGISTRATION_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "registration", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "registration")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

int registration_user(request_rec *request) {
	DEBUG_PUT("%s_user([request_rec *])...");
	apr_table_t *requestMap = requesthelper_getPostMap(request, getModuleConfig()->user.propMappingIn, getModuleConfig()->user.propWhitelistIn);

	const char *user = apr_table_get(requestMap, MONGO_PROPERTY_USERNAME);
	const char *password = apr_table_get(requestMap, MONGO_PROPERTY_PASSWORD);

	if ( user == NULL || password == NULL ) {
		DEBUG_PUT("%s_user([request_rec *]): Bad Request");
		DEBUG_PUT("%s_user([request_rec *])... DONE");
		return HTTP_BAD_REQUEST;
	}

	apr_table_t *usermap = user_checkExistence(request->pool, user, NULL);
	if ( !apr_is_empty_table(usermap) ) {
		ap_rputs("{\"Status\":\"user exists\"}", request);
		DEBUG_PUT("%s_user([request_rec *]): User already exists");
		DEBUG_PUT("%s_user([request_rec *])... DONE");
		return HTTP_CONFLICT;
	}

	char *hashedPassword = password_hashPassword(request->pool, user, password);
	apr_table_set(requestMap, MONGO_PROPERTY_USERNAME, user);
	apr_table_set(requestMap, MONGO_PROPERTY_PASSWORD, hashedPassword);
	apr_table_set(requestMap, "login-token[x]", "0");

	mongo_commit(&getModuleConfig()->user.database, request->pool, requestMap);

	ap_rputs("{\"Status\":\"user created\"}", request);
	DEBUG_PUT("%s_user([request_rec *])... DONE");
	return OK;
}


int registration_removeUser(request_rec *request) {
	DEBUG_PUT("%s_removeUser([request_rec *])...");
	apr_table_t *requestMap = requesthelper_getPostMap(request, getModuleConfig()->user.propMappingIn, getModuleConfig()->user.propWhitelistIn);

	const char *user = apr_table_get(requestMap, MONGO_PROPERTY_USERNAME);
	const char *password = apr_table_get(requestMap, MONGO_PROPERTY_PASSWORD);

	if ( user == NULL || password == NULL ) {
		DEBUG_PUT("%s_removeUser([request_rec *]): Bad Request");
		DEBUG_PUT("%s_removeUser([request_rec *])... DONE");
		return HTTP_BAD_REQUEST;
	}

	char *hashedPassword = password_hashPassword(request->pool, user, password);

	apr_table_t *usermap = user_checkExistence(request->pool, user, hashedPassword);
	if ( apr_is_empty_table(usermap) ) {
		ap_rputs("{\"Status\":\"user doesnt exist\"}", request);
		DEBUG_PUT("%s_removeUser([request_rec *]): User doesnt exist");
		DEBUG_PUT("%s_removeUser([request_rec *])... DONE");
		return HTTP_CONFLICT;
	}

	apr_table_t *deleteEntryMap = apr_table_make(request->pool, 2);
	apr_table_set(deleteEntryMap, MONGO_PROPERTY_USERNAME, user);
	apr_table_set(deleteEntryMap, MONGO_PROPERTY_PASSWORD, hashedPassword);

	mongo_delete(&getModuleConfig()->user.database, request->pool, deleteEntryMap);

	ap_rputs("{\"Status\":\"user removed\"}", request);
	DEBUG_PUT("%s_removeUser([request_rec *])... DONE");
	return OK;
}