#include "assetshandler.h"
#include "../mod_mongocms.h"
#include "../common/fileutil.h"
#include "image/rendition.h"
#include "image/upload.h"
#include "../common/stringutil.h"
#include "../common/entryhelper.h"
#include "../common/requesthelper.h"

int asset_set(mongo_config_t *mongoConfig, request_rec *request);
int asset_get(mongo_config_t *mongoConfig, request_rec *request);
int asset_delete(mongo_config_t *mongoConfig, request_rec *request);
int assets_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename);

// handle the actual request
int assets_handler(request_rec *request, char *filename) {
	char *suffix = fileutil_getSuffix(filename);

	if ( !strcmp(suffix, "json") ) {
		switch ( request->method_number ) {
			case M_GET:
				if ( !strcmp(filename, FILENAME_ALL_DELETE)) {
					return asset_delete(&getModuleConfig()->asset.database, request);
				} else if ( stringutil_startsWith(filename, "list.") )
					return assets_getEntryList(&getModuleConfig()->asset.database, request, filename);
				else
					return asset_get(&getModuleConfig()->asset.database, request);
			case M_POST:
				if ( !strcmp(filename, FILENAME_ALL_DELETE)) {
					return asset_delete(&getModuleConfig()->asset.database, request);
				}
			case M_PUT:
				return asset_set(&getModuleConfig()->asset.database, request);
			default:
				return DECLINED;
		}
	} else {
		switch ( request->method_number ) {
			case M_GET:
				return rendition_get(&getModuleConfig()->asset.database, request, request->filename);
			case M_POST:
			case M_PUT:
				return upload_image(&getModuleConfig()->asset.database, request);
			default:
				return DECLINED;
		}
	}
}

int asset_set(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_updateEntry(request, mongoConfig, getModuleConfig()->asset.propWhitelistIn, getModuleConfig()->asset.propWhitelistOut, getModuleConfig()->asset
			.propMappingIn, getModuleConfig()->asset.propMappingOut);
}

int asset_get(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_getEntry(request, mongoConfig, getModuleConfig()->asset.propWhitelistOut, getModuleConfig()->asset.propMappingOut);
}

int asset_delete(mongo_config_t *mongoConfig, request_rec *request) {
	apr_table_t *map;
	if (request->method_number == M_GET) {
		map = requesthelper_getGetMap(request, getModuleConfig()->asset.propMappingIn, getModuleConfig()->asset.propWhitelistIn);
	} else {
		map = requesthelper_getPostMap(request, getModuleConfig()->asset.propMappingIn, getModuleConfig()->asset.propWhitelistIn);
	}
	return entryhelper_deleteEntry(request, mongoConfig, map);
}

int assets_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename) {
	return entryhelper_getEntryList(request, mongoConfig, getModuleConfig()->asset.queryList, getModuleConfig()->asset.propWhitelistIn, getModuleConfig()->asset.propWhitelistOut, getModuleConfig()
			->asset.propMappingIn, getModuleConfig()->asset.propMappingOut, filename);
}