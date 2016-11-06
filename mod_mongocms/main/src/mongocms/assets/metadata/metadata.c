#include "metadata.h"
#include "../../common/entryhelper.h"
#include "../../mod_mongocms.h"

int metadata_set(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_updateEntry(request, mongoConfig, getModuleConfig()->asset.metadataParamInputWhitelist, getModuleConfig()->asset.metadataParamOutputWhitelist, getModuleConfig()->asset
			.metadataParamMapping, getModuleConfig()->asset.metadataParamMappingInverse);
}

int metadata_get(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_getEntry(request, mongoConfig, getModuleConfig()->asset.metadataParamOutputWhitelist, getModuleConfig()->asset.metadataParamMapping);
}