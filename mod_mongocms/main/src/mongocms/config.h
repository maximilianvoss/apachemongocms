/* 
This file contains standard configurations and structures to run the Apache module
*/

#ifndef __MOD_MONGOCMS_CONFIG_H__
#define __MOD_MONGOCMS_CONFIG_H__

#include "constants.h"
#include "includes.h"
#include "common/mongo.h"

// type for all document dependent configurations
typedef struct {
	mongo_config_t database;
	apr_array_header_t *documentPropInputWhitelist;
	apr_array_header_t *documentPropOutputWhitelist;

	apr_table_t *documentPropMapping;
	apr_table_t *documentPropMappingInverse;
} mongo_config_document_t;

// type for all asset dependent configurations
typedef struct {
	mongo_config_t database;

	apr_array_header_t *metadataParamInputWhitelist;
	apr_array_header_t *metadataParamOutputWhitelist;

	apr_table_t *metadataParamMapping;
	apr_table_t *metadataParamMappingInverse;

	const char *assetstorepath;
	const char *assettmppath;

	apr_array_header_t *image_transform;
} mongo_config_asset_t;

// type for all user dependent configurations
typedef struct {
	mongo_config_t database;
	apr_table_t *profileParamMapping;
	apr_table_t *profileParamMappingInverse;
	apr_array_header_t *profileParamInputWhitelist;
	apr_array_header_t *profileParamOutputWhitelist;
	int sessionMaxCount;
	int sessionExpirationTime;
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
	char name[STRING_STD_LENGTH];
} module_config_transform_t;

#endif