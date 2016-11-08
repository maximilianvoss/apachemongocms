#include "md5.h"
#include "../config.h"
#include <openssl/md5.h>

char *md5_file2md5(apr_pool_t *pool, char *filename) {
	int i;
	MD5_CTX c;
	unsigned char digest[16];
	char *out = (char *) apr_pcalloc(pool, sizeof(char) * 33);
	char *buffer = apr_pcalloc(pool, sizeof(char) * FILE_BUFFER_SIZE);

	MD5_Init(&c);

	FILE *stream = fopen(filename, "r");
	if ( stream == NULL ) {
		return NULL;
	}
	while ( !feof(stream) ) {
		size_t size = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, stream);
		MD5_Update(&c, buffer, size);
	}
	fclose(stream);


	MD5_Final(digest, &c);
	for ( i = 0; i < 16; ++i ) {
		snprintf(&( out[i * 2] ), 16 * 2, "%02x", (unsigned int) digest[i]);
	}

	return out;
}

char *md5_str2md5(apr_pool_t *pool, char *string) {
	int i;
	MD5_CTX c;
	unsigned char digest[16];
	char *out = (char *) apr_pcalloc(pool, sizeof(char) * 33);

	MD5_Init(&c);
	MD5_Update(&c, string, strlen(string));
	MD5_Final(digest, &c);

	for ( i = 0; i < 16; ++i ) {
		snprintf(&( out[i * 2] ), 16 * 2, "%02x", (unsigned int) digest[i]);
	}

	return out;
}