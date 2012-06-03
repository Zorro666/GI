#include <stdio.h>
#include <string.h>

#include "gi_Team.h"

void gi_Team_Init(gi_Team* const pThis)
{
	int i;
	for (i = 0; i < MAX_NUM_SQUAD_PLAYERS; i++)
	{
		gi_Player_Init(&pThis->m_squad[i]);
	}
	for (i = 0; i < MAX_NUM_OFFENCE_PLAYERS; i++)
	{
		pThis->m_offence[i] = NULL;
	}
	for (i = 0; i < MAX_NUM_DEFENCE_PLAYERS; i++)
	{
		pThis->m_defence[i] = NULL;
	}
	for (i = 0; i < MAX_NUM_SPECIALTEAMS_PLAYERS; i++)
	{
		pThis->m_specialTeams[i] = NULL;
	}
	pThis->m_name[0] = '\0';
	pThis->m_numPlayers = 0;
	pThis->m_numOffence = 0;
	pThis->m_numDefence = 0;
	pThis->m_numSpecialTeams = 0;
}

GI_Bool gi_Team_IsValueValid(const Json_Value* const root)
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
	if (strcmp(root->m_name, "Team") != 0)
	{
		return GI_FALSE;
	}
	return GI_TRUE;
}

GI_Return gi_Team_Load(gi_Team* const pThis, const Json_Value* const root)
{
	Json_Value* it;
	int numOffence = 0;
	int numDefence = 0;
	int numSpecialTeams = 0;
	int numPlayers = 0;

	if (gi_Team_IsValueValid(root) == GI_FALSE)
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
					Json_Value* playerRoot = it2->m_first_child;
					if (gi_Player_IsValueValid(playerRoot) == GI_TRUE)
					{
						gi_Player player;
						playerRoot = it2->m_first_child;
						if (gi_Player_Load(&player, playerRoot) == GI_SUCCESS)
						{
							if (player.m_unit == GI_OFFENCE)
							{
								pThis->m_offence[numOffence] = &pThis->m_squad[numPlayers];
								numOffence++;
							}
							else if (player.m_unit == GI_DEFENCE)
							{
								pThis->m_defence[numDefence] = &pThis->m_squad[numPlayers];
								numDefence++;
							}
							else if (player.m_unit == GI_SPECIALTEAMS)
							{
								pThis->m_specialTeams[numSpecialTeams] = &pThis->m_squad[numPlayers];
								numSpecialTeams++;
							}
							if (player.m_unit != GI_SQUAD_UNKNOWN)
							{
								pThis->m_squad[numPlayers] = player;
								numPlayers++;
							}
						}
					}
				}
			}
		}
	}
	pThis->m_numPlayers = numPlayers;
	pThis->m_numOffence = numOffence;
	pThis->m_numDefence = numDefence;
	pThis->m_numSpecialTeams = numSpecialTeams;

	gi_Team_ComputeSpecialTeams(pThis);

	return GI_SUCCESS;
}

void gi_Team_Print(gi_Team* const pThis, FILE* const pFile)
{
	int i;
	fprintf(pFile, "Team:'%s' %d\n", pThis->m_name, pThis->m_numPlayers);
	fprintf(pFile, "Offence: %d\n", pThis->m_numOffence);
	for (i = 0; i < pThis->m_numOffence; i++)
	{
		gi_Player_Print(pThis->m_offence[i], pFile);
	}
	fprintf(pFile,"Defence: %d\n", pThis->m_numDefence);
	for (i = 0; i < pThis->m_numDefence; i++)
	{
		gi_Player_Print(pThis->m_defence[i], pFile);
	}
	fprintf(pFile, "SpecialTeams: %d\n", pThis->m_numSpecialTeams);
	for (i = 0; i < pThis->m_numSpecialTeams; i++)
	{
		gi_Player_Print(pThis->m_specialTeams[i], pFile);
	}
	gi_Team_PrintBestSpecialTeams(pThis, pFile);
}

void gi_Team_ComputeSpecialTeams(gi_Team* const pThis)
{
	int i;
	for (i = 0; i < pThis->m_numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		gi_Player_ComputeSpecialTeams(pPlayer);
	}
}

typedef struct FloatItem
{
	float m_value;
	int m_key;
} FloatItem;

static int floatItem_Compare(const void* a, const void* b)
{
	const float valueA = ((const FloatItem*)a)->m_value;
	const float valueB = ((const FloatItem*)b)->m_value;
	return (valueA < valueB);
}

static void gi_Team_computeAndPrintStats(gi_Team* const pThis, FILE* const pFile, FloatItem* const pStats, const char* const statName)
{
	int i;
	qsort(pStats, (size_t)(pThis->m_numPlayers), sizeof(pStats[0]), floatItem_Compare);
	fprintf(pFile, "\n");
	for (i = 0; i < pThis->m_numPlayers; i++)
	{
		if (pStats[i].m_value > 0.00000001f)
		{
			const int playerIndex = pStats[i].m_key;
			gi_Player* const pPlayer = &pThis->m_squad[playerIndex];
			fprintf(pFile, "Player[%d] '%s' '%s':%f\n", playerIndex, pPlayer->m_name, statName, pStats[i].m_value);
		}
	}
}

void gi_Team_PrintBestSpecialTeams(gi_Team* const pThis, FILE* const pFile)
{
	int i;
	FloatItem stats[MAX_NUM_SQUAD_PLAYERS];

	for (i = 0; i < pThis->m_numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_blocker;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Blocker");

	for (i = 0; i < pThis->m_numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_gunner;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Gunner");

	for (i = 0; i < pThis->m_numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_protector;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Protector");

	for (i = 0; i < pThis->m_numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_runner;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Runner");
}

