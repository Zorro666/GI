#include <stdio.h>
#include <string.h>

#include "gi_Team.h"

void gi_Team_Init(gi_Team* const pThis)
{
	int i;
	for (i = 0; i < MAX_NUM_OFFENCE_PLAYERS; i++)
	{
		gi_Player_Init(&pThis->m_offence[i]);
	}
	for (i = 0; i < MAX_NUM_DEFENCE_PLAYERS; i++)
	{
		gi_Player_Init(&pThis->m_defence[i]);
	}
	pThis->m_name[0] = '\0';
	pThis->m_numOffence = 0;
	pThis->m_numDefence = 0;
}

GI_Return gi_Team_Load(gi_Team* const pThis, const Json_Value* const root)
{
	Json_Value* it;
	int numOffence = 0;
	int numDefence = 0;

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
	if (strcmp(root->m_name, "Team") != 0)
	{
		return GI_ERROR;
	}

	gi_Team_Init(pThis);

	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, MAX_TEAMNAME_SIZE);
			}
		}
		if (it->m_type == JSON_ARRAY)
		{
			if (strcmp(it->m_name, "Squad") == 0)
			{
				Json_Value* it2;
				for (it2 = it->m_first_child; it2 != NULL; it2 = it2->m_next_sibling)
				{
					Json_Value* playerRoot;
					if (it2->m_type == JSON_OBJECT)
					{
						gi_Player* pPlayer = &pThis->m_offence[numOffence];
						playerRoot = it2->m_first_child;
						if (gi_Player_Load(pPlayer, playerRoot) == GI_SUCCESS)
						{
							numOffence++;
						}
					}
				}
			}
		}
	}
	pThis->m_numOffence = numOffence;
	pThis->m_numDefence = numDefence;

	return GI_SUCCESS;
}

void gi_Team_Print(gi_Team* const pThis)
{
	int i;
	printf("Team:'%s'\n", pThis->m_name);
	printf("Offence: %d\n", pThis->m_numOffence);
	for (i = 0; i < MAX_NUM_OFFENCE_PLAYERS; i++)
	{
		if (pThis->m_offence[i].m_name[0] != '\0')
		{
			gi_Player_Print(&pThis->m_offence[i]);
		}
	}
	printf("Defence: %d\n", pThis->m_numDefence);
	for (i = 0; i < MAX_NUM_DEFENCE_PLAYERS; i++)
	{
		if (pThis->m_defence[i].m_name[0] != '\0')
		{
			gi_Player_Print(&pThis->m_defence[i]);
		}
	}
}
