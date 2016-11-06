#ifndef __COMMON_JSONMAPFILTER_H__
#define __COMMON_JSONMAPFILTER_H__

#include "../includes.h"

//#define COMMON_JSONMAPFILTER_DEBUG

apr_table_t *jsonmapfilter_inboundMap(apr_pool_t *pool, apr_table_t *map, apr_table_t *mappingTable);
char *jsonmapfilter_outboundMap(apr_pool_t *pool, apr_table_t *map, apr_table_t *mappingTable);

#endif 
