#include <stdio.h>
#include <string.h>

#include "Player.h"

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

GI_Return gi_Player_Load(gi_Player* const pThis, const Json_Value* const root)
{
	if (root->m_type != JSON_OBJECT)
	{
		fprintf(stderr, "NOT JSON_OBJECT\n");
		return GI_ERROR;
	}
	if (root->m_name == NULL)
	{
		fprintf(stderr, "name is NULL\n");
		return GI_ERROR;
	}
	if (strcmp(root->m_name, "Player") != 0)
	{
		return GI_ERROR;
	}

	gi_Player_Init(pThis);

	return GI_ERROR;
}

void gi_Player_Print(gi_Player* const pThis)
{
	printf("Player:'%s' Position: %s Level:%d QST:%d %d %d Experience:%.2f\n",
			pThis->m_name, pThis->m_position, pThis->m_level, 
			pThis->m_qst[GI_Q], pThis->m_qst[GI_S], pThis->m_qst[GI_T], pThis->m_experience);
}
