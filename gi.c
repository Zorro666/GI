#include <string.h>

#include "gi.h"
#include "OffencePlay.h"

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

GI_Return gi_HandleValue(Json_Value* const value, const int debug)
{
	if (value == NULL)
	{
		return GI_ERROR;
	}
	if (value->m_type == JSON_OBJECT)
	{
		if ((value->m_name != NULL) && (strcmp(value->m_name, "Play") == 0))
		{
			OffencePlay play;
			OffencePlay_Load(&play, value);
			if (debug == 1)
			{
				OffencePlay_Print(&play);
			}
			return GI_SUCCESS;
		}
	}
	if ((value->m_type == JSON_OBJECT) || (value->m_type == JSON_ARRAY))
	{
		Json_Value* it;
		for (it = value->m_first_child; it; it = it->m_next_sibling)
		{
			gi_HandleValue(it, debug);
		}
	}
	return GI_ERROR;
}

GI_Return gi_LoadFile(const char* const fileName, const int debug)
{
	Json_Value* value = gi_ParseFile(fileName, debug);
	if (value == NULL)
	{
		return GI_ERROR;
	}
	while (value != NULL)
	{
		if (gi_HandleValue(value, debug) == GI_SUCCESS)
		{
			return GI_SUCCESS;
		}
		break;
	}
	return GI_ERROR;
}
