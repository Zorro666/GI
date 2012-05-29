#include <stdio.h>
#include <string.h>
#include "json.h"

typedef struct OffencePlay OffencePlay;

#define MAX_OFFENCENAME_SIZE (16)
#define MAX_DEFENCENAME_SIZE (16)

#define MAX_NUM_DEFENCE_FORMATIONS (32)

#define MAX_NUM_OFFENCE_POSITIONS (32)

#define MAX_POSITION_SIZE (8)

typedef struct PositionValue PositionValue;

struct PositionValue
{
	char m_position[MAX_POSITION_SIZE];
	float m_value;
};

void PositionValue_Init(PositionValue* const pThis)
{
	pThis->m_position[0] = '\0';
	pThis->m_value = 0.0f;
}

void PositionValueArray_Parse(PositionValue positionValue[], const int maxSize, Json_Value* const root)
{
	Json_Value* it;
	int i;

	i = 0;
	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_OBJECT)
		{
			Json_Value* value = it->m_first_child;
			if (value->m_type == JSON_FLOAT)
			{
				strncpy(positionValue[i].m_position, value->m_name, MAX_POSITION_SIZE);
				positionValue[i].m_value = value->m_value_data.float_value;
				i++;
				if (i >= maxSize)
				{
					return;
				}
			}
		}
	}
}

struct OffencePlay
{
	char m_name[MAX_OFFENCENAME_SIZE];
	char m_defense[MAX_NUM_DEFENCE_FORMATIONS][MAX_DEFENCENAME_SIZE];
	PositionValue m_base[MAX_NUM_OFFENCE_POSITIONS];
	PositionValue m_bc[MAX_NUM_OFFENCE_POSITIONS];
	PositionValue m_weighting[MAX_NUM_OFFENCE_POSITIONS];
};

Json_Value* FindOffencePlay(Json_Value* const value);

int OffencePlay_Load(OffencePlay* const pThis, const Json_Value* const playRoot);
void OffencePlay_Init(OffencePlay* const pThis);

Json_Value* FindOffencePlay(Json_Value* const value)
{
	return json_FindObjectByName(value, "Play");
}

void OffencePlay_Init(OffencePlay* const pThis)
{
	int i;

	pThis->m_name[0] = '\0';
	for (i = 0; i < MAX_NUM_DEFENCE_FORMATIONS; i++)
	{
		pThis->m_defense[i][0] = '\0';
	}
	for (i = 0; i < MAX_NUM_OFFENCE_POSITIONS; i++)
	{
		PositionValue_Init(&pThis->m_base[i]);
		PositionValue_Init(&pThis->m_bc[i]);
		PositionValue_Init(&pThis->m_weighting[i]);
	}
}

int OffencePlay_Load(OffencePlay* const pThis, const Json_Value* const playRoot)
{
	Json_Value* it;
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

	OffencePlay_Init(pThis);
	for (it = playRoot->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, MAX_OFFENCENAME_SIZE);
			}
		}
		if (it->m_type == JSON_ARRAY)
		{
			if (strcmp(it->m_name, "Def") == 0)
			{
				Json_Value* it2;
				int i;

				i = 0;
				for (it2 = it->m_first_child; it2 != NULL; it2 = it2->m_next_sibling)
				{
					strncpy(pThis->m_defense[i], it2->m_value_data.string_value, MAX_DEFENCENAME_SIZE);
					i++;
				}
			}
			else if (strcmp(it->m_name, "Base") == 0)
			{
				PositionValueArray_Parse(pThis->m_base, MAX_NUM_OFFENCE_POSITIONS, it);
			}
			else if (strcmp(it->m_name, "BC") == 0)
			{
			}
		}
	}

	{
		int i;
		printf("Offence Play:'%s'\n", pThis->m_name);
		printf("Defence: ");
		for (i = 0; i < MAX_NUM_DEFENCE_FORMATIONS; i++)
		{
			if (pThis->m_defense[i][0] != '\0')
			{
				if (i > 0)
				{
					printf(", ");
				}
				printf("%s", pThis->m_defense[i]);
			}
		}
		printf("\n");
	}
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
