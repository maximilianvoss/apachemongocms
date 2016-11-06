#include <magick/MagickCore.h>
#include "extractor.h"
#include "../../config.h"
#include "../../common/stringutil.h"


void extractor_addCreationDateValue(apr_table_t *map, Image *image, char *exifkey);
void extractor_addGPSValue(apr_table_t *map, Image *image, char *exifkeyLong, char *exifkeyLongRef,
                           char *exifkeyLati, char *exifkeyLatiRef);
double extractor_calcGPSPos(char *position, const char *reference);

// calculate GPS Pos from string
double extractor_calcGPSPos(char *position, const char *reference) {
	int orientation = 1;
	float gpsPos = 0;
	if ( position != NULL && reference != NULL ) {

		if ( *reference == 'S' || *reference == 'W' ) {
			orientation = -1;
		}

		stringutil_tokens_t tokens[3];
		stringutil_removeWhitespaces(position);
		stringutil_splitTokens(tokens, position, ',', 3);

		int i;
		char buffer[BUFFER_SIZE];
		for ( i = 0; i < 3; i++ ) {
			stringutil_getToken(&tokens[i], position, buffer);
			int divider = stringutil_indexOf(buffer, '/');
			if ( divider > 0 ) {
				buffer[divider] = '\0';

				double factor = 1.0;
				if ( i == 1 ) factor = 1.0 / 60.0;
				if ( i == 2 ) factor = 1.0 / 3600.0;

				double divident = atof(buffer);
				double divisor = atof(&buffer[divider + 1]);

				double value = ( divident / divisor ) * factor;
				gpsPos += value;
			}
		}
	}
	return gpsPos * orientation;
}

// adding GPS data to map
void extractor_addGPSValue(apr_table_t *map, Image *image, char *exifkeyLong, char *exifkeyLongRef,
                           char *exifkeyLati, char *exifkeyLatiRef) {
	const char *keyLong = GetImageProperty(image, exifkeyLong);
	const char *keyLongRef = GetImageProperty(image, exifkeyLongRef);
	const char *keyLati = GetImageProperty(image, exifkeyLati);
	const char *keyLatiRef = GetImageProperty(image, exifkeyLatiRef);

	double longPos = extractor_calcGPSPos((char *) keyLong, keyLongRef);
	double latiPos = extractor_calcGPSPos((char *) keyLati, keyLatiRef);

	char posBuffer[15];
	snprintf(posBuffer, 14, "%3.10f", longPos);
	apr_table_set(map, "metadata#location#x", posBuffer);

	snprintf(posBuffer, 14, "%3.10f", latiPos);
	apr_table_set(map, "metadata#location#y", posBuffer);
}

// add creationDate to map
void extractor_addCreationDateValue(apr_table_t *map, Image *image, char *exifkey) {
	const char *tmp = GetImageProperty(image, exifkey);

	if ( tmp != NULL ) {
		struct tm tm;
		strptime(tmp, "%Y:%m:%d %H:%M:%S", &tm);
		time_t t = mktime(&tm);

		char timeBuffer[32];
		snprintf(timeBuffer, 32, "%ld", (long) t);

		apr_table_set(map, "metadata#creationDate#$timestamp#t", timeBuffer);
		apr_table_set(map, "metadata#creationDate#$timestamp#i", "0");
	}
}

// extract all necessary metadata from exif
void extractor_extract(apr_table_t *map, char *filename) {
	Image *image;

	ImageInfo *imageInfo = AcquireImageInfo();
	ExceptionInfo *exception = AcquireExceptionInfo();

	strcpy(imageInfo->filename, filename);

	imageInfo->verbose = MagickFalse;
	image = PingImage(imageInfo, exception);

	extractor_addCreationDateValue(map, image, "exif:DateTimeOriginal");
	extractor_addGPSValue(map, image, "exif:GPSLongitude", "exif:GPSLongitudeRef", "exif:GPSLatitude",
	                      "exif:GPSLatitudeRef");

	DestroyImageInfo(imageInfo);
	DestroyExceptionInfo(exception);
	DestroyImage(image);
}

