#ifndef __COMMON_REQUESTHELPER_H__
#define __COMMON_REQUESTHELPER_H__

#include "../includes.h"

apr_table_t *requesthelper_getPostMap(request_rec *request, apr_table_t *mappingIn, apr_array_header_t *whitelistIn);
apr_table_t *requesthelper_getGetMap(request_rec *request, apr_table_t *mappingIn, apr_array_header_t *whiltelistIn);
apr_status_t requesthelper_saveRequestToFile(request_rec *request, const char *filename);

#endif