#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
	do { \
		test_count++; \
		if (equality) \
			test_pass++;\
		else { \
			fprintf(stderr, "%s:%d expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
			main_ret = 1;\
		}\
	}while(0)

#define EXPECT_EQ_INT(expect, actual)	EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define EXPECT_EQ_STRING(expect, actual, alength) \
	EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")

#define lept_init(v) do { (v)->type = LEPT_NULL;} while(0)
static void test_parse_string(void)
{
	lept_value v;
	lept_init(&v);
	lept_set_string(&v, "", 0);
	EXPECT_EQ_STRING("", lept_get_string(&v), lept_get_string_length(&v));
	lept_set_string(&v, "Hello", 5);
	EXPECT_EQ_STRING("Hello", lept_get_string(&v), lept_get_string_length(&v));
	lept_free(&v);
}


static void test_parse_null(void)
{ 
	lept_value v;
	v.type = LEPT_TRUE;
	EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null"));
	EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}


static void test_parse_true(void)
{ 
	lept_value v;
	v.type = LEPT_TRUE;
	EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true"));
	EXPECT_EQ_INT(LEPT_TRUE, lept_get_type(&v));
}

static void test_parse_false(void)
{ 
	lept_value v;
	v.type = LEPT_TRUE;
	EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
	EXPECT_EQ_INT(LEPT_FALSE, lept_get_type(&v));
}

static void test_parse_number(void)
{
	lept_value v;
	v.type = LEPT_TRUE;
}

static void test_parse(void)
{
	test_parse_null();
	test_parse_true();
	test_parse_false();
	test_parse_string();
}


int main(void) 
{
	test_parse();
	printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return main_ret;
}
