#include "entryhelper.h"
#include "fileutil.h"
#include "stringutil.h"
#include "../user/user.h"
#include "jsonhandling.h"

#ifdef COMMON_ENTRYHELPER_DEBUG

#include "logging.h"
#include "maputil.h"
#include "jsonmapfilter.h"
#include "requesthelper.h"
#include "../libs/json2map/config.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "entryhelper", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "entryhelper")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

char *entryhelper_getEntryInternal(request_rec *request, apr_array_header_t *whitelistOut, apr_table_t *mappingOut, apr_table_t *map);

// get entry by id
apr_table_t *entryhelper_getEntryById(mongo_config_t *config, apr_pool_t *pool, char *entryId) {
	DEBUG_MSG("%s_getEntryById([mongo_connection_t *], [apr_pool_t *], %s)...", entryId);

	apr_table_t *entryMap = apr_table_make(pool, CONFIG_TABLE_INIT_SIZE);

	if ( entryId == NULL ) {
		return entryMap;
	}

	apr_table_t *queryMap = apr_table_make(pool, CONFIG_TABLE_INIT_SIZE);
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
uint8_t entryhelper_isEntryWritable(apr_pool_t *pool, apr_table_t *userMap, apr_table_t *map) {
	DEBUG_PUT("%s_isEntryWritable([apr_pool_t *], [apr_table_t*], [apr_table_t *])...");

	char key[BUFFER_SIZE];
	long maxArrayId = maputil_getMaxArrayId(pool, map, MONGO_PRIVILEGES_WRITING);

	const char *userId = user_getUserId(userMap);

	for ( long i = 0; i < maxArrayId + 1; i++ ) {
		sprintf(key, "%s[%ld]", MONGO_PRIVILEGES_WRITING, i);
		const char *value = apr_table_get(map, key);
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


int entryhelper_updateEntry(request_rec *request, mongo_config_t *mongoConfig, apr_array_header_t *whitelistIn, apr_array_header_t *whitelistOut, apr_table_t *mappingIn, apr_table_t *mappingOut) {
	DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *])...");

	char buffer[BUFFER_SIZE];
	apr_table_t *userMap = user_getUserMap(request);

	char *documentId = entryhelper_getEntryId(request);
	apr_table_t *documentMap = entryhelper_getEntryById(mongoConfig, request->pool, documentId);

	apr_table_t *requestMap = requesthelper_getPostMap(request, mappingIn, whitelistIn);
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

		newDocumentMap = maputil_mergeMaps(request->pool, documentMap, requestMap);
		
		sprintf(buffer, "%s#$timestamp#t", MONGO_MODIFIEDAT);
		apr_table_set(newDocumentMap, buffer, stringutil_longToString(request->pool, (long) time(NULL)));

		sprintf(buffer, "%s#$timestamp#i", MONGO_MODIFIEDAT);
		apr_table_set(newDocumentMap, buffer, "0");

		apr_table_set(newDocumentMap, MONGO_MODIFIEDBY, user_getUserName(userMap));

		mongo_update(mongoConfig, request->pool, documentMap, newDocumentMap);
	}

	DEBUG_PUT("%s_updateEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *])... DONE");
	ap_rputs(entryhelper_getEntryInternal(request, whitelistOut, mappingOut, newDocumentMap), request);
	return OK;
}

char *entryhelper_getEntryInternal(request_rec *request, apr_array_header_t *whitelistOut, apr_table_t *mappingOut, apr_table_t *map) {
	DEBUG_PUT("%s_getEntryInternal([request_rec *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *])...");
	apr_table_t *whitelistedMap = maputil_removeUnlistedElements(request->pool, map, whitelistOut);
	DEBUG_PUT("%s_getEntryInternal([request_rec *], [apr_array_header_t *], [apr_table_t *], [apr_table_t *])... DONE");
	return jsonmapfilter_outboundMap(request->pool, whitelistedMap, mappingOut);
	
}


int entryhelper_getEntry(request_rec *request, mongo_config_t *mongoConfig, apr_array_header_t *whitelistOut, apr_table_t *mappingOut) {
	DEBUG_PUT("%s_getEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_table_t *])...");

	char *documentId = entryhelper_getEntryId(request);
	apr_table_t *documentMap = entryhelper_getEntryById(mongoConfig, request->pool, documentId);

	DEBUG_PUT("%s_getEntry([request_rec *], [mongo_config_t *], [apr_array_header_t *], [apr_table_t *])... DONE");
	ap_rputs(entryhelper_getEntryInternal(request, whitelistOut, mappingOut, documentMap), request);
	return OK;
}

int entryhelper_getEntryList(request_rec *request, mongo_config_t *mongoConfig, mongo_config_query_list_t *queryList, apr_array_header_t *whitelistOut, apr_table_t *mappingOut, char *filename) {
	DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s)...", filename);
	char *listName;

	if ( queryList == NULL ) {
		DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s): queryList==NULL", filename);
		DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s)... DONE", filename);
		return HTTP_FORBIDDEN;
	}

	int pos1 = stringutil_indexOf(filename, '.');
	int pos2 = stringutil_lastIndexOf(filename, '.');
	if ( pos1 == pos2 ) {
		DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s): only one dot", filename);
		DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s)... DONE", filename);
		return HTTP_FORBIDDEN;
	}

	listName = apr_pcalloc(request->pool, pos2 - pos1);
	strncpy(listName, &filename[pos1 + 1], pos2 - pos1 - 1);

	while ( queryList != NULL ) {
		if ( !strcmp(queryList->name, listName) ) {
			break;
		}
		queryList = queryList->next;
	}

	if ( queryList == NULL ) {
		DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s): no fitting queryList found", filename);
		DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s)... DONE", filename);
		return HTTP_FORBIDDEN;
	}

	apr_table_t *resultMap = apr_table_make(request->pool, CONFIG_TABLE_INIT_SIZE);
	apr_table_t *documentMap = apr_table_make(request->pool, CONFIG_TABLE_INIT_SIZE);
	bson_t *doc;
	int i = 0;
	char buffer[BUFFER_SIZE];
	mongo_cursor_t *cursor = mongo_query(mongoConfig, request->pool, queryList->map);
	while ( mongoc_cursor_next(cursor->cursor, (const bson_t **) &doc) ) {
		mongo_bson2map(documentMap, doc);

		apr_table_t *whitelistedMap = maputil_removeUnlistedElements(request->pool, documentMap, whitelistOut);
		apr_table_t *objectMap = maputil_mapMap(request->pool, whitelistedMap, mappingOut);

		sprintf(buffer, "result%c%d%c%c", JSON2MAP_MAP_ARRAY_START, i, JSON2MAP_MAP_ARRAY_END, JSON2MAP_MAP_OBJECT_SEPARATOR);
		apr_table_t *movedMap = maputil_moveMapElementsToPrefix(request->pool, objectMap, buffer);

		apr_table_overlap(resultMap, movedMap, APR_OVERLAP_TABLES_SET);
		i++;
	}
	mongo_destroyCursor(cursor);

	ap_rprintf(request, "%s\n", jsonhandling_aprmap2json(request->pool, resultMap));

	DEBUG_MSG("%s_getEntryList([request_rec *], [mongo_config_t *], [mongo_config_query_list_t *] [apr_array_header_t *], [apr_table_t *], %s)... DONE", filename);
	return OK;
}