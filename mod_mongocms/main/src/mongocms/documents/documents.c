#include "documents.h"
#include "../mod_mongocms.h"
#include "../common/entryhelper.h"
#include "../common/fileutil.h"

int documents_set(mongo_config_t *mongoConfig, request_rec *request);
int documents_get(mongo_config_t *mongoConfig, request_rec *request);

int documents_handler(request_rec *request) {
	switch ( request->method_number ) {
		case M_GET:
			return documents_get(&getModuleConfig()->document.database, request);
		case M_POST:
			return documents_set(&getModuleConfig()->document.database, request);
		default:
			return DECLINED;
	}
}

int documents_set(mongo_config_t *mongoConfig, request_rec *request) {
	int status = entryhelper_updateEntry(request, mongoConfig, getModuleConfig()->document.documentPropInputWhitelist, getModuleConfig()->document.documentPropOutputWhitelist,
	                                     getModuleConfig()->document.documentPropMapping,
	                                     getModuleConfig()->document.documentPropMappingInverse);
	return status;
}


int documents_get(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_getEntry(request, mongoConfig, getModuleConfig()->document.documentPropOutputWhitelist, getModuleConfig()->document.documentPropMapping);
}
