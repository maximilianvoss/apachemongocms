#ifndef __COMMON_MAPUTIL_H__
#define __COMMON_MAPUTIL_H__

#include "../includes.h"

//#define COMMON_MAPUTIL_DEBUG

apr_table_t *maputil_removeUnlistedElements(apr_pool_t *pool, apr_table_t *map, apr_array_header_t *array);
apr_table_t *maputil_mapMap(apr_pool_t *pool, apr_table_t *sourceMap, apr_table_t *mappingMap);
long maputil_getMaxArrayId(apr_pool_t *pool, apr_table_t *map, char *matchString);
apr_table_t *maputil_moveMapElementsToPrefix(apr_pool_t *pool, apr_table_t *sourceMap, char *prefixer);
apr_table_t *maputil_mergeMaps(apr_pool_t *pool, apr_table_t *mergedMap, apr_table_t *mapToMerge);

#endif
