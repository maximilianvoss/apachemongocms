#ifndef __JSON2MAP_DEBUGGING_H__
#define __JSON2MAP_DEBUGGING_H__

#include "../../mod_mongocms.h"

#undef DEBUG

#ifdef DEBUG
#define DEBUG_TEXT(fmt, ...) ap_log_error("Unknown File", 0, 1, 1, 99, getModuleConfig()->serverRec, fmt, __VA_ARGS__)
#define DEBUG_PUT(fmt) ap_log_error("Unknown File", 0, 1, 1, 99, getModuleConfig()->serverRec, fmt)
#else
#define DEBUG_TEXT(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

#endif
