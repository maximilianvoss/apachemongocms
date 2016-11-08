#include "assetshandler.h"
#include "../mod_mongocms.h"
#include "../common/fileutil.h"
#include "metadata/metadata.h"
#include "image/rendition.h"
#include "image/upload.h"
#include "../common/stringutil.h"
#include "../common/entryhelper.h"

int assets_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename);

// handle the actual request
int assets_handler(request_rec *request, char *filename) {
	char *suffix = fileutil_getSuffix(filename);

	if ( !strcmp(suffix, "json") ) {
		switch ( request->method_number ) {
			case M_GET:
				if ( stringutil_startsWith(filename, "list.") )
					return assets_getEntryList(&getModuleConfig()->asset.database, request, filename);
				else
					return metadata_get(&getModuleConfig()->asset.database, request);
			case M_POST:
			case M_PUT:
				return metadata_set(&getModuleConfig()->asset.database, request);
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

int assets_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename) {
	return entryhelper_getEntryList(request, mongoConfig, getModuleConfig()->asset.queryList, getModuleConfig()->asset.propWhitelistOut, getModuleConfig()->asset
			.propMappingOut, filename);
}