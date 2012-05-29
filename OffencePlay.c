#include <stdio.h>
#include <string.h>

#include "OffencePlay.h"

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

void OffencePlay_Print(OffencePlay* const pThis)
{
	int i;
	printf("Offence Play:'%s'\n", pThis->m_name);
	printf("Defence: ");
	for (i = 0; i < MAX_NUM_DEFENCE_FORMATIONS; i++)
	{
		if (pThis->m_defense[i][0] != '\0')
		{
			printf("'%s' ", pThis->m_defense[i]);
		}
	}
	printf("\n");

	printf("Base: ");
	for (i = 0; i < MAX_NUM_OFFENCE_POSITIONS; i++)
	{
		PositionValue_Print(&pThis->m_base[i]);
	}
	printf("\n");

	printf("BC: ");
	for (i = 0; i < MAX_NUM_OFFENCE_POSITIONS; i++)
	{
		PositionValue_Print(&pThis->m_bc[i]);
	}
	printf("\n");

	printf("Weighting: ");
	for (i = 0; i < MAX_NUM_OFFENCE_POSITIONS; i++)
	{
		PositionValue_Print(&pThis->m_weighting[i]);
	}
	printf("\n");
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
				PositionValueArray_Parse(pThis->m_bc, MAX_NUM_OFFENCE_POSITIONS, it);
			}
			else if (strcmp(it->m_name, "Weighting") == 0)
			{
				PositionValueArray_Parse(pThis->m_weighting, MAX_NUM_OFFENCE_POSITIONS, it);
			}
		}
	}

	OffencePlay_Print(pThis);
	return 1;
}

