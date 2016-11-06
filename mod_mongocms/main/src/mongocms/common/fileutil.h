#ifndef __COMMON_FILEUTIL_H__
#define __COMMON_FILEUTIL_H__

#include "../includes.h"

int8_t fileutil_exists(char *filename);
char *fileutil_getFilename(char *filePath);
int fileutil_mkpath(char *file_path, mode_t mode);
char *fileutil_getSuffix(char *filename);

#endif
