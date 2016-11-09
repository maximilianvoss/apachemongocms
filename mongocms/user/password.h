#ifndef __USER_PASSWORD_H__
#define __USER_PASSWORD_H__

#include "../includes.h"

char *password_hashPassword(apr_pool_t *pool, const char *user, const char *password);

#endif 
