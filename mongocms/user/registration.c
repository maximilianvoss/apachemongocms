#include "registration.h"
#include "../common/requesthelper.h"
#include "user.h"
#include "userhandler.h"
#include "password.h"
#include "../mod_mongocms.h"
#include "../common/logging.h"

int registration_user(request_rec *request) {
	LOGGING_DEBUG_R(request, "START");
	apr_table_t *requestMap = requesthelper_getPostMap(request, getModuleConfig()->user.propMappingIn, getModuleConfig()->user.propWhitelistIn);

	const char *user = apr_table_get(requestMap, MONGO_PROPERTY_USERNAME);
	const char *password = apr_table_get(requestMap, MONGO_PROPERTY_PASSWORD);

	if ( user == NULL || password == NULL ) {
		LOGGING_DEBUG_R(request, "user == NULL || password == NULL");
		LOGGING_DEBUG_R(request, "DONE");
		return HTTP_BAD_REQUEST;
	}

	apr_table_t *usermap = user_checkExistence(request, user, NULL);
	if ( !apr_is_empty_table(usermap) ) {
		ap_rputs("{\"Status\":\"user exists\"}", request);
		LOGGING_DEBUG_R(request, "user: %s already exists", user);
		LOGGING_DEBUG_R(request, "DONE");
		return HTTP_CONFLICT;
	}

	LOGGING_DEBUG_R(request, "register user: %s", user);
	char *hashedPassword = password_hashPassword(request->pool, user, password);
	apr_table_set(requestMap, MONGO_PROPERTY_USERNAME, user);
	apr_table_set(requestMap, MONGO_PROPERTY_PASSWORD, hashedPassword);
	apr_table_set(requestMap, "login-token[x]", "0");

	mongo_commit(&getModuleConfig()->user.database, request->pool, requestMap);

	ap_rputs("{\"Status\":\"user created\"}", request);
	LOGGING_DEBUG_R(request, "DONE");
	return OK;
}


int registration_removeUser(request_rec *request) {
	LOGGING_DEBUG_R(request, "START");
	apr_table_t *requestMap = requesthelper_getPostMap(request, getModuleConfig()->user.propMappingIn, getModuleConfig()->user.propWhitelistIn);

	const char *user = apr_table_get(requestMap, MONGO_PROPERTY_USERNAME);
	const char *password = apr_table_get(requestMap, MONGO_PROPERTY_PASSWORD);

	if ( user == NULL || password == NULL ) {
		LOGGING_DEBUG_R(request, "user == NULL || password == NULL");
		LOGGING_DEBUG_R(request, "DONE");
		return HTTP_BAD_REQUEST;
	}

	char *hashedPassword = password_hashPassword(request->pool, user, password);

	apr_table_t *usermap = user_checkExistence(request, user, hashedPassword);
	if ( apr_is_empty_table(usermap) ) {
		ap_rputs("{\"Status\":\"user doesnt exist\"}", request);
		LOGGING_DEBUG_R(request, "user: %s doesn't exist", user);
		LOGGING_DEBUG_R(request, "DONE");
		return HTTP_CONFLICT;
	}

	apr_table_t *deleteEntryMap = apr_table_make(request->pool, 2);
	apr_table_set(deleteEntryMap, MONGO_PROPERTY_USERNAME, user);
	apr_table_set(deleteEntryMap, MONGO_PROPERTY_PASSWORD, hashedPassword);

	mongo_delete(&getModuleConfig()->user.database, request->pool, deleteEntryMap);

	ap_rputs("{\"Status\":\"user removed\"}", request);
	LOGGING_DEBUG_R(request, "DONE");
	return OK;
}