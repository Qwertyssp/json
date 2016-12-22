#ifndef __LEPTJSON_H__
#define __LEPTJSON_H__

#include <stddef.h>

typedef enum {
	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBER,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_OBKECT
} lept_type;

typedef struct {
	union {
		struct {
			char *s;
			size_t len;
		} s;
		double n;
	}u;
	lept_type type;
} lept_value;

enum {
	LEPT_PARSE_OK,
	LEPT_PARSE_EXPECT_VALUE,
	LEPT_PARSE_INVALID_VALUE,
	LEPT_PARSE_ROOT_NOT_SINGULAR
};

int lept_parse(lept_value *v, const char *json);

lept_type lept_get_type(const lept_value *v);

double lept_get_number(const lept_value *v);

size_t lept_get_string_length(const lept_value *v);
const char *lept_get_string(const lept_value *v);
size_t lept_get_string_length(const lept_value *v);
void lept_set_string(lept_value *v, const char *s, size_t len);


#endif /*__LEPTJSON_H__ */
