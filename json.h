#ifndef JSON_H
#define JSON_H

#include "block_allocator.h"

typedef struct json_value json_value;

typedef enum json_type
{
	JSON_NULL,
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_STRING,
	JSON_INT,
	JSON_FLOAT,
	JSON_BOOL
} json_type;

struct json_value
{
	json_value* parent;
	json_value* next_sibling;
	json_value* first_child;
	json_value* last_child;

	char* name;
	union value
	{
		char* string_value;
		int int_value;
		float float_value;
	} value_data;

	json_type type;
};

json_value* json_parse_file(char* const filename, char* error_pos[], char* error_desc[], int* error_line, block_allocator* allocator);
json_value* json_parse(char* const source, char* error_pos[], char* error_desc[], int* error_line, block_allocator* allocator);

#endif
