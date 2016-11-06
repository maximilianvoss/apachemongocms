#include "jsonhandling.h"
#include "../libs/json2map/json2map.h"
#include "../libs/json2map/map2json.h"

#ifdef COMMON_JSONHANDLING_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "jsonhandling", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "jsonhandling")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

// add key values to a hashmap (hook for interaction over a apache map)
void jsonhandling_mapHook(void *data, char *key, char *value) {
	DEBUG_MSG("%s_mapHook([data], %s, %s)...", key, value);
	apr_table_t *map = (apr_table_t *) data;
	apr_table_set(map, key, value);
	DEBUG_MSG("%s_mapHook([data], %s, %s)... DONE", key, value);
}

// transform a json into an aprmap
void jsonhandling_json2aprmap(apr_table_t *map, char *json) {
	DEBUG_MSG("%s_json2aprmap([map], %s)...", json);

	json2map_t *json2mapObj = json2map_init();
	json2map_registerHook(json2mapObj, (void *) map, &jsonhandling_mapHook);
	json2map_parse(json2mapObj, json);
	json2map_destroy(json2mapObj);

	DEBUG_MSG("%s_json2aprmap([map], %s)... DONE", json);
}

// add data to the map2jsonObject
int jsonhandling_appendMapData(void *data, const char *key, const char *value) {
	DEBUG_MSG("%s_appendMapData([data], %s, %s)...", key, value);

	map2json_t *obj = (map2json_t *) data;
	map2json_push(obj, (char *) key, (char *) value);

	DEBUG_MSG("%s_appendMapData([data], %s, %s)... DONE", key, value);
	return 1;
}

// transform aprmap to json string
char *jsonhandling_aprmap2json(apr_pool_t *pool, apr_table_t *map) {
	DEBUG_PUT("%s_aprmap2json([pool], [map])...");
	map2json_t *map2jsonObj = map2json_init();

	apr_table_do(jsonhandling_appendMapData, map2jsonObj, map, NULL);

	DEBUG_PUT("%s_aprmap2json([pool], [map]): create map2json object");
	map2json_create(map2jsonObj);
	size_t length = strlen(map2jsonObj->buffer);

	char *jsonString = apr_pcalloc(pool, length);
	strcpy(jsonString, map2jsonObj->buffer);

	DEBUG_PUT("%s_aprmap2json([pool], [map]): destroy map2json object");
	map2json_destroy(map2jsonObj);

	DEBUG_MSG("%s_aprmap2json([pool], [map]): json=%s", jsonString);
	DEBUG_PUT("%s_aprmap2json([pool], [map])... DONE");
	return jsonString;
}