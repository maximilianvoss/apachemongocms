#ifndef __COMMON_STRINGUTIL_H__
#define __COMMON_STRINGUTIL_H__


#include "../includes.h"
#include <string.h>

#define COMMON_STRINGUTIL_DEBUG

typedef struct {
	long start;
	long length;
} stringutil_tokens_t;

void stringutil_getToken(stringutil_tokens_t *token, char *str, char *buffer);
int stringutil_splitTokens(stringutil_tokens_t *tokens, char *str, char chr, int maxCount);
void stringutil_removeWhitespaces(char *str);
int stringutil_indexOf(char *str, char letter);
int stringutil_lastIndexOf(char *str, char letter);
char *stringutil_subString(apr_pool_t *pool, char *str, int start, int end);
char *stringutil_escapeString(apr_pool_t *pool, char *str);
void stringutil_replaceChar(char *str, char old, char new);
uint8_t stringutil_startsWith(char *str, char *token);
char *stringutil_longToString(apr_pool_t *pool, long value);
char *stringutil_replaceVariables(apr_pool_t *pool, char *string, apr_table_t *map);

#endif