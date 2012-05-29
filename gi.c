#include <stdio.h>
#include <string.h>
#include "json.h"

typedef struct OffencePlay OffencePlay;

#define MAX_OFFENCENAME_SIZE (16)
#define MAX_DEFENCENAME_SIZE (16)

#define MAX_NUM_DEFENCE_FORMATIONS (32)
#define MAX_NUM_OFFENCE_FORMATIONS (32)

#define MAX_POSITION_SIZE (8)

typedef struct PositionValue PositionValue;

struct PositionValue
{
	char m_position[MAX_POSITION_SIZE];
	float m_value;
};

struct OffencePlay
{
	char m_name[MAX_OFFENCENAME_SIZE];
	char m_defense[MAX_NUM_DEFENCE_FORMATIONS][MAX_DEFENCENAME_SIZE];
	PositionValue m_base[MAX_NUM_OFFENCE_FORMATIONS][MAX_POSITION_SIZE];
	PositionValue m_bc[MAX_NUM_OFFENCE_FORMATIONS][MAX_POSITION_SIZE];
	PositionValue m_weighting[MAX_NUM_OFFENCE_FORMATIONS][MAX_POSITION_SIZE];
};

Json_Value* FindOffencePlay(Json_Value* const value);
int OffencePlay_Load(OffencePlay* pThis, Json_Value* const playRoot);

Json_Value* FindOffencePlay(Json_Value* const value)
{
	return json_FindObjectByName(value, "Play");
}

int OffencePlay_Load(OffencePlay* pThis, Json_Value* const playRoot)
{
	if (playRoot->m_type != JSON_OBJECT)
	{
		fprintf(stderr, "NOT JSON_OBJECT\n");
		return 0;
	}
	if (playRoot->m_name == NULL)
	{
		fprintf(stderr, "name is NULL\n");
		return 0;
	}
	if (strcmp(playRoot->m_name, "Play") != 0)
	{
		return 0;
	}

	strncpy(pThis->m_name, playRoot->m_name, MAX_OFFENCENAME_SIZE);
	printf("Offence Play:'%s'\n", pThis->m_name);
	return 1;
}

int main(int argc, char* argv[])
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] '%s'\n", i, argv[i]);
	}

	{
		OffencePlay play;
		Json_Value* root;
		Json_Value* playRoot;
		char* error_pos;
		char* error_desc;
		int error_line;
		BlockAllocator json_allocator;
		BlockAllocator_Init(&json_allocator, 128*1024);

		root = json_ParseFile("data/offense/proset.json", &error_pos, &error_desc, &error_line, &json_allocator);
		if (root)
		{
			json_Print(root, 0);
		}
		playRoot = FindOffencePlay(root);
		OffencePlay_Load(&play, playRoot);
	}
	return -1;
}
