#ifndef __USER_USER_H__
#define __USER_USER_H__

#include "../includes.h"

apr_table_t *user_checkExistence(request_rec *request, const char *user, char *hashedPassword);
uint8_t user_isLoginValid(request_rec *request);
apr_table_t *user_getUserMap(request_rec *request);
char inline *user_getUserName(apr_table_t *userMap);
char inline *user_getUserId(apr_table_t *userMap);

#endif
