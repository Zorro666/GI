#ifndef JSON_H
#define JSON_H

#include "BlockAllocator.h"

typedef struct Json_Value Json_Value;

typedef enum Json_Type
{
	JSON_NULL,
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_STRING,
	JSON_INT,
	JSON_FLOAT,
	JSON_BOOL
} Json_Type;

struct Json_Value
{
	Json_Value* m_parent;
	Json_Value* m_next_sibling;
	Json_Value* m_first_child;
	Json_Value* m_last_child;

	char* m_name;
	union Value
	{
		char* string_value;
		int int_value;
		float float_value;
	} m_value_data;

	Json_Type m_type;
};

Json_Value* Json_ParseFile(const char* const filename, char* error_pos[], char* error_desc[], int* error_line, BlockAllocator* const allocator);
Json_Value* Json_Parse(char* const source, char* error_pos[], char* error_desc[], int* error_line, BlockAllocator* const allocator);
void Json_Print(const Json_Value* const value, const int indent);
Json_Value* Json_FindObjectByName(Json_Value* const value, const char* const name);

#endif
