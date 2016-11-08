#include "profile.h"
#include "user.h"
#include "../common/maputil.h"
#include "../mod_mongocms.h"
#include "../common/jsonmapfilter.h"
#include "../common/requesthelper.h"


int profile_get(request_rec *request) {
	apr_table_t *userMap = user_getUserMap(request);
	apr_table_t *whitelistedMap = maputil_removeUnlistedElements(request->pool, userMap, getModuleConfig()->user.propWhitelistOut);
	ap_rputs(jsonmapfilter_outboundMap(request->pool, whitelistedMap, getModuleConfig()->user.propMappingOut), request);
	return OK;
}

int profile_set(request_rec *request) {
	apr_table_t *requestMap = requesthelper_getPostMap(request, getModuleConfig()->user.propMappingIn, getModuleConfig()->user.propWhitelistIn);
	apr_table_t *userMap = user_getUserMap(request);

	apr_table_t *newUserMap = maputil_mergeMaps(request->pool, userMap, requestMap);
	
	mongo_update(&getModuleConfig()->user.database, request->pool, userMap, newUserMap);

	return profile_get(request);
}