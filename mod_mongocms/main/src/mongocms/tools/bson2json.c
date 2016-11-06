#include "tools.h"

int main(int argc, char **arv) {

#define QUERY "{ \"a\": \"/usr/bin/data\"}"

	bson_t query;
	bson_init_from_json(&query, QUERY, strlen(QUERY), NULL);


	size_t size = 1000;
	printf("%s\n", bson_as_json(&query, &size));

	bson_destroy(&query);

	return 0;
}