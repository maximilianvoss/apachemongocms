#ifndef __COMMON_MD5_H__
#define __COMMON_MD5_H__

#include "../includes.h"

char *md5_file2md5(apr_pool_t *pool, char *filename);
char *md5_str2md5(apr_pool_t *pool, char *string);
		
#endif
