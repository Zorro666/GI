#include <stdio.h>
#include <string.h>

#include "gi_Team.h"
#include "gi_Compare.h"
#include "gi_PlayInfo.h"
#include "gi_Logger.h"

static void gi_Team_SortByPosition(gi_Team* const pThis)
{
	gi_Player squadTemp[GI_SQUAD_PLAYERS_MAX_SIZE];
	SizetItem stats[GI_SQUAD_PLAYERS_MAX_SIZE];
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
	qsort(stats, numPlayers, sizeof(stats[0]), SizetItem_CompareBigger);
	for (i = 0; i < numPlayers; i++)
	{
		squadTemp[i] = pThis->m_squad[i];
	}
	for (i = 0; i < numPlayers; i++)
	{
		const size_t playerIndex = stats[i].m_key;
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
	size_t currentPosition;
	size_t currentStart;

	if (numPlayers == 0)
	{
		return;
	}

	currentStart = 0;
	currentPosition = GI_POSITION_UNKNOWN;
	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		const GI_UNIT unit = pPlayer->m_unit;
		const size_t position = pPlayer->m_position;
		if (position != currentPosition)
		{
			if ((position < currentPosition) && (currentPosition != GI_POSITION_UNKNOWN))
			{
				GI_FATAL_ERROR("unsorted positions in squad %d < %d\n", position, currentPosition);
				return;
			}
			if (currentPosition != GI_POSITION_UNKNOWN)
			{
				pThis->m_positionCounts[currentPosition] = i - currentStart;
			}
			pThis->m_positionStarts[position] = i;
			currentPosition = position;
			currentStart = i;
		}

		if (unit == GI_UNIT_OFFENCE)
		{
			pThis->m_offence[numOffence] = pPlayer;
			numOffence++;
		}
		else if (unit == GI_UNIT_DEFENCE)
		{
			pThis->m_defence[numDefence] = pPlayer;
			numDefence++;
		}
		else if (unit == GI_UNIT_SPECIALTEAMS)
		{
			pThis->m_specialTeams[numSpecialTeams] = pPlayer;
			numSpecialTeams++;
		}
	}
	pThis->m_positionCounts[currentPosition] = numPlayers - pThis->m_positionStarts[currentPosition];

#if 0
	for (i = 0; i < GI_NUM_POSITIONS; i++)
	{
		GI_FATAL_ERROR("position[%d] '%s': start:%d count:%d\n", i, gi_GetPositionName(i), 
						pThis->m_positionStarts[i], pThis->m_positionCounts[i]);
	}
#endif /*#if 0*/

	pThis->m_numOffence = numOffence;
	pThis->m_numDefence = numDefence;
	pThis->m_numSpecialTeams = numSpecialTeams;
}

void gi_Team_Init(gi_Team* const pThis)
{
	size_t i;
	for (i = 0; i < GI_SQUAD_PLAYERS_MAX_SIZE; i++)
	{
		gi_Player_Init(&pThis->m_squad[i]);
	}
	for (i = 0; i < GI_OFFENCE_PLAYERS_MAX_SIZE; i++)
	{
		pThis->m_offence[i] = NULL;
	}
	for (i = 0; i < GI_DEFENCE_PLAYERS_MAX_SIZE; i++)
	{
		pThis->m_defence[i] = NULL;
	}
	for (i = 0; i < GI_SPECIALTEAMS_PLAYERS_MAX_SIZE; i++)
	{
		pThis->m_specialTeams[i] = NULL;
	}
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		pThis->m_positionStarts[i] = 0;
		pThis->m_positionCounts[i] = 0;
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
		return GI_RETURN_ERROR;
	}

	gi_Team_Init(pThis);

	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_STRING)
		{
			if (strcmp(it->m_name, "Name") == 0)
			{
				strncpy(pThis->m_name, it->m_value_data.string_value, GI_TEAMNAME_MAX_SIZE);
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
						if (gi_Player_Load(&player, playerRoot) == GI_RETURN_SUCCESS)
						{
							if (player.m_unit != GI_UNIT_UNKNOWN)
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

	return GI_RETURN_SUCCESS;
}

void gi_Team_Print(const gi_Team* const pThis, FILE* const pFile)
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

void gi_Team_ComputeSpecialTeams(const gi_Team* const pThis, gi_PlayInfo* const pPlayInfo)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	if (numPlayers == 0)
	{
		return;
	}
	for (i = 0; i < numPlayers; i++)
	{
		const gi_Player* const pPlayer = &pThis->m_squad[i];
		gi_PlayInfo_ComputeSpecialTeams(pPlayInfo, pPlayer, i);
	}
}

static void gi_Team_computeAndPrintStats(const gi_Team* const pThis, FILE* const pFile, 
																				 FloatItem* const pStats, const char* const statName)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	if (numPlayers == 0)
	{
		return;
	}
	qsort(pStats, numPlayers, sizeof(pStats[0]), FloatItem_CompareSmaller);
	fprintf(pFile, "\n");
	fprintf(pFile, "**** %s ****\n", statName);
	for (i = 0; i < numPlayers; i++)
	{
		if (pStats[i].m_value > 0.00000001f)
		{
			const size_t playerIndex = pStats[i].m_key;
			const gi_Player* const pPlayer = &pThis->m_squad[playerIndex];
			fprintf(pFile, "Player[%d] '%s' Position:%s '%s':%f\n", playerIndex, pPlayer->m_name, gi_GetPositionName(pPlayer->m_position), 
							statName, pStats[i].m_value);
		}
	}
}

void gi_Team_PrintBestSpecialTeams(const gi_Team* const pThis, const gi_PlayInfo* const pPlayInfo, FILE* const pFile)
{
	FloatItem stats[GI_SQUAD_PLAYERS_MAX_SIZE];
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;

	if (numPlayers == 0)
	{
		return;
	}

	for (i = 0; i < numPlayers; i++)
	{
		stats[i].m_value = pPlayInfo->m_specialTeamsValues[i].m_blocker;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Blocker");

	for (i = 0; i < numPlayers; i++)
	{
		stats[i].m_value = pPlayInfo->m_specialTeamsValues[i].m_gunner;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Gunner");

	for (i = 0; i < numPlayers; i++)
	{
		stats[i].m_value = pPlayInfo->m_specialTeamsValues[i].m_protector;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Protector");

	for (i = 0; i < numPlayers; i++)
	{
		stats[i].m_value = pPlayInfo->m_specialTeamsValues[i].m_runner;
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, "Runner");
}

void gi_Team_ComputeOffenceBase(const gi_Team* const pThis, gi_PlayInfo* const pPlayInfo)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;

	if (numPlayers == 0)
	{
		return;
	}
	for (i = 0; i < numPlayers; i++)
	{
		size_t p;
		const gi_Player* const pPlayer = &pThis->m_squad[i];
		gi_PlayInfo_ComputeOffenceBase(pPlayInfo, pPlayer, i);
		for (p = 0; p < GI_OFFENCE_PLAYS_MAX_SIZE; p++)
		{
			const float baseValue = pPlayInfo->m_offenceStatsBase[p][i];
			if (baseValue > 0.0f)
			{
				GI_LOG("Play '%s' Player[%d] '%s' %s value %f", 
								pPlayInfo->m_offencePlays[p].m_name, 
								i, pPlayer->m_name, gi_GetPositionName(pPlayer->m_position), baseValue);
			}
		}
	}
}

