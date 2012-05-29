#include "gi.h"

BlockAllocator g_json_allocator;

void gi_Init(void)
{
	BlockAllocator_Init(&g_json_allocator, 128*1024);
}

Json_Value* gi_FindOffencePlay(Json_Value* const value)
{
	return Json_FindObjectByName(value, "Play");
}

Json_Value* gi_ParseFile(const char* const fileName, const int debug)
{
	Json_Value* root;
	char* error_pos;
	char* error_desc;
	int error_line;

	root = Json_ParseFile(fileName, &error_pos, &error_desc, &error_line, &g_json_allocator);
	if (root)
	{
		if (debug == 1)
		{
			Json_Print(root, 0);
		}
	}

	/* TODO: error handling output the error information */
	return root;
}
