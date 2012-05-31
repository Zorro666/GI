#include <stdio.h>
#include <string.h>

#include "gi_Player.h"

void gi_Player_Init(gi_Player* const pThis)
{
	pThis->m_name[0] = '\0';
	pThis->m_position[0] = '\0';
	pThis->m_level = 0;
	pThis->m_qst[0] = 0;
	pThis->m_qst[1] = 0;
	pThis->m_qst[2] = 0;
	pThis->m_experience = 0.0f;
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
			if (strcmp(it->m_name, "Position") == 0)
			{
				strncpy(pThis->m_position, it->m_value_data.string_value, MAX_POSITION_SIZE);
			}
		}
		if (it->m_type == JSON_INT)
		{
			if (strcmp(it->m_name, "Level") == 0)
			{
				pThis->m_level = it->m_value_data.int_value;
			}
			if (strcmp(it->m_name, "Q") == 0)
			{
				pThis->m_qst[GI_Q] = it->m_value_data.int_value;
			}
			if (strcmp(it->m_name, "S") == 0)
			{
				pThis->m_qst[GI_S] = it->m_value_data.int_value;
			}
			if (strcmp(it->m_name, "T") == 0)
			{
				pThis->m_qst[GI_T] = it->m_value_data.int_value;
			}
		}
		if (it->m_type == JSON_FLOAT)
		{
			if (strcmp(it->m_name, "Experience") == 0)
			{
				pThis->m_experience = it->m_value_data.float_value;
			}
		}
	}

	return GI_SUCCESS;
}

void gi_Player_Print(gi_Player* const pThis)
{
	printf("Player:'%s' Position: %s Level:%d QST:%d %d %d Experience:%.2f\n",
			pThis->m_name, pThis->m_position, pThis->m_level, 
			pThis->m_qst[GI_Q], pThis->m_qst[GI_S], pThis->m_qst[GI_T], pThis->m_experience);
}
