#include "maputil.h"
#include "../libs/json2map/config.h"
#include "../constants.h"

#ifdef COMMON_MAPUTIL_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "maputil", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "maputil")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

typedef struct {
	apr_array_header_t *arr;
	apr_table_t *resultMap;
} maputil_removeUnlistedElemements_t;

typedef struct {
	apr_table_t *resultMap;
	apr_table_t *mappingMap;
} maputil_mapMap_t;

typedef struct {
	apr_table_t *resultMap;
	char *prefixer;
} maputil_mapPrefix_t;

// Loop over map to remove unlisted elements
int maputil_removeUnlistedElementesLoop(void *data, const char *key, const char *value) {
	DEBUG_MSG("%s_removeUnlistedElementesLoop([void *], %s, %s)...", key, value);
	uint32_t i;
	uint8_t hit = 0;

	maputil_removeUnlistedElemements_t *container = (maputil_removeUnlistedElemements_t *) data;

	apr_array_header_t *arr = container->arr;

	char *arrKey;
	for ( i = 0; i < arr->nelts; i++ ) {
		arrKey = APR_ARRAY_IDX(arr, i, char *);
		if ( !strcmp(arrKey, key) ) {
			DEBUG_MSG("%s_removeUnlistedElementesLoop([void *], %s, %s): key was a hit in the array", key, value);
			hit = 1;
			break;
		}
	}

	if ( hit ) {
		apr_table_set(container->resultMap, key, value);
	}

	DEBUG_MSG("%s_removeUnlistedElementesLoop([void *], %s, %s)... DONE", key, value);
	return 1;
}

// method to remove unlisted elements from list
apr_table_t *maputil_removeUnlistedElements(apr_pool_t *pool, apr_table_t *map, apr_array_header_t *array) {
	DEBUG_PUT("%s_removeUnlistedElements([apr_pool_t *], [apr_table_t *], [apr_array_header_t *])...");
	if ( array != NULL && !apr_is_empty_array(array) ) {
		maputil_removeUnlistedElemements_t data;
		data.arr = array;
		data.resultMap = apr_table_make(pool, 1);

		apr_table_do(maputil_removeUnlistedElementesLoop, &data, map, NULL);
		DEBUG_PUT("%s_removeUnlistedElements([apr_pool_t *], [apr_table_t *], [apr_array_header_t *])... DONE");
		return data.resultMap;
	} else {
		DEBUG_PUT("%s_removeUnlistedElements([apr_pool_t *], [apr_table_t *], [apr_array_header_t *])... DONE");
		return map;
	}
}


int maputil_mapMapLoop(void *data, const char *key, const char *value) {
	DEBUG_MSG("%s_mapMapLoop([void *], %s, %s)...", key, value);

	maputil_mapMap_t *container = (maputil_mapMap_t *) data;
	const char *newKey;

	newKey = apr_table_get(container->mappingMap, key);
	if ( newKey != NULL ) {
		DEBUG_MSG("%s_mapMapLoop([void *], %s, %s): transformed key: %s -> %s", key, value, key, newKey);
		apr_table_set(container->resultMap, newKey, value);
	} else {
		apr_table_set(container->resultMap, key, value);
	}

	DEBUG_MSG("%s_mapMapLoop([void *], %s, %s)... DONE", key, value);
	return 1;
}


apr_table_t *maputil_mapMap(apr_pool_t *pool, apr_table_t *sourceMap, apr_table_t *mappingMap) {
	DEBUG_PUT("%s_maputil_mapMap([apr_pool_t *], [apr_table_t *], [apr_table_t *])...");
	
	maputil_mapMap_t data;
	data.resultMap = apr_table_make(pool, 1);
	data.mappingMap = mappingMap;

	if ( sourceMap != NULL && mappingMap != NULL && !apr_is_empty_table(sourceMap) && !apr_is_empty_table(mappingMap) ) {
		apr_table_do(maputil_mapMapLoop, &data, sourceMap, NULL);
		DEBUG_PUT("%s_maputil_mapMap([apr_pool_t *], [apr_table_t *], [apr_table_t *])... DONE");
		return data.resultMap;
	} else {
		DEBUG_PUT("%s_maputil_mapMap([apr_pool_t *], [apr_table_t *], [apr_table_t *]): One of the maps is empty");
		DEBUG_PUT("%s_maputil_mapMap([apr_pool_t *], [apr_table_t *], [apr_table_t *])... DONE");
		return sourceMap;
	}
}


long maputil_getMaxArrayId(apr_pool_t *pool, apr_table_t *map, char *matchString) {
	DEBUG_MSG("%s_getMaxArrayId([apr_pool_t *], [apr_table_t *], %s)...", matchString);

	char *keyStr = apr_pcalloc(pool, strlen(matchString) + 4);
	sprintf(keyStr, "%s%c%c%c", matchString, JSON2MAP_MAP_ARRAY_START, JSON2MAP_MAP_ARRAY_COUNT, JSON2MAP_MAP_ARRAY_END);

	const char *value = apr_table_get(map, keyStr);

	DEBUG_MSG("%s_getMaxArrayId([apr_pool_t *], [apr_table_t *], %s): value=%s, atoi(value)=%d", matchString, value, atoi(value));

	DEBUG_MSG("%s_getMaxArrayId([apr_pool_t *], [apr_table_t *], %s)... DONE", matchString);

	return atoi(value);
}

int maputil_moveMapElementsToPrefixLoop(void *data, const char *key, const char *value) {
	char buffer[BUFFER_SIZE];
	maputil_mapPrefix_t *container = (maputil_mapPrefix_t *) data;

	sprintf(buffer, "%s%s", container->prefixer, key);
	apr_table_set(container->resultMap, buffer, value);

	return 1;
}


apr_table_t *maputil_moveMapElementsToPrefix(apr_pool_t *pool, apr_table_t *sourceMap, char *prefixer) {
	DEBUG_MSG("%s_moveMapElementsToPrefix([apr_pool_t *], [apr_table_t *], %s)...", prefixer);

	maputil_mapPrefix_t data;
	data.resultMap = apr_table_make(pool, 1);
	data.prefixer = prefixer;

	apr_table_do(maputil_moveMapElementsToPrefixLoop, &data, sourceMap, NULL);

	DEBUG_MSG("%s_moveMapElementsToPrefix([apr_pool_t *], [apr_table_t *], %s)... DONE", prefixer);

	return data.resultMap;
}


int maputil_mergeMapsLoop(void *data, const char *key, const char *value) {
	DEBUG_MSG("%s_mergeMapsLoop([void *], %s, %s)...", key, value);
	apr_table_t *mergedMap = (apr_table_t *) data;
	apr_table_set(mergedMap, key, value);
	DEBUG_MSG("%s_mergeMapsLoop([void *], %s, %s)... DONE", key, value);
	return 1;
}

apr_table_t *maputil_mergeMaps(apr_pool_t *pool, apr_table_t *mergedMap, apr_table_t *mapToMerge) {
	DEBUG_PUT("%s_mergeMaps([apr_pool_t *], [apr_table_t *], [apr_table_t *])...");

	apr_table_t *resultMap = apr_table_clone(pool, mergedMap);
	apr_table_do(maputil_mergeMapsLoop, resultMap, mapToMerge, NULL);
	DEBUG_PUT("%s_mergeMaps([apr_pool_t *], [apr_table_t *], [apr_table_t *])... DONE");
	return resultMap;
}