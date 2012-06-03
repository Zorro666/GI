#include <stdio.h>
#include <string.h>

#include "gi_Team.h"

typedef struct IntItem
{
	int m_value;
	size_t m_key;
} IntItem;

static int intItem_Compare(const void* a, const void* b)
{
	const int valueA = ((const IntItem*)a)->m_value;
	const int valueB = ((const IntItem*)b)->m_value;
	return (valueA < valueB);
}

typedef struct FloatItem
{
	float m_value;
	size_t m_key;
} FloatItem;

static int floatItem_Compare(const void* a, const void* b)
{
	const float valueA = ((const FloatItem*)a)->m_value;
	const float valueB = ((const FloatItem*)b)->m_value;
	return (valueA < valueB);
}

static void gi_Team_SortByPosition(gi_Team* const pThis)
{
	gi_Player squadTemp[MAX_NUM_SQUAD_PLAYERS];
	IntItem stats[MAX_NUM_SQUAD_PLAYERS];
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;

	if (numPlayers == 0)
	{
		return;
	}

	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_position;
		stats[i].m_key = i;
	}
	qsort(stats, numPlayers, sizeof(stats[0]), intItem_Compare);
	for (i = 0; i < numPlayers; i++)
	{
		squadTemp[i] = pThis->m_squad[i];
	}
	for (i = 0; i < numPlayers; i++)
	{
		size_t reverseI = numPlayers-i-1;
		const size_t playerIndex = stats[reverseI].m_key;
		pThis->m_squad[i] = squadTemp[playerIndex];
	}
}

static void gi_Team_UpdatePositionArrays(gi_Team* const pThis)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	size_t numOffence = 0;
	size_t numDefence = 0;
	size_t numSpecialTeams = 0;

	if (numPlayers == 0)
	{
		return;
	}

	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		const GI_SQUAD_UNIT unit = pPlayer->m_unit;
		if (unit == GI_OFFENCE)
		{
			pThis->m_offence[numOffence] = pPlayer;
			numOffence++;
		}
		else if (unit == GI_DEFENCE)
		{
			pThis->m_defence[numDefence] = pPlayer;
			numDefence++;
		}
		else if (unit == GI_SPECIALTEAMS)
		{
			pThis->m_specialTeams[numSpecialTeams] = pPlayer;
			numSpecialTeams++;
		}
	}
	pThis->m_numOffence = numOffence;
	pThis->m_numDefence = numDefence;
	pThis->m_numSpecialTeams = numSpecialTeams;
}

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
	size_t numPlayers = 0;

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

	gi_Team_SortByPosition(pThis);
	gi_Team_UpdatePositionArrays(pThis);

	gi_Team_ComputeSpecialTeams(pThis);

	return GI_SUCCESS;
}

void gi_Team_Print(gi_Team* const pThis, FILE* const pFile)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	if (numPlayers == 0)
	{
		return;
	}
	fprintf(pFile, "Team:'%s' %d\n", pThis->m_name, numPlayers);
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
}

void gi_Team_ComputeSpecialTeams(gi_Team* const pThis)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	if (numPlayers == 0)
	{
		return;
	}
	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		gi_Player_ComputeSpecialTeams(pPlayer);
	}
}

static void gi_Team_computeAndPrintStats(gi_Team* const pThis, FILE* const pFile, FloatItem* const pStats, const char* const statName)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	if (numPlayers == 0)
	{
		return;
	}
	qsort(pStats, numPlayers, sizeof(pStats[0]), floatItem_Compare);
	fprintf(pFile, "\n");
	fprintf(pFile, "**** %s ****\n", statName);
	for (i = 0; i < numPlayers; i++)
	{
		if (pStats[i].m_value > 0.00000001f)
		{
			const size_t playerIndex = pStats[i].m_key;
			gi_Player* const pPlayer = &pThis->m_squad[playerIndex];
			fprintf(pFile, "Player[%d] '%s' Position:%s '%s':%f\n", playerIndex, pPlayer->m_name, gi_GetPositionName(pPlayer->m_position), 
							statName, pStats[i].m_value);
		}
	}
}

void gi_Team_PrintBestSpecialTeams(gi_Team* const pThis, FILE* const pFile)
{
	FloatItem stats[MAX_NUM_SQUAD_PLAYERS];
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;

	if (numPlayers == 0)
	{
		return;
	}

	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_blocker;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Blocker");

	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_gunner;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Gunner");

	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_protector;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Protector");

	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		stats[i].m_value = pPlayer->m_specialTeamsValues.m_runner;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Runner");
}

