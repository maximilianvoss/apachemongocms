#include "fileutil.h"

// get suffix of a file
char *fileutil_getSuffix(char *filename) {
	char *ptr = filename;
	char *suffix = filename;

	while ( *ptr != '\0' ) {
		if ( *ptr == '.' ) {
			suffix = ++ptr;
		} else {
			ptr++;
		}
	}
	return suffix;
}

// check if file exists
int8_t fileutil_exists(char *filename) {
	struct stat buffer;
	return ( stat(filename, &buffer) == 0 );
}

// get the name of a file when path is provided
char *fileutil_getFilename(char *filePath) {
	char *ptr = filePath;
	char *filename = filePath;

	while ( *ptr != '\0' ) {
		if ( *ptr == '/' ) {
			filename = ++ptr;
		} else {
			ptr++;
		}
	}
	return filename;
}

int fileutil_mkpath(char *file_path, mode_t mode) {
	assert(file_path && *file_path);
	char *p;
	for ( p = strchr(file_path + 1, '/'); p; p = strchr(p + 1, '/') ) {
		*p = '\0';
		if ( mkdir(file_path, mode) == -1 ) {
			if ( errno != EEXIST ) {
				*p = '/';
				return -1;
			}
		}
		*p = '/';
	}
	return 0;
}