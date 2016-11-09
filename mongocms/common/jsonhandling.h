#ifndef __COMMON_JSONHANDLING_H__
#define __COMMON_JSONHANDLING_H__

#include "../includes.h"

//#define COMMON_JSONHANDLING_DEBUG

void jsonhandling_json2aprmap(apr_table_t *map, char *json);
char *jsonhandling_aprmap2json(apr_pool_t *pool, apr_table_t *map);

#endif
