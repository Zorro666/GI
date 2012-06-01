#include <stdio.h>
#include <string.h>

#include "gi_Player.h"

static char* s_offenceNames[] = {"QB", "RB", "TE", "WR", "C", "OG", "OC", "ZZ"};
static char* s_defenceNames[] = {"DE", "DT", "IB", "OB", "CB", "SF", "ZZ"};
static char* s_specialTeamsNames[] = {"R", "K", "P", "ZZ"};

void gi_Player_Init(gi_Player* const pThis)
{
	pThis->m_name[0] = '\0';
	pThis->m_position[0] = '\0';
	pThis->m_level = 0;
	pThis->m_qst[0] = 0;
	pThis->m_qst[1] = 0;
	pThis->m_qst[2] = 0;
	pThis->m_experience = 0.0f;
	pThis->m_unit = GI_UNKNOWN;
	pThis->m_age = -1;
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
				strncpy(pThis->m_position, it->m_value_data.string_value, MAX_POSITION_SIZE);
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

	pThis->m_unit = GI_UNKNOWN;
	for (i = 0; ; i++)
	{
		if (strcmp(s_offenceNames[i], "ZZ") == 0)
		{
			break;
		}
		if (strcmp(pThis->m_position, s_offenceNames[i]) == 0)
		{
			pThis->m_unit = GI_OFFENCE;
			break;
		}
	}
	for (i = 0; ; i++)
	{
		if (strcmp(s_defenceNames[i], "ZZ") == 0)
		{
			break;
		}
		if (strcmp(pThis->m_position, s_defenceNames[i]) == 0)
		{
			pThis->m_unit = GI_DEFENCE;
			break;
		}
	}
	for (i = 0; ; i++)
	{
		if (strcmp(s_specialTeamsNames[i], "ZZ") == 0)
		{
			break;
		}
		if (strcmp(pThis->m_position, s_specialTeamsNames[i]) == 0)
		{
			pThis->m_unit = GI_SPECIALTEAMS;
			break;
		}
	}
	return GI_SUCCESS;
}

void gi_Player_Print(gi_Player* const pThis)
{
	printf("Player:'%s' '%s' Position: %s Level:%d QST:%d %d %d Age:%d Experience:%.2f\n",
			pThis->m_name, 
			((pThis->m_unit == GI_OFFENCE) ? "Offence" : ((pThis->m_unit == GI_DEFENCE) ? "Defence" : "Special Teams")),
			pThis->m_position, pThis->m_level, 
			pThis->m_qst[GI_Q], pThis->m_qst[GI_S], pThis->m_qst[GI_T], pThis->m_age, pThis->m_experience);
}
