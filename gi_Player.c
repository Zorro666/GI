#include <stdio.h>
#include <string.h>

#include "gi_Player.h"

static void gi_Player_ComputeQSTs(gi_Player* const pThis)
{
	float level = (float)pThis->m_rawLevel;
	float experience = pThis->m_experience;
	float Q = (float)(pThis->m_rawQST[GI_QST_Q])/100.0f;
	float S = (float)(pThis->m_rawQST[GI_QST_S])/100.0f;
	float T = (float)(pThis->m_rawQST[GI_QST_T])/100.0f;

	level += experience;
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
	pThis->m_age = -1;
	pThis->m_level = 0.0f;
	pThis->m_QST[0] = 0.0f;
	pThis->m_QST[1] = 0.0f;
	pThis->m_QST[2] = 0.0f;
	gi_SpecialTeamsValues_Init(&pThis->m_specialTeamsValues);
}

GI_Bool gi_Player_IsValueValid(const Json_Value* const root)
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
	if (strcmp(root->m_name, "Player") != 0)
	{
		return GI_FALSE;
	}
	return GI_TRUE;
}

GI_Return gi_Player_Load(gi_Player* const pThis, const Json_Value* const root)
{
	Json_Value* it;
	char position[GI_POSITIONNAME_MAX_SIZE];
	position[0] = '\0';

	if (gi_Player_IsValueValid(root) == GI_FALSE)
	{
		return GI_ERROR;
	}

	gi_Player_Init(pThis);

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
			if (strcmp(it->m_name, "Level") == 0)
			{
				pThis->m_rawLevel = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "Q") == 0)
			{
				pThis->m_rawQST[GI_QST_Q] = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "S") == 0)
			{
				pThis->m_rawQST[GI_QST_S] = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "T") == 0)
			{
				pThis->m_rawQST[GI_QST_T] = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "Age") == 0)
			{
				pThis->m_age = it->m_value_data.int_value;
			}
		}
		else if (it->m_type == JSON_FLOAT)
		{
			if (strcmp(it->m_name, "Experience") == 0)
			{
				pThis->m_experience = it->m_value_data.float_value;
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

	return GI_SUCCESS;
}

void gi_Player_Print(const gi_Player* const pThis, FILE* const pFile)
{
	fprintf(pFile, "Player:'%s' '%s' Position:%s Level:%d QST:%d %d %d Age:%d Experience:%.2f L:%5.2f QST:%5.2f %5.2f %5.2f\n",
			pThis->m_name, 
			gi_GetUnitName(pThis->m_unit),
			gi_GetPositionName(pThis->m_position), 
			pThis->m_rawLevel, 
			pThis->m_rawQST[GI_QST_Q], pThis->m_rawQST[GI_QST_S], pThis->m_rawQST[GI_QST_T], 
			pThis->m_age, pThis->m_experience,
			pThis->m_level, 
			pThis->m_QST[GI_QST_Q], pThis->m_QST[GI_QST_S], pThis->m_QST[GI_QST_T]
			);
}

void gi_Player_ComputeSpecialTeams(gi_Player* const pThis)
{
	const GI_POSITION position = pThis->m_position;
	float blocker = 0.0f;
	float gunner = 0.0f;
	float protector = 0.0f;
	float runner = 0.0f;
	float level = (float)pThis->m_rawLevel;
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
	if (level > 10.0f)
	{
		level = 10.0f+((level-10.0f)*0.2f);
	}
	Q *= level;
	S *= level;
	T *= level;

	/* Personal Protector: RB, SF, TE, IB, OB: S x 1.5 */
	switch (position)
	{
		case GI_POSITION_RB:
		case GI_POSITION_SF:
		case GI_POSITION_TE:
		case GI_POSITION_IB:
		case GI_POSITION_OB:
			protector = 1.5f*S;
			break;
		default:
			protector = 0.0f;
			break;
	};

	/* Blocker: */
	/* DE, DT, C, OT, OG: L x 0.8 */
	/* IB, OB: L x 0.7 */
	/* TE, RB, SF: S + T x 0.33 */
	switch (position)
	{
		case GI_POSITION_DE:
		case GI_POSITION_DT:
		case GI_POSITION_OC:
		case GI_POSITION_OG:
			blocker = 0.8f*level;
			break;
		case GI_POSITION_IB:
		case GI_POSITION_OB:
			blocker = 0.7f*level;
			break;
		case GI_POSITION_TE:
		case GI_POSITION_RB:
		case GI_POSITION_SF:
			blocker = 0.33f*(S+T);
			break;
		default:
			blocker = 0.0f;
			break;
	};

	/* Runner: */
	/* R: L x 0.9 */
	/* CB, WR: L x 0.8 */
	/* TE, RB, SF: Q + T x 0.33 */
	switch (position)
	{
		case GI_POSITION_R:
			runner = 0.9f*level;
			break;
		case GI_POSITION_CB:
		case GI_POSITION_WR:
			runner = 0.8f*level;
			break;
		case GI_POSITION_TE:
		case GI_POSITION_RB:
		case GI_POSITION_SF:
			runner = 0.33f*(Q+T);
			break;
		default:
			runner = 0.0f;
			break;
	};

	/* Gunner: SF, WR, CB, R: Q x 1.5 */
	switch (position)
	{
		case GI_POSITION_SF:
		case GI_POSITION_WR:
		case GI_POSITION_CB:
		case GI_POSITION_R:
			gunner = 1.5f*Q;
			break;
		default:
			gunner = 0.0f;
			break;
	};

	gi_SpecialTeamsValues_Set(&pThis->m_specialTeamsValues, blocker, gunner, protector, runner);
}
