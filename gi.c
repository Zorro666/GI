#include <stdio.h>
#include <string.h>

#include "gi.h"
#include "gi_Team.h"
#include "gi_GlobalInfo.h"

static BlockAllocator s_json_allocator;
static gi_GlobalInfo s_globalInfo;

void gi_Init(void)
{
	BlockAllocator_Init(&s_json_allocator, 128*1024);
	gi_GlobalInfo_Init(&s_globalInfo);
}

Json_Value* gi_ParseFile(const char* const fileName, const int debug)
{
	Json_Value* root;
	char* error_pos;
	char* error_desc;
	int error_line;

	root = Json_ParseFile(fileName, &error_pos, &error_desc, &error_line, &s_json_allocator);
	if (root)
	{
		if (debug == 1)
		{
			Json_Print(root, stdout, 0);
		}
	}
	else
	{
		fprintf(stderr, "ERROR parsing file '%s'\n", fileName);
		fprintf(stderr, "ERROR line:%d pos:'%s' desc:'%s'\n", error_line, error_pos, error_desc);
		return NULL;
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
		if (value->m_name != NULL) 
		{
			if (gi_OffencePlay_IsValueValid(value) == GI_TRUE)
			{
				gi_OffencePlay play;
				if (gi_OffencePlay_Load(&play, value) == GI_SUCCESS)
				{
					if (debug == 1)
					{
						gi_OffencePlay_Print(&play, stdout);
					}
					
					return GI_SUCCESS;
				}
			}
			if (gi_Team_IsValueValid(value) == GI_TRUE)
			{
				gi_Team* const pTeam = &s_globalInfo.m_team;
				if (gi_Team_Load(pTeam, value) == GI_SUCCESS)
				{
					if (debug == 1)
					{
						gi_Team_Print(pTeam, stdout);
					}
					return GI_SUCCESS;
				}
			}
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
