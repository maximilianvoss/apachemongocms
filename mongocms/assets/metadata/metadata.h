#ifndef __ASSETS_METADATA_METADATAREQUEST_H__
#define __ASSETS_METADATA_METADATAREQUEST_H__

#include "../../includes.h"
#include "../../common/mongo.h"

int metadata_set(mongo_config_t *mongoConfig, request_rec *request);
int metadata_get(mongo_config_t *mongoConfig, request_rec *request);

#endif
