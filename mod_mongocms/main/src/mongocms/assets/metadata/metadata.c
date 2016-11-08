#include "metadata.h"
#include "../../common/entryhelper.h"
#include "../../mod_mongocms.h"

int metadata_set(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_updateEntry(request, mongoConfig, getModuleConfig()->asset.propWhitelistIn, getModuleConfig()->asset.propWhitelistOut, getModuleConfig()->asset
			.propMappingIn, getModuleConfig()->asset.propMappingOut);
}

int metadata_get(mongo_config_t *mongoConfig, request_rec *request) {
	return entryhelper_getEntry(request, mongoConfig, getModuleConfig()->asset.propWhitelistOut, getModuleConfig()->asset.propMappingOut);
}