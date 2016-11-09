#ifndef __MOD_MONGOCMS_CONSTANTS_H__
#define __MOD_MONGOCMS_CONSTANTS_H__

// Size for parsing POST attributes
#define MAX_ALLOWED_REQUEST_SIZE 1<<22  // 4MB

// Buffer size to handle small things
#define SMALL_BUFFER_SIZE 1<<10         // 1kB

// Buffer size to handle files
#define FILE_BUFFER_SIZE 1<<18          // 256kB

// Init size of config arrays
#define CONFIG_ARRAY_INIT_SIZE 5

// Init size of config table
#define CONFIG_TABLE_INIT_SIZE 5

// configuration of mongodb 
#define MONGO_OID "_id#$oid"
#define MONGO_CREATEDBY "createdBy"
#define MONGO_CREATEDAT "createdAt"
#define MONGO_MODIFIEDBY "modifiedBy"
#define MONGO_MODIFIEDAT "modifiedAt"
#define MONGO_PRIVILEGES_WRITING "privileges#writing"

// configuration for the user's collection
#define USER_PROFILE_FILENAME "profile.json"
#define USER_LOGIN_FILENAME "login.json"
#define USER_LOGOUT_FILENAME "logout.json"
#define USER_REGISTRATION_FILENAME "register.json"
#define USER_UNREGISTRATION_FILENAME "unregister.json"

#define USER_MONGO_PROPERTY_USERNAME "username"
#define USER_MONGO_PROPERTY_PASSWORD "password"

#endif
