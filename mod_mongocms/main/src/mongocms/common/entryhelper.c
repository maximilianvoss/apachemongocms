#include "entryhelper.h"
#include "fileutil.h"
#include "stringutil.h"
#include "../user/user.h"

#ifdef COMMON_ENTRYHELPER_DEBUG

#include "logging.h"
#include "maputil.h"
#include "jsonmapfilter.h"
#include "requesthelper.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "entryhelper", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "entryhelper")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

int entryhelper_getEntryInternal(request_rec *request, apr_array_header_t *outputWhitelist, apr_table_t *map, char *documentId, apr_table_t *documentMap);

// get entry by id
apr_table_t *entryhelper_getEntryById(mongo_config_t *config, apr_pool_t *pool, char *entryId) {
	DEBUG_MSG("%s_getEntryById([mongo_connection_t *], [apr_pool_t *], %s)...", entryId);

	apr_table_t *entryMap = apr_table_make(pool, 30);

	if ( entryId == NULL ) {
		return entryMap;
	}

	apr_table_t *queryMap = apr_table_make(pool, 1);
	apr_table_set(queryMap, MONGO_OID, entryId);

	mongo_cursor_t *cursor = mongo_query(config, pool, queryMap);
	bson_t *doc;
	if ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		DEBUG_MSG("%s_getEntryById([mongo_connection_t *], [request_rec *], %s): Entry found", entryId);
		mongo_bson2map(entryMap, doc);
	} else {
		DEBUG_MSG("%s_getEntryById([mongo_connection_t *], [request_rec *], %s): Entry not found", entryId);
	}
	mongo_destroyCursor(cursor);
	DEBUG_MSG("%s_getEntryById([mongo_connection_t *], [request_rec *], %s)... DONE", entryId);
	return entryMap;
}

// check if entry is writable
uint8_t entryhelper_isEntryWritable(apr_pool_t *pool, apr_table_t *userMap, apr_table_t *entryMap) {
	DEBUG_PUT("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *])...");

	char key[BUFFER_SIZE];
	long maxArrayId = maputil_getMaxArrayId(pool, entryMap, MONGO_PRIVILEGES_WRITING);

	const char *userId = user_getUserId(userMap);

	for ( long i = 0; i < maxArrayId + 1; i++ ) {
		sprintf(key, "%s[%ld]", MONGO_PRIVILEGES_WRITING, i);
		const char *value = apr_table_get(entryMap, key);
		DEBUG_MSG("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *]): User name in array: %s", value);

		if ( !strcmp(value, userId) ) {
			DEBUG_PUT("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *]): User matches");
			DEBUG_PUT("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *])... DONE");
			return 1;
		}
	}
	DEBUG_PUT("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *]): User doesnt match");
	DEBUG_PUT("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *])... DONE");
	return 0;
}

// get entry id
char *entryhelper_getEntryId(request_rec *request) {
	DEBUG_PUT("%s_getEntryId([request_rec *])...");
	char *filename = request->filename;
	char *entryId = NULL;

	if ( filename != NULL ) {
		entryId = (char *) apr_pcalloc(request->pool, strlen(fileutil_getFilename(filename)));
		strcpy(entryId, fileutil_getFilename(filename));
		entryId[stringutil_indexOf(entryId, '.')] = '\0';
	}

	if ( !mongo_isOidValid(entryId) ) {
		entryId = NULL;
	}
	
	DEBUG_MSG("%s_getEntryId([request_rec *]): entryId: %s", entryId);
	DEBUG_PUT("%s_getEntryId([request_rec *])... DONE");
	return entryId;
}


int entryhelper_updateEntry(request_rec *request, mongo_config_t *mongoConfig, apr_array_header_t *inputWhiteList, apr_array_header_t *outputWhitelist, apr_table_t *map, apr_table_t *inverseMap) {
	DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *])...");

	char buffer[BUFFER_SIZE];
	apr_table_t *userMap = user_getUserMap(request);

	char *documentId = entryhelper_getEntryId(request);
	apr_table_t *documentMap = entryhelper_getEntryById(mongoConfig, request->pool, documentId);

	apr_table_t *requestMap = requesthelper_getPostMap(request, inverseMap, inputWhiteList);
	apr_table_t *newDocumentMap;


	if ( apr_is_empty_table(documentMap) ) {
		DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *]): Document has to be created");

		newDocumentMap = requestMap;
		sprintf(buffer, "%s#$timestamp#t", MONGO_CREATEDAT);
		apr_table_set(newDocumentMap, buffer, stringutil_longToString(request->pool, (long) time(NULL)));

		sprintf(buffer, "%s#$timestamp#i", MONGO_CREATEDAT);
		apr_table_set(newDocumentMap, buffer, "0");

		sprintf(buffer, "%s#$timestamp#t", MONGO_MODIFIEDAT);
		apr_table_set(newDocumentMap, buffer, stringutil_longToString(request->pool, (long) time(NULL)));

		sprintf(buffer, "%s#$timestamp#i", MONGO_MODIFIEDAT);
		apr_table_set(newDocumentMap, buffer, "0");

		apr_table_set(newDocumentMap, MONGO_CREATEDBY, user_getUserName(userMap));
		apr_table_set(newDocumentMap, MONGO_MODIFIEDBY, user_getUserName(userMap));

		sprintf(buffer, "%s[0]", MONGO_PRIVILEGES_WRITING);
		apr_table_set(newDocumentMap, buffer, user_getUserId(userMap));

		documentId = mongo_commit(mongoConfig, request->pool, newDocumentMap);
		apr_table_set(newDocumentMap, MONGO_OID, documentId);
	} else {
		DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *]): Document can be modified");

		if ( !entryhelper_isEntryWritable(request->pool, userMap, documentMap) ) {
			DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *]): Entry is not writable to user");
			return HTTP_FORBIDDEN;
		}

		newDocumentMap = apr_table_clone(request->pool, documentMap);
		maputil_mergeMaps(newDocumentMap, requestMap);

		sprintf(buffer, "%s#$timestamp#t", MONGO_MODIFIEDAT);
		apr_table_set(newDocumentMap, buffer, stringutil_longToString(request->pool, (long) time(NULL)));

		sprintf(buffer, "%s#$timestamp#i", MONGO_MODIFIEDAT);
		apr_table_set(newDocumentMap, buffer, "0");

		apr_table_set(newDocumentMap, MONGO_MODIFIEDBY, user_getUserName(userMap));

		mongo_update(mongoConfig, request->pool, documentMap, newDocumentMap);
	}

	DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *])... DONE");
	return entryhelper_getEntryInternal(request, outputWhitelist, map, documentId, newDocumentMap);
}

int entryhelper_getEntryInternal(request_rec *request, apr_array_header_t *outputWhitelist, apr_table_t *map, char *documentId, apr_table_t *documentMap) {
	DEBUG_MSG("%s_getEntryInternal([request_rec *], [apr_array_header_t *], [apr_table_t *], %s, [apr_table_t *])...", documentId);
	apr_table_t *whitelistedMap = maputil_removeUnlistedElements(request->pool, documentMap, outputWhitelist);
	ap_rputs(jsonmapfilter_outboundMap(request->pool, whitelistedMap, map), request);

	DEBUG_MSG("%s_getEntryInternal([request_rec *], [apr_array_header_t *], [apr_table_t *], %s, [apr_table_t *])... DONE", documentId);
	return OK;
}


int entryhelper_getEntry(request_rec *request, mongo_config_t *mongoConfig, apr_array_header_t *outputWhitelist, apr_table_t *map) {
	DEBUG_PUT("%s_getEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_table_t *])...");

	char *documentId = entryhelper_getEntryId(request);
	apr_table_t *documentMap = entryhelper_getEntryById(mongoConfig, request->pool, documentId);

	DEBUG_PUT("%s_getEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_table_t *])... DONE");
	return entryhelper_getEntryInternal(request, outputWhitelist, map, documentId, documentMap);
}
