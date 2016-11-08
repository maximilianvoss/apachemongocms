#include "session.h"
#include "../config.h"
#include "../common/maputil.h"
#include "../common/stringutil.h"
#include "../common/md5.h"
#include "../mod_mongocms.h"
#include "userhandler.h"

void session_terminateExpiredSessions(apr_pool_t *pool, apr_table_t *userMap, long currentTimeStamp) {
	apr_table_t *userUpdate = apr_table_make(pool, 2);

	apr_table_set(userUpdate, "$pull#login-token#expire-timestamp#$lte#$timestamp#t", stringutil_longToString(pool, currentTimeStamp));
	apr_table_set(userUpdate, "$pull#login-token#expire-timestamp#$lte#$timestamp#i", "0");

	mongo_update(&getModuleConfig()->user.database, pool, userMap, userUpdate);
}

void session_terminateSession(apr_pool_t *pool, char *tokenId) {
	apr_table_t *update = apr_table_make(pool, 1);
	apr_table_t *old = apr_table_make(pool, 0);
	apr_table_set(update, "$pull#login-token#id", tokenId);

	mongo_update(&getModuleConfig()->user.database, pool, old, update);
}


long session_getSessionCount(apr_pool_t *pool, apr_table_t *userMap) {
	return maputil_getMaxArrayId(pool, userMap, "login-token");
}


char *session_createUserSession(apr_pool_t *pool, apr_table_t *userMap, long currentTimestamp, long expireTimeStamp) {
	char buffer[BUFFER_SIZE];
	const char *username = apr_table_get(userMap, USER_MONGO_PROPERTY_USERNAME);
	apr_table_t *userMapUpdated = apr_table_clone(pool, userMap);

	long newArrayId = maputil_getMaxArrayId(pool, userMapUpdated, "login-token");
	if ( newArrayId > 10 ) {
		return NULL;
	}

	sprintf(buffer, "login-token[%ld]#login-timestamp#$timestamp#t", newArrayId);
	apr_table_set(userMapUpdated, buffer, stringutil_longToString(pool, currentTimestamp));

	sprintf(buffer, "login-token[%ld]#login-timestamp#$timestamp#i", newArrayId);
	apr_table_set(userMapUpdated, buffer, "0");

	sprintf(buffer, "login-token[%ld]#expire-timestamp#$timestamp#t", newArrayId);
	apr_table_set(userMapUpdated, buffer, stringutil_longToString(pool, expireTimeStamp));

	sprintf(buffer, "login-token[%ld]#expire-timestamp#$timestamp#i", newArrayId);
	apr_table_set(userMapUpdated, buffer, "0");

	sprintf(buffer, "%s,%ld,%ld,%ld", username, currentTimestamp, expireTimeStamp, newArrayId);
	char *idHash = md5_str2md5(pool, buffer);

	sprintf(buffer, "login-token[%ld]#id", newArrayId);
	apr_table_set(userMapUpdated, buffer, idHash);

	mongo_update(&getModuleConfig()->user.database, pool, userMap, userMapUpdated);

	return idHash;
}