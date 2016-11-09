#include "stringutil.h"
#include "../constants.h"

#ifdef COMMON_STRINGUTIL_DEBUG

#include "../common/logging.h"

#define DEBUG_MSG(fmt, ...) LOG_SERVER_DEBUG_FORMAT(fmt, "stringutil", __VA_ARGS__)
#define DEBUG_PUT(fmt) LOG_SERVER_DEBUG_FORMAT(fmt, "stringutil")
#else
#define DEBUG_MSG(fmt, ...)
#define DEBUG_PUT(fmt)
#endif


void stringutil_getToken(stringutil_tokens_t *token, char *str, char *buffer) {
	memcpy(buffer, str + token->start, token->length);
	char *ptr = buffer + token->length;
	*ptr = '\0';
}


int stringutil_splitTokens(stringutil_tokens_t *tokens, char *str, char chr, int maxCount) {
	char *lastPos;
	char *newPos;
	int count = 0;

	newPos = str;
	lastPos = str;

	while ( *newPos != '\0' ) {
		if ( *newPos == chr ) {
			tokens[count].start = ( lastPos - str );
			tokens[count].length = ( newPos - lastPos );
			lastPos = newPos + 1;
			count++;
			if ( count >= maxCount ) {
				return maxCount;
			}
		}
		newPos++;
	}
	if ( newPos != str ) {
		tokens[count].start = ( lastPos - str );
		tokens[count].length = ( newPos - lastPos );
		count++;
	}
	return count;
}

// remove all whitespaces from string
void stringutil_removeWhitespaces(char *str) {
	char *ptrCurrent;
	char *pos;

	if ( str == NULL ) {
		return;
	}

	pos = str;
	ptrCurrent = str;
	while ( *pos != '\0' ) {
		if ( *pos == ' ' ) {
			pos++;
		} else {
			*ptrCurrent = *pos;
			pos++;
			ptrCurrent++;
		}
	}
	*ptrCurrent = '\0';
}


//  find the first index of a char in a string
int stringutil_indexOf(char *str, char letter) {
	//TODO use strchr as base method
	int pos = 0;
	while ( *str != '\0' ) {
		if ( *str == letter ) {
			return pos;
		}
		str++;
		pos++;
	}
	return -1;
}

// find the last index of a char in a string
int stringutil_lastIndexOf(char *str, char letter) {
	//TODO use strchr as base method in a loop
	int pos = 0;
	int found = -1;
	while ( *str != '\0' ) {
		if ( *str == letter ) {
			found = pos;
		}
		str++;
		pos++;
	}
	return found;
}

// extract a substring from a string
char *stringutil_subString(apr_pool_t *pool, char *str, int start, int end) {
	DEBUG_MSG("%s_substring([apr_pool_t *], %s, %d, %d)...", str, start, end);
	int length;
	if ( end == 0 ) {
		length = strlen(str) - start;
	} else {
		length = end - start;
	}
	DEBUG_MSG("%s_substring([apr_pool_t *], %s, %d, %d)L length=%d", str, start, end, length);

	if ( length < 0 ) {
		DEBUG_MSG("%s_substring([apr_pool_t *], %s, %d, %d)... DONE", str, start, end);
		return NULL;
	}
	
	char *outStr = (char *) apr_pcalloc(pool, sizeof(char) * ( length + 1 ));
	char *pos = outStr;

	for ( int i = start; i < start + length; i++ ) {
		*pos = str[i];
		pos++;
	}
	*pos = '\0';

	DEBUG_MSG("%s_substring([apr_pool_t *], %s, %d, %d) = %s... DONE", str, start, end, outStr);
	return outStr;
}

// escape a string (\\ to \)
char *stringutil_escapeString(apr_pool_t *pool, char *str) {

	if ( str == NULL ) {
		return NULL;
	}
	size_t length = strlen(str);

	char *outStr = (char *) apr_pcalloc(pool, sizeof(char) * ( length + 1 ));
	char *tmp = outStr;

	char *ptr = str;
	char *lookAhead = str + 1;

	while ( *ptr != '\0' ) {
		if ( *ptr == '\\' && *lookAhead == '\\' ) {
			*tmp = *ptr;
			ptr++;
			lookAhead++;
			tmp++;
		} else if ( *ptr != '\\' ) {
			*tmp = *ptr;
			tmp++;
		}
		ptr++;
		lookAhead++;
	}
	return outStr;
}

// replace a specific char in a string
void stringutil_replaceChar(char *str, char old, char new) {
	if ( str == NULL ) {
		return;
	}
	while ( *str != '\0' ) {
		if ( *str == old ) {
			*str = new;
		}
		str++;
	}
}

uint8_t stringutil_startsWith(char *str, char *token) {
	if ( str == NULL || token == NULL ) {
		return 0;
	}

	while ( *token != '\0' ) {
		if ( *str == '\0' ) {
			return 0;
		}
		if ( *token != *str ) {
			return 0;
		}
		token++;
		str++;
	}
	return 1;
}


char *stringutil_longToString(apr_pool_t *pool, long value) {
	char *buffer = apr_pcalloc(pool, sizeof(char) * 21);
	sprintf(buffer, "%ld", value);
	return buffer;
}


char *stringutil_replaceVariables(apr_pool_t *pool, char *str, apr_table_t *map) {
	DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *])...", str);

	char variableName[255];
	char *ptr = str;
	char *ptr2 = variableName;
	const char *variableValue;
	char *buffer = apr_pcalloc(pool, SMALL_BUFFER_SIZE);
	char *ptr3 = buffer;

	while ( *ptr != '\0' ) {

		if ( *ptr == '@' && *( ptr + 1 ) == '{' ) {
			ptr += 2;
			memset(variableName, '\0', 255);
			ptr2 = variableName;

			while ( *ptr != '\0' && *ptr != '}' ) {
				*ptr2 = *ptr;
				ptr++;
				ptr2++;
			}
			DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *]): variableName=%s", str, variableName);

			if ( *ptr == '\0' ) {
				DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *]): Unexpected end of String", str);
				return NULL;
			}

			variableValue = apr_table_get(map, variableName);
			DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *]): variableValue=%s", str, variableValue);
			if ( variableValue == NULL ) {
				DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *]): Expected field is not set", str);
				return NULL;
			}
			size_t length = strlen(variableValue);
			memcpy(ptr3, variableValue, length);
			ptr3 += length - 1;
		} else {
			*ptr3 = *ptr;
		}
		ptr++;
		ptr3++;
	}

	DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *]): output=%s", str, buffer);
	DEBUG_MSG("%s_replaceVariables([apr_pool_t *], %s, [apr_table_t *])... DONE", str);
	return buffer;
}