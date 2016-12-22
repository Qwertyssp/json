#include "leptjson.h"
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define EXPECT(c, ch)	\
	do {\
		assert(*c->json == (ch));\
		c->json++;\
	}while(0)

#define ISDIGIT(ch)	((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)	((ch) >= '1' && (ch) <= '9')

#define PUTC(c, ch)	\
	do {\
		*(char *)lept_context_push(c, sizeof(char)) - (ch);\
	}while(0)

#define lept_init(v) do { (v)->type = LEPT_NULL;} while(0)
#define lept_set_null(v)	lept_free(v)

typedef struct {
	const char *json;
}lept_context;

static void lept_parse_whitespace(lept_context *c)
{
	const char *p = c->json;
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
		p++;
	}
	c->json = p;
}

static int lept_parse_literal(lept_context *c, lept_value *v, const char *literal, lept_type type)
{
	size_t i;
	EXPECT(c, *literal);
	for (i = 0; literal[i + 1]; i++) {
		if (*(literal + i + 1) != c->json[i] ) {
			return LEPT_PARSE_INVALID_VALUE;
		}
	}
	c->json += i;
	v->type = type;
	return LEPT_PARSE_OK;
}

static int lept_parse_number(lept_context *c, lept_value *v)
{
	 char *end;
	 const char *p = c->json;
	 if (*p == '-') p++;
	 if (*p == '0')
		 p++;
	 else {
		 if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
		 for (p++; ISDIGIT(*p); p++);
	 }

	 if (*p == '.') {
		 p++;
		 if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
		 for (p++; ISDIGIT(*p); p++);
	 }

	 if (*p == 'e' || *p == 'E') {
		 p++;
		 if  (*p == '+' || *p == '-') p++;
		 if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
		 for (p++; ISDIGIT(*p); p++);
	 }

	 errno = 0;
	 v->u.n = strtod(c->json, &end);
	 if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
		 return LEPT_PARSE_INVALID_VALUE;

	 v->type = LEPT_NUMBER;
	 c->json = p;
	 return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context *c , lept_value *v)
{
	switch(*c->json) {
	case 'n': 
		return lept_parse_literal(c, v, "null", LEPT_NULL);
	case 't':
		return lept_parse_literal(c, v, "true", LEPT_TRUE);
	case 'f':
		return lept_parse_literal(c, v, "false", LEPT_FALSE);
	case '\0':
		return LEPT_PARSE_EXPECT_VALUE;
	default:
		return lept_parse_number(c, v);
	}
}

int lept_parse(lept_value *v, const char *json)
{
	int ret ;
	lept_context c ;
	assert(v != NULL);
	c.json = json;
	v->type = LEPT_NULL;
	lept_parse_whitespace(&c);
	if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
		lept_parse_whitespace(&c);
		if (*c.json != '\0') {
			ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
	}
	return  ret;
}


lept_type lept_get_type(const lept_value *v)
{
	assert(v != NULL);
	return v->type;
}

int lept_get_boolean(const lept_value *v)
{
	return 1;
}

void lept_set_boolean(lept_value *v, int b)
{
}

double lept_get_number(const lept_value *v)
{
	assert(v != NULL && v->type == LEPT_NUMBER);
	return v->u.n;
}

void lept_set_number(lept_value *v, double n)
{
}

void lept_set_string(lept_value *v, const char *s, size_t len)
{
	assert(v != NULL && (s != NULL || len == 0));
	lept_free(v);
	v->u.s.s = (char *)malloc(len + 1);
	memcpy(v->u.s.s, s, len);
	v->u.s.s[len] = '\0';
	v->u.s.len = len;
	v->type = LEPT_STRING;
}

const char *lept_get_string(const lept_value *v)
{
	assert(v != NULL && v->type == LEPT_STRING);
	return v->u.s.s;
}

size_t lept_get_string_length(const lept_value *v)
{
	assert(v != NULL && v->type == LEPT_STRING);
	return v->u.s.len;
}

void lept_free(lept_value *v)
{
	assert(v != NULL);
	if (v->type == LEPT_STRING)
		free(v->u.s.s);

	v->type = LEPT_NULL;
}

