#include "logout.h"
#include "../common/cookie.h"
#include "session.h"


#ifdef USER_LOGOUT_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "logout", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "logout")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

int logout_destroyUserSession(request_rec *request) {
	DEBUG_PUT("%s_serverSession([request_rec *])...");
	char *tokenId = cookie_getCookie(request, "tokenId");

	if ( tokenId != NULL ) {
		DEBUG_MSG("%s_serverSession([request_rec *]): tokenId=%s", tokenId);
		session_terminateSession(request->pool, tokenId);
	}

	cookie_setCookie(request, "tokenId", "", time(NULL), "vmware", "/");
	ap_rputs("{ \"tokenId\": null }", request);

	DEBUG_PUT("%s_serverSession([request_rec *])... DONE");
	return OK;
}