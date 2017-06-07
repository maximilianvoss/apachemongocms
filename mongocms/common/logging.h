#ifndef __COMMON_LOGGING_H__
#define __COMMON_LOGGING_H__

#include "../mod_mongocms.h"

#define LOG_SERVER_ERROR(file, line, status, fmt, ...) ap_log_error(file, line, 1, 1, status, PublisherApp.config()->serverRec, fmt, __VA_ARGS__)
#define LOG_REQUEST_ERROR(file, line, status, request, fmt, ...) ap_log_rerror(file, line, 1, 1, status, request, fmt, __VA_ARGS__)
#define LOG_SERVER_DEBUG_FORMAT(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error("Unknown File", 0, 1, 1, 99, getModuleConfig()->serverRec, fmt, __VA_ARGS__)

//// LOGGING LEVEL
//// ERROR    0
//// WARN     1
//// INFO     2
//// DEBUG    3
//// TRACE    4

#define LOGGING_ERROR(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error(__FILE__, __LINE__, 1, APLOG_ERR, 0, getModuleConfig()->serverRec, fmt, ##__VA_ARGS__)
#define LOGGING_WARN(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error(__FILE__, __LINE__, 1, APLOG_WARNING, APR_SUCCESS, getModuleConfig()->serverRec, fmt, ##__VA_ARGS__)
#define LOGGING_INFO(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error(__FILE__, __LINE__, 1, APLOG_INFO, APR_SUCCESS, getModuleConfig()->serverRec, fmt, ##__VA_ARGS__)
#define LOGGING_DEBUG(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error(__FILE__, __LINE__, 1, APLOG_DEBUG, APR_SUCCESS, getModuleConfig()->serverRec, fmt, ##__VA_ARGS__)
#define LOGGING_TRACE(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error(__FILE__, __LINE__, 1, APLOG_TRACE1, APR_SUCCESS, getModuleConfig()->serverRec, fmt, ##__VA_ARGS__)

#endif
