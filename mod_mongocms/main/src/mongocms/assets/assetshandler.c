#include "assetshandler.h"
#include "../mod_mongocms.h"
#include "../common/fileutil.h"
#include "metadata/metadata.h"
#include "image/rendition.h"
#include "image/upload.h"

// handle the actual request
int assets_handler(request_rec *request) {
	char *filename = request->filename;
	char *suffix = fileutil_getSuffix(filename);

	if ( !strcmp(suffix, "json") ) {
		switch ( request->method_number ) {
			case M_GET:
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
				return rendition_get(&getModuleConfig()->asset.database, request, filename);
			case M_POST:
			case M_PUT:
				return upload_image(&getModuleConfig()->asset.database, request);
			default:
				return DECLINED;
		}
	}
}

