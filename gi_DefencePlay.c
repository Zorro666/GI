#include <stdio.h>
#include <string.h>

#include "gi_DefencePlay.h"
#include "gi_Player.h"
#include "gi_Logger.h"

void gi_DefencePlay_Init(gi_DefencePlay* const pThis)
{
	size_t i;

	pThis->m_name[0] = '\0';
	pThis->m_zone[0] = '\0';

	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Init(&pThis->m_base[i]);
		gi_PositionValue_Init(&pThis->m_tackler[i]);
		gi_PositionValue_Init(&pThis->m_weighting[i]);
	}
	pThis->m_numBase = 0;
	pThis->m_numTackler = 0;
	pThis->m_numWeighting = 0;
}

GI_Bool gi_DefencePlay_IsValueValid(const Json_Value* const root)
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
	if (strcmp(root->m_name, "DefencePlay") != 0)
	{
		return GI_FALSE;
	}
	return GI_TRUE;
}

GI_Return gi_DefencePlay_Load(gi_DefencePlay* const pThis, const Json_Value* const root)
{
	Json_Value* it;

	if (gi_DefencePlay_IsValueValid(root) == GI_FALSE)
	{
		return GI_RETURN_ERROR;
	}

	gi_DefencePlay_Init(pThis);
	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, GI_DEFENCENAME_MAX_SIZE);
			}
			if (strcmp(it->m_name, "Zone") == 0)
			{
				strncpy(pThis->m_zone, it->m_value_data.string_value, GI_ZONENAME_MAX_SIZE);
			}
		}
		if (it->m_type == JSON_ARRAY)
		{
			if (strcmp(it->m_name, "Base") == 0)
			{
				/* TODO: base should be FLOAT data - ERROR if INT data found */
				pThis->m_numBase = gi_PositionValueArray_Parse(pThis->m_base, GI_POSITION_NUM, it);
			}
			else if (strcmp(it->m_name, "Tackler") == 0)
			{
				/* TODO: tackler should be INT data - ERROR if FLOAT data found */
				pThis->m_numTackler = gi_PositionValueArray_Parse(pThis->m_tackler, GI_POSITION_NUM, it);
			}
			else if (strcmp(it->m_name, "Weighting") == 0)
			{
				/* TODO: weighting should be INT data - ERROR if FLOAT data found */
				pThis->m_numWeighting = gi_PositionValueArray_Parse(pThis->m_weighting, GI_POSITION_NUM, it);
			}
		}
	}

	return GI_RETURN_SUCCESS;
}

void gi_DefencePlay_Print(const gi_DefencePlay* const pThis, FILE* const pFile)
{
	size_t i;
	fprintf(pFile, "Defence Play:'%s'\n", pThis->m_name);
	fprintf(pFile, "Defence: ");
	if (pThis->m_zone[0] != '\0')
	{
		fprintf(pFile, "'%s' ", pThis->m_zone);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Base: ");
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Print(&pThis->m_base[i], pFile);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Tackler: ");
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Print(&pThis->m_tackler[i], pFile);
	}
	fprintf(pFile, "\n");

	fprintf(pFile, "Weighting: ");
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		gi_PositionValue_Print(&pThis->m_weighting[i], pFile);
	}
	fprintf(pFile, "\n");
}

float gi_DefencePlay_ComputeBase(const gi_DefencePlay* const pThis, const gi_Player* const pPlayer)
{
	size_t i;
	const GI_POSITION playerPosition = pPlayer->m_position;
	float baseValue = 0.0f;

	for (i = 0; i < pThis->m_numBase; i++)
	{
		const gi_PositionValue* const pPositionValue = &pThis->m_base[i];
		if (pPositionValue->m_basePosition == playerPosition)
		{
			if (pPositionValue->m_valueType == GI_TYPE_FLOAT)
			{
				const GI_QST playQST = pPositionValue->m_qst;
				const float playerStat = pPlayer->m_QST[playQST] * pPositionValue->m_value.f;
				baseValue += playerStat;
			}
		}
	}

	return baseValue;
}
