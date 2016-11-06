/*
Starting point of the Apache module 
It keeps all methods to dispatch requests, read configuration and open the Soy and MongoDB connection
*/

#include "mod_mongocms.h"
#include "module/directives.h"
#include "module/requesthandler.h"

// method to register hooks
static void mongocms_register_hooks(apr_pool_t *p);

// module configuration
static module_config_t module_config;

// register the module hooks
module AP_MODULE_DECLARE_DATA mongocms_module = {
		STANDARD20_MODULE_STUFF,
		NULL,
		NULL,
		NULL,
		NULL,
		mongocms_directives,
		mongocms_register_hooks
};

// do a clean up of the Mongo Connections while destroying the Apache module
static apr_status_t mongocms_cleanup(void *dummy) {
	mongo_destroy(&module_config.document.database);
	mongo_destroy(&module_config.asset.database);
	mongo_destroy(&module_config.user.database);
	mongoc_cleanup();
	return APR_SUCCESS;
}

// store server_rec into singleton for logging support
static void serverRecToSingelton(apr_pool_t *pool, server_rec *s) {
	module_config.serverRec = s;
}

// the actual hook registrations
static void mongocms_register_hooks(apr_pool_t *pool) {
	mongoc_init();
	module_config.modulePool = pool;
	
	// Register cleanup task
	apr_pool_cleanup_register(pool, NULL, mongocms_cleanup, apr_pool_cleanup_null);

	// init singleton for Server Rec
	ap_hook_child_init(serverRecToSingelton, NULL, NULL, APR_HOOK_FIRST);

	// Application handler
	ap_hook_handler(request_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

// expose the module configuration to other methods
module_config_t *getModuleConfig() {
	return &module_config;
}