/* 
This file contains standard configurations and structures to run the Apache module
*/

#ifndef __MOD_MONGOCMS_CONFIG_H__
#define __MOD_MONGOCMS_CONFIG_H__

#include "constants.h"
#include "includes.h"
#include "common/mongo.h"


typedef struct s_mongo_config_query_list {
	char *name;
	char *query;
	struct s_mongo_config_query_list *next;
} mongo_config_query_list_t;

// type for all document dependent configurations
typedef struct {
	mongo_config_t database;
	apr_array_header_t *propWhitelistIn;
	apr_array_header_t *propWhitelistOut;

	apr_table_t *propMappingOut;
	apr_table_t *propMappingIn;

	mongo_config_query_list_t *queryList;
} mongo_config_document_t;

// type for all asset dependent configurations
typedef struct {
	mongo_config_t database;

	apr_array_header_t *propWhitelistIn;
	apr_array_header_t *propWhitelistOut;

	apr_table_t *propMappingOut;
	apr_table_t *propMappingIn;

	mongo_config_query_list_t *queryList;

	const char *assetstorepath;
	const char *assettmppath;

	apr_array_header_t *assetTransform;
} mongo_config_asset_t;

// type for all user dependent configurations
typedef struct {
	mongo_config_t database;
	apr_table_t *propMappingOut;
	apr_table_t *propMappingIn;
	apr_array_header_t *propWhitelistIn;
	apr_array_header_t *propWhitelistOut;
	int sessionMaxCount;
	int sessionExpirationTime;
	mongo_config_query_list_t *queryList;
} mongo_config_user_t;

// Type for configurations
typedef struct {
	server_rec *serverRec;
	apr_pool_t *modulePool;

	mongo_config_document_t document;
	mongo_config_user_t user;
	mongo_config_asset_t asset;
} module_config_t;

// Type for image transformations
typedef struct {
	uint16_t width;
	char *name;
} module_config_transform_t;

#endif