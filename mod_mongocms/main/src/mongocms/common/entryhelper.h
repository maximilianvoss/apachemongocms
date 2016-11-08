#ifndef __COMMON_ENTRYHELPER_H__
#define __COMMON_ENTRYHELPER_H__

#include "../includes.h"
#include "mongo.h"
#include "../config.h"

#define COMMON_ENTRYHELPER_DEBUG

char *entryhelper_getEntryId(request_rec *request);
apr_table_t *entryhelper_getEntryById(mongo_config_t *config, apr_pool_t *pool, char *entryId);
uint8_t entryhelper_isEntryWritable(apr_pool_t *pool, apr_table_t *userMap, apr_table_t *map);
int entryhelper_updateEntry(request_rec *request, mongo_config_t *mongoConfig, apr_array_header_t *whitelistIn, apr_array_header_t *whitelistOut, apr_table_t *mappingIn, apr_table_t *mappingOut);
int entryhelper_getEntry(request_rec *request, mongo_config_t *mongoConfig, apr_array_header_t *whitelistOut, apr_table_t *mappingOut);
int entryhelper_getEntryList(request_rec *request, mongo_config_t *mongoConfig, mongo_config_query_list_t *queryList, apr_array_header_t *whitelistOut, apr_table_t *mappingOut, char *filename);

#endif
