#include <stdio.h>
#include <string.h>

#include "gi_OffencePlay.h"

void gi_OffencePlay_Init(gi_OffencePlay* const pThis)
{
	size_t i;

	pThis->m_name[0] = '\0';
	for (i = 0; i < GI_MAX_NUM_DEFENCE_FORMATIONS; i++)
	{
		pThis->m_defense[i][0] = '\0';
	}
	for (i = 0; i < GI_NUM_POSITIONS; i++)
	{
		gi_PositionValue_Init(&pThis->m_base[i]);
		gi_PositionValue_Init(&pThis->m_bc[i]);
		gi_PositionValue_Init(&pThis->m_weighting[i]);
	}
}

GI_Bool gi_OffencePlay_IsValueValid(const Json_Value* const root)
{
	if (root == NULL)
	{
		fprintf(stderr, "root is NULL\n");
		return GI_FALSE;
	}
	if (root->m_type != JSON_OBJECT)
	{
		fprintf(stderr, "NOT JSON_OBJECT\n");
		return GI_FALSE;
	}
	if (root->m_name == NULL)
	{
		fprintf(stderr, "name is NULL\n");
		return GI_FALSE;
	}
	if (strcmp(root->m_name, "OffencePlay") != 0)
	{
		return GI_FALSE;
	}
	return GI_TRUE;
}

GI_Return gi_OffencePlay_Load(gi_OffencePlay* const pThis, const Json_Value* const root)
{
	Json_Value* it;

	if (gi_OffencePlay_IsValueValid(root) == GI_FALSE)
	{
		return GI_ERROR;
	}

	gi_OffencePlay_Init(pThis);
	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, GI_MAX_OFFENCENAME_SIZE);
			}
		}
		if (it->m_type == JSON_ARRAY)
		{
			if (strcmp(it->m_name, "Def") == 0)
			{
				Json_Value* it2;
				size_t i;

				i = 0;
				for (it2 = it->m_first_child; it2 != NULL; it2 = it2->m_next_sibling)
				{
					strncpy(pThis->m_defense[i], it2->m_value_data.string_value, GI_MAX_DEFENCENAME_SIZE);
					i++;
				}
			}
			else if (strcmp(it->m_name, "Base") == 0)
			{
				gi_PositionValueArray_Parse(pThis->m_base, GI_NUM_POSITIONS, it);
			}
			else if (strcmp(it->m_name, "BC") == 0)
			{
				gi_PositionValueArray_Parse(pThis->m_bc, GI_NUM_POSITIONS, it);
			}
			else if (strcmp(it->m_name, "Weighting") == 0)
			{
				gi_PositionValueArray_Parse(pThis->m_weighting, GI_NUM_POSITIONS, it);
			}
		}
	}

	return GI_SUCCESS;
}

void gi_OffencePlay_Print(const gi_OffencePlay* const pThis, FILE* const pFile)
{
	size_t i;
	fprintf(pFile, "Offence Play:'%s'\n", pThis->m_name);
	fprintf(pFile, "Defence: ");
	for (i = 0; i < GI_MAX_NUM_DEFENCE_FORMATIONS; i++)
	{
		if (pThis->m_defense[i][0] != '\0')
		{
			fprintf(pFile, "'%s' ", pThis->m_defense[i]);
		}
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Base: ");
	for (i = 0; i < GI_NUM_POSITIONS; i++)
	{
		gi_PositionValue_Print(&pThis->m_base[i], pFile);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "BC: ");
	for (i = 0; i < GI_NUM_POSITIONS; i++)
	{
		gi_PositionValue_Print(&pThis->m_bc[i], pFile);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Weighting: ");
	for (i = 0; i < GI_NUM_POSITIONS; i++)
	{
		gi_PositionValue_Print(&pThis->m_weighting[i], pFile);
	}
	fprintf(pFile, "\n");
}


