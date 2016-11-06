#ifndef __USER_USER_H__
#define __USER_USER_H__

#include "../includes.h"

//#define LOGIN_USER_DEBUG

apr_table_t *user_checkExistence(apr_pool_t *pool, const char *username, char *hashedPassword);
uint8_t user_isLoginValid(request_rec *request);
apr_table_t *user_getUserMap(request_rec *request);
char *user_getUserName(apr_table_t *userMap);
char *user_getUserId(apr_table_t *userMap);

#endif
