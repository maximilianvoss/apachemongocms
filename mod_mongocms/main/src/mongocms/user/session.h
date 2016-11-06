#ifndef __USER_SESSION_H__
#define __USER_SESSION_H__

#include "../includes.h"

long session_getSessionCount(apr_pool_t *pool, apr_table_t *userMap);
char *session_createUserSession(apr_pool_t *pool, apr_table_t *userMap, long currentTimestamp, long expireTimeStamp);
void session_terminateExpiredSessions(apr_pool_t *pool, apr_table_t *userMap, long currentTimeStamp);
void session_terminateSession(apr_pool_t *pool, char *tokenId);

#endif 
