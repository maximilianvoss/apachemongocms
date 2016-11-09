#include "password.h"
#include "../common/md5.h"

char *password_hashPassword(apr_pool_t *pool, const char *user, const char *password) {
	char *buffer = apr_palloc(pool, sizeof(char) * strlen(user) + strlen(password) + 2);
	sprintf(buffer, "%s=%s", user, password);
	return md5_str2md5(pool, buffer);
}