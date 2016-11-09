#include "upload.h"
#include "../../common/fileutil.h"
#include "../../mod_mongocms.h"
#include "../../common/md5.h"
#include "../metadata/extractor.h"
#include "../../common/requesthelper.h"
#include "../../common/jsonmapfilter.h"
#include "../../user/user.h"
#include "../../common/stringutil.h"

char *upload_createTmpFile(apr_pool_t *pool, const char *filename);
char *upload_createDestinyFilename(apr_pool_t *pool, const char *path, char *md5hash);

// create custom ID for uploaded file
char *upload_createTmpFile(apr_pool_t *pool, const char *filename) {
	char tmpStr[14 + 10 + 1];

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(tmpStr, "%d%d%d%d%d%d%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
	        getpid());
	char *outStr = apr_pstrcat(pool, filename, tmpStr, NULL);

	return outStr;
}

// create the file path where the asset shall be stored in asset store
char *upload_createDestinyFilename(apr_pool_t *pool, const char *path, char *md5hash) {
	char tmpStr1[3];
	char tmpStr2[3];

	strncpy(tmpStr1, md5hash, 2);
	strncpy(tmpStr2, &md5hash[2], 2);
	tmpStr1[2] = '\0';
	tmpStr2[2] = '\0';

	char *outStr = apr_pstrcat(pool, path, "/", tmpStr1, "/", tmpStr2, "/", md5hash, NULL);
	return outStr;
}

// handle a post request
int upload_image(mongo_config_t *config, request_rec *request) {
	char *buffer;
	char *newFilePath;
	char *tmpFile = upload_createTmpFile(request->pool, getModuleConfig()->asset.assettmppath);

	int status = requesthelper_saveRequestToFile(request, tmpFile);
	if ( status == OK ) {
		char *md5hash = md5_file2md5(request->pool, tmpFile);
		newFilePath = upload_createDestinyFilename(request->pool, getModuleConfig()->asset.assetstorepath, md5hash);

		apr_table_t *map = apr_table_make(request->pool, 10);
		apr_table_set(map, MONGO_PROPERTY_FILENAME, newFilePath);

		if ( !fileutil_exists(newFilePath) ) {
			fileutil_mkpath(newFilePath, 0700);
			if ( rename(tmpFile, newFilePath) ) {
				unlink(tmpFile);
				return HTTP_INTERNAL_SERVER_ERROR;
			}
		}
		extractor_extract(map, newFilePath);

		apr_table_t *userMap = user_getUserMap(request);
		apr_table_set(map, MONGO_PROPERTY_CREATEDBY, user_getUserName(userMap));

		buffer = apr_pcalloc(request->pool, SMALL_BUFFER_SIZE);
		sprintf(buffer, "%s#$timestamp#t", MONGO_PROPERTY_CREATEDAT);
		apr_table_set(map, buffer, stringutil_longToString(request->pool, (long) time(NULL)));

		sprintf(buffer, "%s#$timestamp#i", MONGO_PROPERTY_CREATEDAT);
		apr_table_set(map, buffer, "0");

		sprintf(buffer, "%s[0]", MONGO_PROPERTY_PRIVILEGES_WRITING);
		apr_table_set(map, buffer, user_getUserId(userMap));

		char *oidStr = mongo_commit(config, request->pool, map);

		apr_table_t *responseMap = apr_table_make(request->pool, 2);
		apr_table_set(responseMap, MONGO_PROPERTY_OID, oidStr);
		apr_table_set(responseMap, "Status", "OK");

		ap_rputs(jsonmapfilter_outboundMap(request->pool, responseMap, getModuleConfig()->asset.propMappingOut), request);
	}

	return status;
}


