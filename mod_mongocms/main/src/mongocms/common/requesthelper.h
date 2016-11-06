#ifndef __COMMON_REQUESTHELPER_H__
#define __COMMON_REQUESTHELPER_H__

#include "../includes.h"

apr_table_t *requesthelper_getPostMap(request_rec *request, apr_table_t *inverseMap, apr_array_header_t *inputWhiteList);
apr_status_t requesthelper_saveRequestToFile(request_rec *request, const char *filename);

#endif