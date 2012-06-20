#include <stdio.h>
#include <string.h>

#include "gi_OffencePlay.h"
#include "gi_PositionValue.h"
#include "gi_OffencePlayPrivate.h"
#include "gi_Logger.h"

void gi_OffencePlay_Init(gi_OffencePlay* const pThis)
{
	size_t i;

	pThis->m_name[0] = '\0';
	for (i = 0; i < GI_DEFENCE_FORMATIONS_MAX_SIZE; i++)
	{
		pThis->m_defence[i][0] = '\0';
	}
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Init(&pThis->m_base[i]);
		gi_PositionValue_Init(&pThis->m_bc[i]);
		gi_PositionValue_Init(&pThis->m_weighting[i]);
	}
	pThis->m_numBase = 0;
	pThis->m_numBC = 0;
	pThis->m_numWeighting = 0;
}

GI_BOOL gi_OffencePlay_IsValueValid(const Json_Value* const root)
{
	if (root == NULL)
	{
		GI_FATAL_ERROR("root is NULL\n");
		return GI_FALSE;
	}
	if (root->m_type != JSON_OBJECT)
	{
		GI_FATAL_ERROR("NOT JSON_OBJECT\n");
		return GI_FALSE;
	}
	if (root->m_name == NULL)
	{
		GI_FATAL_ERROR("name is NULL\n");
		return GI_FALSE;
	}
	if (strcmp(root->m_name, "OffencePlay") != 0)
	{
		return GI_FALSE;
	}
	return GI_TRUE;
}

GI_RETURN gi_OffencePlay_Load(gi_OffencePlay* const pThis, const Json_Value* const root)
{
	Json_Value* it;

	if (gi_OffencePlay_IsValueValid(root) == GI_FALSE)
	{
		return GI_RETURN_ERROR;
	}

	gi_OffencePlay_Init(pThis);
	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, GI_OFFENCENAME_MAX_SIZE);
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
					strncpy(pThis->m_defence[i], it2->m_value_data.string_value, GI_DEFENCENAME_MAX_SIZE);
					i++;
				}
			}
			else if (strcmp(it->m_name, "Base") == 0)
			{
				/* base should be FLOAT data - ERROR if INT data found */
				pThis->m_numBase = gi_PositionValueArray_Parse(pThis->m_base, GI_POSITION_NUM, it, GI_TYPE_FLOAT);
			}
			else if (strcmp(it->m_name, "BC") == 0)
			{
				/* bc should be FLOAT data - ERROR if INT data found */
				pThis->m_numBC = gi_PositionValueArray_Parse(pThis->m_bc, GI_POSITION_NUM, it, GI_TYPE_FLOAT);
			}
			else if (strcmp(it->m_name, "Weighting") == 0)
			{
				/* weighting should be INT data - ERROR if FLOAT data found */
				pThis->m_numWeighting = gi_PositionValueArray_Parse(pThis->m_weighting, GI_POSITION_NUM, it, GI_TYPE_INT);
			}
		}
	}

	return GI_RETURN_SUCCESS;
}

void gi_OffencePlay_Print(const gi_OffencePlay* const pThis, FILE* const pFile)
{
	size_t i;
	fprintf(pFile, "Offence Play:'%s'\n", pThis->m_name);
	fprintf(pFile, "Defence: ");
	for (i = 0; i < GI_DEFENCE_FORMATIONS_MAX_SIZE; i++)
	{
		if (pThis->m_defence[i][0] != '\0')
		{
			fprintf(pFile, "'%s' ", pThis->m_defence[i]);
		}
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Base: ");
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Print(&pThis->m_base[i], pFile);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "BC: ");
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Print(&pThis->m_bc[i], pFile);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Weighting: ");
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Print(&pThis->m_weighting[i], pFile);
	}
	fprintf(pFile, "\n");
}

float gi_OffencePlay_ComputeBase(const gi_OffencePlay* const pThis, const GI_POSITION playerPosition, const float* const playerQST)
{
	size_t i;
	float baseValue = 0.0f;

	for (i = 0; i < pThis->m_numBase; i++)
	{
		const gi_PositionValue* const pPositionValue = &pThis->m_base[i];
		if (gi_PositionValue_GetBasePosition(pPositionValue) == playerPosition)
		{
			if (gi_PositionValue_GetValueType(pPositionValue) == GI_TYPE_FLOAT)
			{
				const GI_QST playQST = gi_PositionValue_GetQST(pPositionValue);
				const float playerStat = playerQST[playQST] * gi_PositionValue_GetFloatValue(pPositionValue);
				baseValue += playerStat;
			}
		}
	}

	return baseValue;
}

const char* gi_OffencePlay_GetName(const gi_OffencePlay* const pThis)
{
	return pThis->m_name;
}

const char* gi_OffencePlay_GetDefence(const gi_OffencePlay* const pThis, const size_t defenceIndex)
{
	return pThis->m_defence[defenceIndex];
}
