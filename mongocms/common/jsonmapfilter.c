#include "jsonmapfilter.h"
#include "maputil.h"
#include "jsonhandling.h"

#ifdef COMMON_JSONMAPFILTER_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "jsonmapfilter", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "jsonmapfilter")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

apr_table_t *jsonmapfilter_inboundMap(apr_pool_t *pool, apr_table_t *map, apr_table_t *mappingTable) {
	DEBUG_PUT("%s_inboundMap([apr_pool_t *], [apr_table_t *], [apr_table_t *])");

	return maputil_mapMap(pool, map, mappingTable);
}

char *jsonmapfilter_outboundMap(apr_pool_t *pool, apr_table_t *map, apr_table_t *mappingTable) {
	DEBUG_PUT("%s_outboundMap([apr_pool_t *], [apr_table_t *], [apr_table_t *])...");
	apr_table_t *resultMap = maputil_mapMap(pool, map, mappingTable);
	char *json = jsonhandling_aprmap2json(pool, resultMap);
	DEBUG_PUT("%s_outboundMap([apr_pool_t *], [apr_table_t *], [apr_table_t *])... DONE");
	return json;
}
