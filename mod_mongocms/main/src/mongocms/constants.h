#ifndef __MOD_MONGOCMS_CONSTANTS_H__
#define __MOD_MONGOCMS_CONSTANTS_H__

// Size for parsing POST attributes
#define MAX_ALLOWED_REQUEST_SIZE 1<<22 // 4MB

// standard length of string array
#define STRING_STD_LENGTH 1<<10

// Standard Buffer size
#define BUFFER_SIZE 1<<12

// Init size of config arrays
#define CONFIG_ARRAY_INIT_SIZE 5

// Init size of config table
#define CONFIG_TABLE_INIT_SIZE 5

#define MONGO_OID "_id#$oid"
#define MONGO_CREATEDBY "createdBy"
#define MONGO_CREATEDAT "createdAt"
#define MONGO_MODIFIEDBY "modifiedBy"
#define MONGO_MODIFIEDAT "modifiedAt"
#define MONGO_PRIVILEGES_WRITING "privileges#writing"

#endif
