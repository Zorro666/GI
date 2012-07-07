#include <stdio.h>
#include <string.h>

#include "gi_Player.h"
#include "gi_PlayerPrivate.h"
#include "gi_Logger.h"

static float ConvertInjuryToFraction(const GI_INJURY injury)
{
	static int s_injuryPercentage[GI_INJURY_NUM] = {
			00, 	/* GI_INJURY_A */
			10, 	/* GI_INJURY_B */
			20, 	/* GI_INJURY_C */
			30, 	/* GI_INJURY_D */
			50, 	/* GI_INJURY_E */
			50, 	/* GI_INJURY_F */
			75, 	/* GI_INJURY_G */
			100,	/* GI_INJURY_H */
			100, 	/* GI_INJURY_UNKNOWN */
	};
	float fraction = 1.0f - (float)(s_injuryPercentage[injury])/100.0f;
	if (fraction > 1.0f)
	{
		fraction = 1.0f;
	}
	if (fraction < 0.0f)
	{
		fraction = 0.0f;
	}
	return fraction;
}

static void gi_Player_ComputeQSTs(gi_Player* const pThis)
{
	float level = (float)pThis->m_rawLevel;
	const float experience = pThis->m_experience;
	const float injuryFraction = ConvertInjuryToFraction(pThis->m_injury);
	float Q = (float)(pThis->m_rawQST[GI_QST_Q])/100.0f;
	float S = (float)(pThis->m_rawQST[GI_QST_S])/100.0f;
	float T = (float)(pThis->m_rawQST[GI_QST_T])/100.0f;

	level += experience;
	level *= injuryFraction;
	if (level < 1.0f)
	{
		level = 1;
	}
	Q *= level;
	S *= level;
	T *= level;

	pThis->m_level = level;
	pThis->m_QST[GI_QST_Q] = Q;
	pThis->m_QST[GI_QST_S] = S;
	pThis->m_QST[GI_QST_T] = T;
}

void gi_Player_Init(gi_Player* const pThis)
{
	pThis->m_name[0] = '\0';
	pThis->m_position = GI_POSITION_UNKNOWN;
	pThis->m_rawLevel = 0;
	pThis->m_rawQST[0] = 0;
	pThis->m_rawQST[1] = 0;
	pThis->m_rawQST[2] = 0;
	pThis->m_experience = 0.0f;
	pThis->m_unit = GI_UNIT_UNKNOWN;
	pThis->m_age = 0;
	pThis->m_level = 0.0f;
	pThis->m_QST[0] = 0.0f;
	pThis->m_QST[1] = 0.0f;
	pThis->m_QST[2] = 0.0f;
	pThis->m_injury = GI_INJURY_UNKNOWN;
}

GI_BOOL gi_Player_IsValueValid(const Json_Value* const root)
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
	if (strcmp(root->m_name, "Player") != 0)
	{
		return GI_FALSE;
	}
	return GI_TRUE;
}

GI_RETURN gi_Player_Load(gi_Player* const pThis, const Json_Value* const root)
{
	Json_Value* it;
	char position[GI_POSITIONNAME_MAX_SIZE];
	position[0] = '\0';

	if (gi_Player_IsValueValid(root) == GI_FALSE)
	{
		return GI_RETURN_ERROR;
	}

	gi_Player_Init(pThis);
	pThis->m_injury = GI_INJURY_A;

	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, GI_PLAYERNAME_MAX_SIZE);
			}
			else if (strcmp(it->m_name, "Position") == 0)
			{
				strncpy(position, it->m_value_data.string_value, GI_POSITIONNAME_MAX_SIZE);
			}
		}
		else if (it->m_type == JSON_INT)
		{
			const size_t value = (size_t)(it->m_value_data.int_value);
			if (strcmp(it->m_name, "Level") == 0)
			{
				pThis->m_rawLevel = value;
			}
			else if (strcmp(it->m_name, "Q") == 0)
			{
				pThis->m_rawQST[GI_QST_Q] = value;
			}
			else if (strcmp(it->m_name, "S") == 0)
			{
				pThis->m_rawQST[GI_QST_S] = value;
			}
			else if (strcmp(it->m_name, "T") == 0)
			{
				pThis->m_rawQST[GI_QST_T] = value;
			}
			else if (strcmp(it->m_name, "Age") == 0)
			{
				pThis->m_age = value;
			}
		}
		else if (it->m_type == JSON_FLOAT)
		{
			const float value = it->m_value_data.float_value;
			if (strcmp(it->m_name, "Experience") == 0)
			{
				pThis->m_experience = value;
			}
		}
	}

	pThis->m_unit = GI_UNIT_UNKNOWN;
	pThis->m_position = gi_GetPositionFromName(position);
	switch (pThis->m_position)
	{
		case GI_POSITION_QB:
		case GI_POSITION_RB:
		case GI_POSITION_WR:
		case GI_POSITION_TE:
		case GI_POSITION_OC:
		case GI_POSITION_OG:
		case GI_POSITION_OT:
			pThis->m_unit = GI_UNIT_OFFENCE;
			break;
		case GI_POSITION_DE:
		case GI_POSITION_DT:
		case GI_POSITION_IB:
		case GI_POSITION_OB:
		case GI_POSITION_CB:
		case GI_POSITION_SF:
			pThis->m_unit = GI_UNIT_DEFENCE;
			break;
		case GI_POSITION_R:
		case GI_POSITION_K:
		case GI_POSITION_P:
			pThis->m_unit = GI_UNIT_SPECIALTEAMS;
			break;
		default:
			pThis->m_unit = GI_UNIT_UNKNOWN;
			break;
	}

	gi_Player_ComputeQSTs(pThis);

	return GI_RETURN_SUCCESS;
}

void gi_Player_Print(const gi_Player* const pThis, FILE* const pFile)
{
	fprintf(pFile, "Player:'%s' '%s' Position:%s Level:%d QST:%d %d %d Age:%d Experience:%.2f L:%5.2f QST:%5.2f %5.2f %5.2f Injury:%s\n",
			pThis->m_name, 
			gi_GetUnitName(pThis->m_unit),
			gi_GetPositionName(pThis->m_position), 
			pThis->m_rawLevel, 
			pThis->m_rawQST[GI_QST_Q], pThis->m_rawQST[GI_QST_S], pThis->m_rawQST[GI_QST_T], 
			pThis->m_age, pThis->m_experience,
			pThis->m_level, 
			pThis->m_QST[GI_QST_Q], pThis->m_QST[GI_QST_S], pThis->m_QST[GI_QST_T],
			gi_GetInjuryName(pThis->m_injury)
			);
}

const char* gi_Player_GetName(const gi_Player* const pThis)
{
	return pThis->m_name;
}

GI_POSITION gi_Player_GetPosition(const gi_Player* const pThis)
{
	return pThis->m_position;
}

size_t gi_Player_GetRawLevel(const gi_Player* const pThis)
{
	return pThis->m_rawLevel;
}

const size_t* gi_Player_GetRawQST(const gi_Player* const pThis)
{
	return pThis->m_rawQST;
}

float gi_Player_GetExperience(const gi_Player* const pThis)
{
	return pThis->m_experience;
}

GI_UNIT gi_Player_GetUnit(const gi_Player* const pThis)
{
	return pThis->m_unit;
}

size_t gi_Player_GetAge(const gi_Player* const pThis)
{
	return pThis->m_age;
}

float gi_Player_GetLevel(const gi_Player* const pThis)
{
	return pThis->m_level;
}

const float* gi_Player_GetQST(const gi_Player* const pThis)
{
	return pThis->m_QST;
}

GI_INJURY gi_Player_GetInjury(const gi_Player* const pThis)
{
	return pThis->m_injury;
}

const char* gi_Player_GetPositionName(const gi_Player* const pThis)
{
	return gi_GetPositionName(pThis->m_position);
}

void gi_Player_SetInjury(gi_Player* const pThis, const GI_INJURY injury)
{
	pThis->m_injury = injury;
}

float gi_Player_GetSpecialTeamsLevelQST(const gi_Player* const pThis, float* const pQ, float* const pS, float* const pT)
{
	const GI_POSITION position = pThis->m_position;
	const float injuryFraction = ConvertInjuryToFraction(pThis->m_injury);
	float level = (float)(pThis->m_rawLevel);
	float experience = pThis->m_experience;
	float Q = (float)(pThis->m_rawQST[GI_QST_Q])/100.0f;
	float S = (float)(pThis->m_rawQST[GI_QST_S])/100.0f;
	float T = (float)(pThis->m_rawQST[GI_QST_T])/100.0f;

	/* Ignore experience for R, K, P */
	switch (position)
	{
		case GI_POSITION_R:
		case GI_POSITION_K:
		case GI_POSITION_P:
			experience = 0.0f;
			break;
		default:
			break;
	};
	level += experience;
	level *= injuryFraction;
	if (level > 10.0f)
	{
		level = 10.0f+((level-10.0f)*0.2f);
	}
	if (level < 1.0f)
	{
		level = 1;
	}

	Q *= level;
	S *= level;
	T *= level;

	*pQ = Q;
	*pS = S;
	*pT = T;

	return level;
}

