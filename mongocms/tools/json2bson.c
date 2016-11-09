#include "tools.h"

int main(int argc, char **arv) {
	char json[4096];

	sprintf(json, "{ \"test\" : { \"$date\" : new Date() } }");

	bson_t b;
	bson_error_t error;
	bson_init_from_json(&b, json, strlen(json) + 1, NULL);

	mongoc_client_t *client = mongoc_client_new("mongodb://vmware2:27017/");
	mongoc_collection_t *collection = mongoc_client_get_collection(client, "test", "test");

	mongoc_collection_insert(collection, MONGOC_INSERT_NONE, &b, NULL, &error);
	size_t size = 1000;
	printf("%s\n", bson_as_json(&b, &size));
	bson_destroy(&b);


	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);

	return 0;
}