#include "requesthelper.h"
#include "../config.h"
#include "maputil.h"
#include "jsonmapfilter.h"

apr_table_t *requesthelper_readPost(request_rec *request);
apr_table_t *requesthelper_readGet(request_rec *request);

// save post file request to file
apr_status_t requesthelper_saveRequestToFile(request_rec *request, const char *filename) {
	apr_file_t *file = NULL;
	apr_status_t status, s_close;
	apr_off_t length = 0, count = 0;
	const char *hdr;

	// get content-length
	hdr = apr_table_get(request->headers_in, "Content-Length");
	length = ( hdr ) ? apr_atoi64(hdr) : LLONG_MAX;

	// create file.
	status = apr_file_open(&file, filename, APR_WRITE | APR_CREATE | APR_TRUNCATE, APR_FPROT_OS_DEFAULT, request->pool);
	if ( status != APR_SUCCESS ) goto FINALLY;

	// save to file.
	status = ap_setup_client_block(request, REQUEST_CHUNKED_DECHUNK);
	if ( status == OK ) {
		char *buffer = apr_pcalloc(request->pool, FILE_BUFFER_SIZE);
		long bytes;

		while ( ( ( bytes = ap_get_client_block(request, buffer, FILE_BUFFER_SIZE) ) > 0 ) && ( length > count ) ) {
			apr_size_t wr = 0;
			if ( count + bytes > length ) {
				bytes = length - count;
			}
			while ( wr < bytes ) {
				apr_size_t w = bytes - wr;
				status = apr_file_write(file, buffer, &w);
				if ( status != APR_SUCCESS ) goto FINALLY;
				wr += w;
			}
			count += bytes;
		}
	}

	FINALLY:
	if ( file ) {
		s_close = apr_file_close(file);
		if ( s_close != APR_SUCCESS ) {
			status = s_close;
		}
	}
	if ( status != APR_SUCCESS ) {
		apr_file_remove(filename, request->pool);
	}
	return status;
}


apr_table_t *requesthelper_getPostMap(request_rec *request, apr_table_t *mappingIn, apr_array_header_t *whitelistIn) {
	apr_table_t *requestMap = requesthelper_readPost(request);
	apr_table_t *transformedRequestMap = jsonmapfilter_inboundMap(request->pool, requestMap, mappingIn);
	apr_table_t *whitelistedMap = maputil_removeUnlistedElements(request->pool, transformedRequestMap, whitelistIn);
	return whitelistedMap;
}

// read standard values from a post request
apr_table_t *requesthelper_readPost(request_rec *request) {
	apr_array_header_t *pairs = NULL;
	apr_off_t len;
	apr_size_t size;
	int res;
	char *buffer;

	apr_table_t *map;

	res = ap_parse_form_data(request, NULL, &pairs, (size_t) -1, MAX_ALLOWED_REQUEST_SIZE);
	if ( res != OK || !pairs ) {
		return NULL; /* Return NULL if we failed or if there are is no POST data */
	}

	map = apr_table_make(request->pool, ( pairs->nelts + 1 ));
	while ( pairs && !apr_is_empty_array(pairs) ) {
		ap_form_pair_t *pair = (ap_form_pair_t *) apr_array_pop(pairs);
		apr_brigade_length(pair->value, 1, &len);
		size = (apr_size_t) len;
		buffer = apr_palloc(request->pool, size + 1);
		apr_brigade_flatten(pair->value, buffer, &size);
		buffer[len] = 0;

		apr_table_set(map, apr_pstrdup(request->pool, pair->name), buffer);
	}
	return map;
}

apr_table_t *requesthelper_getGetMap(request_rec *request, apr_table_t *mappingIn, apr_array_header_t *whiltelistIn) {
	apr_table_t *requestMap = requesthelper_readGet(request);
	apr_table_t *transformedRequestMap = jsonmapfilter_inboundMap(request->pool, requestMap, mappingIn);
	apr_table_t *whitelistedMap = maputil_removeUnlistedElements(request->pool, transformedRequestMap, whiltelistIn);
	return whitelistedMap;
}

// read standard vales from a get request
apr_table_t *requesthelper_readGet(request_rec *request) {
	char *key;
	char *val;
	const char *data = request->args;

	apr_table_t *map = apr_table_make(request->pool, CONFIG_TABLE_INIT_SIZE);
	if ( data != NULL ) {
		while ( *data && ( val = ap_getword(request->pool, &data, '&') ) ) {
			key = ap_getword(request->pool, (const char **) &val, '=');
			ap_unescape_url(key);
			ap_unescape_url(val);
			apr_table_set(map, key, val);
		}
	}
	return map;
}