#include "rendition.h"
#include "../../common/fileutil.h"
#include "../../common/stringutil.h"
#include "../../mod_mongocms.h"
#include "../../common/entryhelper.h"
#include <wand/MagickWand.h>

#ifdef ASSETS_RENDITION_DEBUG

#include "../../common/logging.h"
#include "../../config.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "rendition", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "rendition")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif

// retrieve width of image based on transformation name
uint16_t rendition_getWidthForTransformation(apr_pool_t *pool, char *filePath) {
	DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s)...", filePath);
	
	char *filename = fileutil_getFilename(filePath);
	char *transform = stringutil_subString(pool, filename, stringutil_indexOf(filename, '.') + 1,
	                                       stringutil_lastIndexOf(filename, '.'));

	DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s): transformation string=%s", filePath, transform);
	
	if ( apr_is_empty_array(getModuleConfig()->asset.assetTransform) ) {
		DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s): transformation list is empty", filePath);
		DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s)... DONE", filePath);
		return 0;
	}

	int i;
	module_config_transform_t element;
	apr_array_header_t *tmpArray = getModuleConfig()->asset.assetTransform;
	for ( i = 0; i < tmpArray->nalloc; i++ ) {
		element = APR_ARRAY_IDX(tmpArray, i, module_config_transform_t);
		if ( element.name != NULL && !strcmp(element.name, transform) ) {
			DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s): found the fitting one; width=%d", filePath, element.width);
			DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s)... DONE", filePath);
			return element.width;
		}
	}
	DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s): no match was found", filePath);
	DEBUG_MSG("%s_getWidthForTransformation([apr_pool_t *], %s)... DONE", filePath);
	return 0;
}

// create a image and write it to file
uint8_t rendition_createRendition(char *in, char *out, uint16_t newWidth, uint16_t newHeight) {
	DEBUG_MSG("%s_createRendition(%s, %s, %d, %d)...", in, out, newWidth, newHeight);
	MagickBooleanType status;
	MagickWand *wand;

	size_t current_width;
	size_t current_height;
	float ratio;

	MagickWandGenesis();
	wand = NewMagickWand();
	status = MagickReadImage(wand, in);
	if ( status == MagickFalse ) {
		DEBUG_MSG("%s_createRendition(%s, %s, %d, %d): error on reading image", in, out, newWidth, newHeight);
		DEBUG_MSG("%s_createRendition(%s, %s, %d, %d)... DONE", in, out, newWidth, newHeight);
		return 1;
	}

	if ( !newWidth && !newHeight ) {
		DEBUG_MSG("%s_createRendition(%s, %s, %d, %d): error on reading width & height", in, out, newWidth, newHeight);
		DEBUG_MSG("%s_createRendition(%s, %s, %d, %d)... DONE", in, out, newWidth, newHeight);
		return 1;
	}

	current_height = MagickGetImageHeight(wand);
	current_width = MagickGetImageWidth(wand);
	ratio = (float) current_width / current_height;

	size_t sizedWidth = (size_t) newWidth;
	size_t sizedHeight = (size_t) newHeight;

	if ( !sizedWidth ) {
		sizedWidth = (size_t) newHeight * ratio;
	} else if ( !newHeight ) {
		sizedHeight = (size_t) newWidth / ratio;
	}

	MagickResizeImage(wand, sizedWidth, sizedHeight, LanczosFilter, 1.0);

	status = MagickWriteImages(wand, out, MagickTrue);
	if ( status == MagickFalse ) {
		return 1;
	}

	DestroyMagickWand(wand);
	MagickWandTerminus();
	DEBUG_MSG("%s_createRendition(%s, %s, %d, %d)... DONE", in, out, newWidth, newHeight);
	return 0;
}


// get image rendition
int rendition_get(mongo_config_t *config, request_rec *request, char *filename) {
	DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s)...", filename);
	// do nothing if rendtion is already cached
	if ( fileutil_exists(filename) ) {
		DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s): file exists", filename);
		DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s)... DONE", filename);
		return DECLINED;
	}
	
	uint16_t width = rendition_getWidthForTransformation(request->pool, filename);
	if ( !width ) {
		DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s): width = 0", filename);
		DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s)... DONE", filename);
		return DECLINED;
	}
	DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s): width = %d", filename, width);
	
	char *assetId = entryhelper_getEntryId(request);
	apr_table_t *assetMap = entryhelper_getEntryById(config, request->pool, assetId);
	if ( !apr_is_empty_table(assetMap) ) {
		char *path = stringutil_escapeString(request->pool, (char *) apr_table_get(assetMap, "file"));
		stringutil_replaceChar(path, '\\', '/');

		// calculate image
		rendition_createRendition(path, filename, width, 0);

		char fbuffer[BUFFER_SIZE];
		FILE *file;
		if ( fileutil_exists(filename) ) {
			file = fopen(filename, "r");
			while ( fread(fbuffer, 1, BUFFER_SIZE, file) ) {
				ap_rwrite(fbuffer, BUFFER_SIZE, request);
			}
			fclose(file);
			DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s)... DONE", filename);
			return OK;
		}
	}
	DEBUG_MSG("%s_get([mongo_config_t *], [request_rec *], %s)... DONE", filename);
	return DECLINED;
}
