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

// mongo attributes 
#define MONGO_PROPERTY_OID "_id#$oid"
#define MONGO_PROPERTY_USERNAME "username"
#define MONGO_PROPERTY_PASSWORD "password"
#define MONGO_PROPERTY_CREATEDBY "createdBy"
#define MONGO_PROPERTY_CREATEDAT "createdAt"
#define MONGO_PROPERTY_MODIFIEDBY "modifiedBy"
#define MONGO_PROPERTY_MODIFIEDAT "modifiedAt"
#define MONGO_PROPERTY_PRIVILEGES_WRITING "privileges#writing"
#define MONGO_PROPERTY_REFERENCES "references"

// filenames
#define FILENAME_USER_PROFILE "profile.json"
#define FILENAME_USER_LOGIN "login.json"
#define FILENAME_USER_LOGOUT "logout.json"
#define FILENAME_USER_REGISTRATION "register.json"
#define FILENAME_ALL_DELETE "delete.json"

#endif
