#ifndef __ASSETS_RENDITION_RENDITION_H__
#define __ASSETS_RENDITION_RENDITION_H__

#include "../../includes.h"
#include "../../config.h"

//#define ASSETS_RENDITION_DEBUG

int rendition_get(mongo_config_t *config, request_rec *request, char *filename);

#endif