#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libs/json2map/map2json.h"
#include "../libs/json2map/config.h"


char *maputil_setPrimitivePrefix(char *str) {

	if ( str == NULL ) {
		return NULL;
	}

	int length = strlen(str);

	char *retStr = calloc(sizeof(char), length + 2);
	*retStr = JSON2MAP_PRIMITIVE_PREFIXER;
	memcpy(retStr + 1, str, length + 1);
	return retStr;
}

int main(int argc, char **argv) {

	// db.login.update({}, {"$pull":{"login-token":{"expire-time":{"$lte": new Date()}}}})

	map2json_t *map = map2json_init();


	map2json_push(map, "$pull#login-token#expire-time#$lte", maputil_setPrimitivePrefix("new Date()"));
	printf("%s\n", map2json_create(map));

	map2json_destroy(map);

	return 0;
}