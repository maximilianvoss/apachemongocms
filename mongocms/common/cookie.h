#ifndef __COMMON_COOKIE_H__
#define __COMMON_COOKIE_H__

#include "../includes.h"
#include "../config.h"
#include <time.h>

//#define COMMON_COOKIE_DEBUG

void cookie_setCookie(request_rec *request, char *name, char *value, time_t expireDate, char *domain, char *path);
char *cookie_getCookie(request_rec *request, char *name);

#endif 
