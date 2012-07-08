#ifndef JSON_H
#define JSON_H

#include <stdio.h>
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

Json_Value* Json_ParseFile(const char* const filename, char* error_pos[], char* error_desc[], int* error_line, BlockAllocator* const allocator);
Json_Value* Json_Parse(char* const source, char* error_pos[], char* error_desc[], int* error_line, BlockAllocator* const allocator);
void Json_Print(const Json_Value* const value, FILE* const pFile, const int indent);
Json_Value* Json_FindObjectByName(Json_Value* const value, const char* const name);

/*
const Json_Value* Json_Value_GetParent(const Json_Value* pThis);
*/
const Json_Value* Json_Value_GetNextSibling(const Json_Value* pThis);
const Json_Value* Json_Value_GetFirstChild(const Json_Value* pThis);
/*
const Json_Value* Json_Value_GetLastChild(const Json_Value* pThis);
*/

const char* Json_Value_GetName(const Json_Value* const pThis);

Json_Type Json_Value_GetType(const Json_Value* const pThis);
const char* Json_Value_GetStringValue(const Json_Value* const pThis);
int Json_Value_GetIntValue(const Json_Value* const pThis);
float Json_Value_GetFloatValue(const Json_Value* const pThis);

#endif
