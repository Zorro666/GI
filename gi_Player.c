#include <stdio.h>
#include <string.h>

#include "gi_Player.h"

static char* s_positionNames[GI_POSITION_UNKNOWN+1] = {
	/*GI_QB, GI_RB, GI_WR, GI_TE, GI_OC, GI_OG, GI_OT, GI_DE, GI_DT, GI_IB, GI_OB, GI_CB, GI_SF, GI_R, GI_K, GI_P, GI_POSITION_UNKNOWN */
	"QB", "RB", "WR", "TE", "OC", "OG", "OT", "DE", "DT", "IB", "OB", "CB", "SF", "R", "K", "P", "UNKNOWN" };

void gi_Player_Init(gi_Player* const pThis)
{
	pThis->m_name[0] = '\0';
	pThis->m_position = GI_POSITION_UNKNOWN;
	pThis->m_level = 0;
	pThis->m_qst[0] = 0;
	pThis->m_qst[1] = 0;
	pThis->m_qst[2] = 0;
	pThis->m_experience = 0.0f;
	pThis->m_unit = GI_SQUAD_UNKNOWN;
	pThis->m_age = -1;
	pThis->m_specialTeamsValues.m_blocker = 0.0f;
	pThis->m_specialTeamsValues.m_gunner = 0.0f;
	pThis->m_specialTeamsValues.m_protector = 0.0f;
	pThis->m_specialTeamsValues.m_runner = 0.0f;
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
	int i;
	char position[MAX_POSITION_SIZE];
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
				strncpy(pThis->m_name, it->m_value_data.string_value, MAX_PLAYERNAME_SIZE);
			}
			else if (strcmp(it->m_name, "Position") == 0)
			{
				strncpy(position, it->m_value_data.string_value, MAX_POSITION_SIZE);
			}
		}
		else if (it->m_type == JSON_INT)
		{
			if (strcmp(it->m_name, "Level") == 0)
			{
				pThis->m_level = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "Q") == 0)
			{
				pThis->m_qst[GI_Q] = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "S") == 0)
			{
				pThis->m_qst[GI_S] = it->m_value_data.int_value;
			}
			else if (strcmp(it->m_name, "T") == 0)
			{
				pThis->m_qst[GI_T] = it->m_value_data.int_value;
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

	pThis->m_unit = GI_SQUAD_UNKNOWN;
	for (i = 0; i < GI_POSITION_UNKNOWN; i++)
	{
		if (strcmp(s_positionNames[i], position) == 0)
		{
			pThis->m_position = i;
			break;
		}
	}
	switch (pThis->m_position)
	{
		case GI_QB:
		case GI_RB:
		case GI_WR:
		case GI_TE:
		case GI_OC:
		case GI_OG:
		case GI_OT:
			pThis->m_unit = GI_OFFENCE;
			break;
		case GI_DE:
		case GI_DT:
		case GI_IB:
		case GI_OB:
		case GI_CB:
		case GI_SF:
			pThis->m_unit = GI_DEFENCE;
			break;
		case GI_R:
		case GI_K:
		case GI_P:
			pThis->m_unit = GI_SPECIALTEAMS;
			break;
		default:
			pThis->m_unit = GI_SQUAD_UNKNOWN;
			break;
	}

	return GI_SUCCESS;
}

void gi_Player_Print(gi_Player* const pThis)
{
	printf("Player:'%s' '%s' Position: %s Level:%d QST:%d %d %d Age:%d Experience:%.2f\n",
			pThis->m_name, 
			((pThis->m_unit == GI_OFFENCE) ? "Offence" : ((pThis->m_unit == GI_DEFENCE) ? "Defence" : "Special Teams")),
			s_positionNames[pThis->m_position], pThis->m_level, 
			pThis->m_qst[GI_Q], pThis->m_qst[GI_S], pThis->m_qst[GI_T], pThis->m_age, pThis->m_experience);
}

void gi_Player_ComputeSpecialTeams(gi_Player* const pThis)
{
	const GI_POSITION position = pThis->m_position;
	float blocker = 0.0f;
	float gunner = 0.0f;
	float protector = 0.0f;
	float runner = 0.0f;

	/* Personal Protector: RB, SF, TE, IB, OB: S x 1.5 */
	switch (position)
	{
		case GI_RB:
		case GI_SF:
		case GI_TE:
		case GI_IB:
		case GI_OB:
			protector = 1.5f * (float)pThis->m_level * (float)(pThis->m_qst[GI_S])/100.0f;
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
		case GI_DE:
		case GI_DT:
		case GI_OC:
		case GI_OG:
			blocker = 0.8f * (float)(pThis->m_level);
			break;
		case GI_IB:
		case GI_OB:
			blocker = 0.7f * (float)(pThis->m_level);
			break;
		case GI_TE:
		case GI_RB:
		case GI_SF:
			blocker = 0.33f * (float)(pThis->m_level) * (float)(pThis->m_qst[GI_S]+pThis->m_qst[GI_T])/100.0f;
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
		case GI_R:
			runner = 0.9f * (float)(pThis->m_level);
			break;
		case GI_CB:
		case GI_WR:
			runner = 0.8f * (float)(pThis->m_level);
			break;
		case GI_TE:
		case GI_RB:
		case GI_SF:
			runner = 0.33f * (float)(pThis->m_level) * (float)(pThis->m_qst[GI_Q]+pThis->m_qst[GI_T])/100.0f;
			break;
		default:
			runner = 0.0f;
			break;
	};

	/* Gunner: SF, WR, CB, R: Q x 1.5 */
	switch (position)
	{
		case GI_SF:
		case GI_WR:
		case GI_CB:
		case GI_R:
			gunner = 1.5f * (float)(pThis->m_level) * (float)(pThis->m_qst[GI_Q])/100.0f;
			break;
		default:
			gunner = 0.0f;
			break;
	};

	pThis->m_specialTeamsValues.m_blocker = blocker;
	pThis->m_specialTeamsValues.m_gunner = gunner;
	pThis->m_specialTeamsValues.m_protector = protector;
	pThis->m_specialTeamsValues.m_runner = runner;
}
