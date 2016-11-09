#include "documents.h"
#include "../mod_mongocms.h"
#include "../common/entryhelper.h"
#include "../common/stringutil.h"

int documents_set(mongo_config_t *mongoConfig, request_rec *request);
int documents_get(mongo_config_t *mongoConfig, request_rec *request);
int document_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename);

int documents_handler(request_rec *request, char *filename) {
	switch ( request->method_number ) {
		case M_GET:
			if ( stringutil_startsWith(filename, "list.") )
				return document_getEntryList(&getModuleConfig()->document.database, request, filename);
			else
				return documents_get(&getModuleConfig()->document.database, request);
		case M_POST:
			return documents_set(&getModuleConfig()->document.database, request);
		default:
			return DECLINED;
	}
}

int documents_set(mongo_config_t *mongoConfig, request_rec *request) {
	int status = entryhelper_updateEntry(request, mongoConfig, getModuleConfig()->document.propWhitelistIn, getModuleConfig()->document.propWhitelistOut,
	                                     getModuleConfig()->document.propMappingIn,
	                                     getModuleConfig()->document.propMappingOut);
	return status;
}


int documents_get(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_getEntry(request, mongoConfig, getModuleConfig()->document.propWhitelistOut, getModuleConfig()->document.propMappingOut);
}


int document_getEntryList(mongo_config_t *mongoConfig, request_rec *request, char *filename) {
	return entryhelper_getEntryList(request, mongoConfig, getModuleConfig()->document.queryList, getModuleConfig()->document.propWhitelistIn, getModuleConfig()->document.propWhitelistOut,
	                                getModuleConfig()->document.propMappingIn, getModuleConfig()->document.propMappingOut, filename);
}