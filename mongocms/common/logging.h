#ifndef __COMMON_LOGGING_H__
#define __COMMON_LOGGING_H__

#include "../mod_mongocms.h"

#define LOG_SERVER_ERROR(file, line, status, fmt, ...) ap_log_error(file, line, 1, 1, status, PublisherApp.config()->serverRec, fmt, __VA_ARGS__)
#define LOG_REQUEST_ERROR(file, line, status, request, fmt, ...) ap_log_rerror(file, line, 1, 1, status, request, fmt, __VA_ARGS__)
#define LOG_SERVER_DEBUG_FORMAT(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error("Unknown File", 0, 1, 1, 99, getModuleConfig()->serverRec, fmt, __VA_ARGS__)

//AP_DECLARE(void) ap_log_error(const char *file, int line, int module_index,
//                              int level, apr_status_t status,
//                              const server_rec *s, const char *fmt, ...);


//// LOGGING LEVEL
//// ERROR    0
//// WARN     1
//// INFO     2
//// DEBUG    3
//// TRACE    4
//
//#ifndef LOGGING_LEVEL
//#define LOGGING_LEVEL 0
//#endif
//
//#define LOGGING_ERROR(fmt, ...) { fprintf(stderr, "ERROR: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr,"\n"); }
//#define LOGGING_WARN(fmt, ...) if ( LOGGING_LEVEL > 0 ) { fprintf(stderr, "WARN: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr,"\n"); }
//#define LOGGING_INFO(fmt, ...) if ( LOGGING_LEVEL > 1 ) { fprintf(stderr, "INFO: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr,"\n"); }
//#define LOGGING_DEBUG(fmt, ...) if ( LOGGING_LEVEL > 2 ) { fprintf(stderr, "DEBUG: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr, "\n"); }
//#define LOGGING_TRACE(fmt, ...) if ( LOGGING_LEVEL > 3 ) { fprintf(stderr, "TRACE: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr, "\n"); }

#define LOGGING_ERROR(fmt, ...) if(getModuleConfig()->serverRec != NULL) ap_log_error(__FILE__, __LINE__, 1, APRLOG_ERR, APR_SUCCESS, getModuleConfig()->serverRec, fmt, ##__VA_ARGS__)

#endif
