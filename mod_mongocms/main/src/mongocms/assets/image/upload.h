#ifndef __ASSETS_UPLOAD_UPLOAD_H__
#define __ASSETS_UPLOAD_UPLOAD_H__

#include "../../includes.h"
#include "../../common/mongo.h"

int upload_image(mongo_config_t *config, request_rec *request);

#endif
