#include "cookie.h"
#include "stringutil.h"

#ifdef COMMON_COOKIE_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "cookie", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "cookie")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

void cookie_setCookie(request_rec *request, char *name, char *value, time_t expireDate, char *domain, char *path) {
	DEBUG_MSG("%s_setCookie([request_rec *], %s, %s, %ld, %s, %s)...", name, value, expireDate, domain, path);

	char *buffer = apr_pcalloc(request->pool, sizeof(char) * SMALL_BUFFER_SIZE);
	char *ptr = buffer;

	if ( name != NULL ) {
		strcpy(ptr, name);
		ptr += strlen(name);

		*ptr = '=';
		ptr++;
	}

	if ( value != NULL ) {
		strcpy(ptr, value);
		ptr += strlen(value);
	}

	if ( expireDate != 0 ) {
		strcpy(ptr, "; expires=");
		ptr += 10;

		char timeBuffer[30];
		strftime(timeBuffer, sizeof(timeBuffer), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&expireDate));

		strcpy(ptr, timeBuffer);
		ptr += strlen(timeBuffer);
	}


	if ( domain != NULL ) {
		strcpy(ptr, "; domain=");
		ptr += 9;
		strcpy(ptr, domain);
		ptr += strlen(domain);
	}


	if ( path != NULL ) {
		strcpy(ptr, "; path=");
		ptr += 7;
		strcpy(ptr, path);
		ptr += strlen(path);
	}

	*ptr = '\0';

	apr_table_set(request->headers_out, "Set-Cookie", buffer);
	DEBUG_MSG("%s_setCookie([request_rec *], %s, %s, %ld, %s, %s): Set-Cookie: %s", name, value, expireDate, domain, path, buffer);
	DEBUG_MSG("%s_setCookie([request_rec *], %s, %s, %ld, %s, %s)... DONE", name, value, expireDate, domain, path);
}

char *cookie_getCookie(request_rec *request, char *name) {
	DEBUG_MSG("%s_getCookie([request_rec *], %s)...", name);
	char *buffer;
	char *cookieString = (char *) apr_table_get(request->headers_in, "Cookie");

	if ( cookieString == NULL ) {
		DEBUG_MSG("%s_getCookie([request_rec *], %s)... DONE", name);
		return NULL;
	}

	int nameLength = strlen(name);
	buffer = apr_pcalloc(request->pool, sizeof(char) * nameLength + 3);
	strcpy(buffer, name);
	buffer[nameLength + 1] = '=';
	buffer[nameLength + 2] = '\0';

	char *oldPos = cookieString;
	char *startPos = NULL;
	while ( startPos == NULL ) {
		startPos = strstr(oldPos, buffer);
		if ( startPos == NULL ) {
			break;
		}
		if ( !( startPos == cookieString || *( startPos - 1 ) == ' ' || *( startPos - 1 ) == ';' ) ) {
			oldPos = startPos;
			startPos = NULL;
			continue;
		}
	}

	if ( startPos == NULL ) {
		DEBUG_MSG("%s_getCookie([request_rec *], %s)... DONE", name);
		return NULL;
	}

	char *valPos = strchr(startPos, '=');
	int valueStartPos = valPos - cookieString;

	char *endPos = strchr(valPos, ';');

	char *value;
	if ( endPos != NULL ) {
		value = stringutil_subString(request->pool, cookieString, valueStartPos + 1, endPos - cookieString);
	} else {
		value = stringutil_subString(request->pool, cookieString, valueStartPos + 1, 0);
	}

	DEBUG_MSG("%s_getCookie([request_rec *], %s): value=%s", name, value);

	DEBUG_MSG("%s_getCookie([request_rec *], %s)... DONE", name);
	return value;
}