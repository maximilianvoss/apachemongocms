#include "directives.h"
#include "../mod_mongocms.h"
#include "../common/jsonhandling.h"

#define DIRECTIVES_MODULE_HELPER(x) if(!strcmp(cmd->cmd->name, x))

const char *directives_parserString(cmd_parms *cmd, void *cfg, const char *arg);
const char *directives_parserInt(cmd_parms *cmd, void *cfg, const char *arg);
const char *directives_parserStringList(cmd_parms *cmd, void *cfg, const char *arg);
const char *directives_parserTransformator(cmd_parms *cmd, void *cfg, const char *arg1, const char *arg2);
const char *directives_parserSpecialAssetImageTransform(cmd_parms *cmd, void *cfg, const char *arg1, const char *arg2);
const char *directives_parseCreateList(cmd_parms *cmd, void *mconfig, const char *args);

// Documents
const char DocMongoURI[] = "DocMongoURI";
const char DocMongoDB[] = "DocMongoDB";
const char DocMongoCollection[] = "DocMongoCollection";
const char DocumentPropInputWhitelist[] = "DocumentPropInputWhitelist";
const char DocumentPropOutputWhitelist[] = "DocumentPropOutputWhitelist";
const char DocumentPropMapping[] = "DocumentPropMapping";
const char DocumentQueryList[] = "DocumentQueryList";
// Assets
const char AssetMongoURI[] = "AssetMongoURI";
const char AssetMongoDB[] = "AssetMongoDB";
const char AssetMongoCollection[] = "AssetMongoCollection";
const char AssetStorePath[] = "AssetStorePath";
const char AssetTmpPath[] = "AssetTmpPath";
const char AssetQueryList[] = "AssetQueryList";
const char AssetTransform[] = "AssetTransform";

const char AssetMetadataParamMapping[] = "AssetMetadataParamMapping";
const char AssetMetadataParamInputWhitelist[] = "AssetMetadataParamInputWhitelist";
const char AssetMetadataParamOutputWhitelist[] = "AssetMetadataParamOutputWhitelist";

// User Management
const char UserMongoURI[] = "UserMongoURI";
const char UserMongoDB[] = "UserMongoDB";
const char UserMongoCollection[] = "UserMongoCollection";
const char UserProfileParamMapping[] = "UserProfileParamMapping";
const char UserProfileParamInputWhitelist[] = "UserProfileParamInputWhitelist";
const char UserProfileParamOutputWhitelist[] = "UserProfileParamOutputWhitelist";
const char UserSessionsMaxCount[] = "UserSessionsMaxCount";
const char UserSessionExpirationTime[] = "UserSessionExpirationTime";
const char UserQueryList[] = "UserQueryList";


// Directives to read configurations
command_rec mongocms_directives[] = {
		// Documents
		// MongoDB Documents
		AP_INIT_TAKE1(DocMongoURI, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(DocMongoDB, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(DocMongoCollection, directives_parserString, NULL, RSRC_CONF, ""),

		// Document-Handling
		AP_INIT_TAKE1(DocumentPropInputWhitelist, directives_parserStringList, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(DocumentPropOutputWhitelist, directives_parserStringList, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE2(DocumentPropMapping, directives_parserTransformator, NULL, RSRC_CONF, ""),
		AP_INIT_RAW_ARGS(DocumentQueryList, directives_parseCreateList, NULL, RSRC_CONF, ""),

		// Assets
		// MongoDB Assets
		AP_INIT_TAKE1(AssetMongoURI, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(AssetMongoDB, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(AssetMongoCollection, directives_parserString, NULL, RSRC_CONF, ""),

		// Assets
		AP_INIT_TAKE1(AssetStorePath, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(AssetTmpPath, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE2(AssetTransform, directives_parserSpecialAssetImageTransform, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(AssetMetadataParamInputWhitelist, directives_parserStringList, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(AssetMetadataParamOutputWhitelist, directives_parserStringList, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE2(AssetMetadataParamMapping, directives_parserTransformator, NULL, RSRC_CONF, ""),
		AP_INIT_RAW_ARGS(AssetQueryList, directives_parseCreateList, NULL, RSRC_CONF, ""),

		// User
		// MongoDB User
		AP_INIT_TAKE1(UserMongoURI, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(UserMongoDB, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(UserMongoCollection, directives_parserString, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE2(UserProfileParamMapping, directives_parserTransformator, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(UserProfileParamInputWhitelist, directives_parserStringList, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(UserProfileParamOutputWhitelist, directives_parserStringList, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(UserSessionsMaxCount, directives_parserInt, NULL, RSRC_CONF, ""),
		AP_INIT_TAKE1(UserSessionExpirationTime, directives_parserInt, NULL, RSRC_CONF, ""),
		AP_INIT_RAW_ARGS(UserQueryList, directives_parseCreateList, NULL, RSRC_CONF, ""),

		{NULL}
};

const char *directives_parserString(cmd_parms *cmd, void *cfg, const char *arg) {
	DIRECTIVES_MODULE_HELPER(DocMongoURI) getModuleConfig()->document.database.uri = arg;
	DIRECTIVES_MODULE_HELPER(DocMongoDB) getModuleConfig()->document.database.db = arg;
	DIRECTIVES_MODULE_HELPER(DocMongoCollection) getModuleConfig()->document.database.collection = arg;
	DIRECTIVES_MODULE_HELPER(AssetMongoURI) getModuleConfig()->asset.database.uri = arg;
	DIRECTIVES_MODULE_HELPER(AssetMongoDB) getModuleConfig()->asset.database.db = arg;
	DIRECTIVES_MODULE_HELPER(AssetMongoCollection) getModuleConfig()->asset.database.collection = arg;
	DIRECTIVES_MODULE_HELPER(UserMongoURI) getModuleConfig()->user.database.uri = arg;
	DIRECTIVES_MODULE_HELPER(UserMongoDB) getModuleConfig()->user.database.db = arg;
	DIRECTIVES_MODULE_HELPER(UserMongoCollection) getModuleConfig()->user.database.collection = arg;
	DIRECTIVES_MODULE_HELPER(AssetStorePath) getModuleConfig()->asset.assetstorepath = arg;
	DIRECTIVES_MODULE_HELPER(AssetTmpPath) getModuleConfig()->asset.assettmppath = arg;
	return NULL;
}

const char *directives_parserInt(cmd_parms *cmd, void *cfg, const char *arg) {
	int argInt = atoi(arg);
	DIRECTIVES_MODULE_HELPER(UserSessionsMaxCount) getModuleConfig()->user.sessionMaxCount = argInt;
	DIRECTIVES_MODULE_HELPER(UserSessionExpirationTime) getModuleConfig()->user.sessionExpirationTime = argInt;
	
	return NULL;
}

const char *directives_parserStringList(cmd_parms *cmd, void *cfg, const char *arg) {
	apr_array_header_t **arr = NULL;
	DIRECTIVES_MODULE_HELPER(DocumentPropInputWhitelist) arr = &getModuleConfig()->document.propWhitelistIn;
	DIRECTIVES_MODULE_HELPER(DocumentPropOutputWhitelist) arr = &getModuleConfig()->document.propWhitelistOut;
	DIRECTIVES_MODULE_HELPER(AssetMetadataParamInputWhitelist) arr = &getModuleConfig()->asset.propWhitelistIn;
	DIRECTIVES_MODULE_HELPER(AssetMetadataParamOutputWhitelist) arr = &getModuleConfig()->asset.propWhitelistOut;
	DIRECTIVES_MODULE_HELPER(UserProfileParamInputWhitelist) arr = &getModuleConfig()->user.propWhitelistIn;
	DIRECTIVES_MODULE_HELPER(UserProfileParamOutputWhitelist) arr = &getModuleConfig()->user.propWhitelistOut;
	
	if ( *arr == NULL ) {
		*arr = apr_array_make(getModuleConfig()->modulePool, CONFIG_ARRAY_INIT_SIZE, sizeof(char *));
	}
	char **newPtr = apr_array_push(*arr);
	*newPtr = apr_pcalloc(getModuleConfig()->modulePool, sizeof(char) * ( strlen(arg) + 1 ));
	strcpy(*newPtr, arg);

	return NULL;
}

const char *directives_parserTransformator(cmd_parms *cmd, void *cfg, const char *arg1, const char *arg2) {
	apr_table_t **transformOut = NULL;
	apr_table_t **transformIn = NULL;
	
	DIRECTIVES_MODULE_HELPER(AssetMetadataParamMapping) {
		transformOut = &getModuleConfig()->asset.propMappingOut;
		transformIn = &getModuleConfig()->asset.propMappingIn;
	}

	DIRECTIVES_MODULE_HELPER(UserProfileParamMapping) {
		transformOut = &getModuleConfig()->user.propMappingOut;
		transformIn = &getModuleConfig()->user.propMappingIn;
	}

	DIRECTIVES_MODULE_HELPER(DocumentPropMapping) {
		transformOut = &getModuleConfig()->document.propMappingOut;
		transformIn = &getModuleConfig()->document.propMappingIn;
	}

	if ( *transformOut == NULL ) {
		*transformOut = apr_table_make(getModuleConfig()->modulePool, CONFIG_TABLE_INIT_SIZE);
	}
	if ( *transformIn == NULL ) {
		*transformIn = apr_table_make(getModuleConfig()->modulePool, CONFIG_TABLE_INIT_SIZE);
	}

	apr_table_set(*transformOut, arg1, arg2);
	apr_table_set(*transformIn, arg2, arg1);
	return NULL;
}

const char *directives_parserSpecialAssetImageTransform(cmd_parms *cmd, void *cfg, const char *arg1, const char *arg2) {

	if ( getModuleConfig()->asset.assetTransform == NULL ) {
		getModuleConfig()->asset.assetTransform = apr_array_make(getModuleConfig()->modulePool, CONFIG_TABLE_INIT_SIZE, sizeof(module_config_transform_t));
	}

	module_config_transform_t *newPtr = apr_array_push(getModuleConfig()->asset.assetTransform);
	strncpy(newPtr->name, arg1, STRING_STD_LENGTH);
	newPtr->width = atoi(arg2);
	return NULL;
}

const char *directives_parseCreateList(cmd_parms *cmd, void *cfg, const char *args) {
	char *ptr;
	char *listName;
	char *query;
	mongo_config_query_list_t *queryList;

	ptr = (char *) args;
	while ( *ptr != '\0' && ( *ptr != ' ' && *ptr != '\t' ) ) {
		ptr++;
	}

	listName = apr_pcalloc(getModuleConfig()->modulePool, ptr - args + 1);
	strncpy(listName, args, ( ptr - args ));

	while ( *ptr != '\0' && ( *ptr == ' ' || *ptr == '\t' ) ) {
		ptr++;
	}

	query = apr_pcalloc(getModuleConfig()->modulePool, strlen(ptr) + 1);
	strcpy(query, ptr);

	queryList = apr_pcalloc(getModuleConfig()->modulePool, sizeof(mongo_config_query_list_t));
	queryList->map = apr_table_make(getModuleConfig()->modulePool, CONFIG_TABLE_INIT_SIZE);
	queryList->name = listName;
	jsonhandling_json2aprmap(queryList->map, query);

	DIRECTIVES_MODULE_HELPER(DocumentQueryList) {
		queryList->next = getModuleConfig()->document.queryList;
		getModuleConfig()->document.queryList = queryList;
	}
	DIRECTIVES_MODULE_HELPER(AssetQueryList) {
		queryList->next = getModuleConfig()->asset.queryList;
		getModuleConfig()->asset.queryList = queryList;
	}
	DIRECTIVES_MODULE_HELPER(UserQueryList) {
		queryList->next = getModuleConfig()->user.queryList;
		getModuleConfig()->user.queryList = queryList;
	}

	return NULL;
}