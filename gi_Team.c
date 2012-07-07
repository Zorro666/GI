#include <stdio.h>
#include <string.h>

#include "gi_Team.h"
#include "gi_TeamPrivate.h"

#include "gi_OffencePlay.h"
#include "gi_DefencePlay.h"
#include "gi_SpecialTeamsValues.h"
#include "gi_Compare.h"
#include "gi_PlayInfo.h"
#include "gi_Logger.h"

static gi_Player* gi_Team_FindPlayer(gi_Team* const pThis, const char* const playerName, size_t* const pPlayerIndex)
{
	const size_t numPlayers = pThis->m_numPlayers;
	size_t i;
	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		if (strcmp(gi_Player_GetName(pPlayer), playerName) == 0)
		{
			if (pPlayerIndex != NULL)
			{
				*pPlayerIndex = i;
			}
			return pPlayer;
		}
	}
	if (pPlayerIndex != NULL)
	{
		*pPlayerIndex = GI_SQUAD_PLAYERS_MAX_SIZE;
	}
	return NULL;
}

static gi_Player* gi_Team_FindPlayerUnit(gi_Team* const pThis, const char* const playerName, const GI_UNIT unit, size_t* const pPlayerIndex)
{
	const size_t numPlayers = pThis->m_numPlayers;
	size_t i;
	for (i = 0; i < numPlayers; i++)
	{
		gi_Player* const pPlayer = &pThis->m_squad[i];
		if (strcmp(gi_Player_GetName(pPlayer), playerName) == 0)
		{
			if (gi_Player_GetUnit(pPlayer) == unit)
			{
				if (pPlayerIndex != NULL)
				{
					*pPlayerIndex = i;
				}
				return pPlayer;
			}
		}
	}
	if (pPlayerIndex != NULL)
	{
		*pPlayerIndex = GI_SQUAD_PLAYERS_MAX_SIZE;
	}
	return NULL;
}

static GI_RETURN gi_Team_AddUsedPlayer(gi_Team* const pThis, const char* const usedPlayerName, const GI_UNIT usedPlayerUnit)
{
	gi_Player* pPlayer = NULL;
	size_t playerIndex = GI_SQUAD_PLAYERS_MAX_SIZE;
	/* Any player is allowed to be on special teams */
	if (usedPlayerUnit == GI_UNIT_SPECIALTEAMS)
	{
		pPlayer = gi_Team_FindPlayer(pThis, usedPlayerName, &playerIndex);
	}
	else
	{
		pPlayer = gi_Team_FindPlayerUnit(pThis, usedPlayerName, usedPlayerUnit, &playerIndex);
	}
	if ((pPlayer != NULL) && (playerIndex < GI_SQUAD_PLAYERS_MAX_SIZE))
	{
		pThis->m_usedPlayers[usedPlayerUnit][playerIndex] = 1;
		return GI_RETURN_SUCCESS;
	}
	return GI_RETURN_ERROR;
}

static GI_RETURN gi_Team_SetPlayerInjury(gi_Team* const pThis, const char* const injuryPlayerName, const GI_INJURY injury)
{
	gi_Player* const pPlayer = gi_Team_FindPlayer(pThis, injuryPlayerName, NULL);
	if (pPlayer != NULL)
	{
		gi_Player_SetInjury(pPlayer, injury);
		return GI_RETURN_SUCCESS;
	}
	return GI_RETURN_ERROR;
}

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
		stats[i].m_value = gi_Player_GetPosition(pPlayer);
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
		const GI_UNIT unit = gi_Player_GetUnit(pPlayer);
		const size_t position = gi_Player_GetPosition(pPlayer);
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
			fprintf(pFile, "Player[%d] '%s' Position:%s '%s':%f\n", playerIndex, 
							gi_Player_GetName(pPlayer), gi_Player_GetPositionName(pPlayer), 
							statName, pStats[i].m_value);
		}
	}
}

static void gi_PickBestPlayers(const size_t numPlayersToPick, size_t* const pUsedPlayers, size_t* const pNumUsedPlayers, 
															 const FloatItem* const pStats, const size_t numPlayers)
{
	size_t i;
	size_t numUsedPlayers = *pNumUsedPlayers;
	for (i = 0; i < numPlayersToPick; i++)
	{
		size_t p;
		size_t playerIndex = GI_SQUAD_PLAYERS_MAX_SIZE;
		for (p = 0; p < numPlayers; p++)
		{
			size_t u;
			playerIndex = pStats[p].m_key;
			for (u = 0; u < numUsedPlayers; u++)
			{
				if (pUsedPlayers[u] == playerIndex)
				{
					playerIndex = GI_SQUAD_PLAYERS_MAX_SIZE;
					break;
				}
			}
			if (playerIndex != GI_SQUAD_PLAYERS_MAX_SIZE)
			{
				break;
			}
		}
		pUsedPlayers[numUsedPlayers] = playerIndex;
		numUsedPlayers++;
	}
	*pNumUsedPlayers = numUsedPlayers;
}

typedef float StatFunc(const gi_SpecialTeamsValues* const pSpecialTeamsValues);

static void gi_Team_BestStatHelper(const gi_Team* const pThis, const gi_PlayInfo* const pPlayInfo, 
																	 size_t* pNumUsedPlayers, size_t* const pUsedPlayers, 
																	 StatFunc func, const size_t numForStat, const char* const statName, FILE* const pFile)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	size_t lastNumUsedPlayers;
	size_t numUsedPlayers = *pNumUsedPlayers;
	FloatItem stats[GI_SQUAD_PLAYERS_MAX_SIZE];

	for (i = 0; i < numPlayers; i++)
	{
		const gi_SpecialTeamsValues* const pSpecialTeamsValues = gi_PlayInfo_GetSpecialTeamsValuesForPlayer(pPlayInfo, i);
		/*stats[i].m_value = gi_SpecialTeamsValues_GetBlocker(pSpecialTeamsValues);*/
		stats[i].m_value = func(pSpecialTeamsValues);
		stats[i].m_key = i;
	}
	gi_Team_computeAndPrintStats(pThis, pFile, stats, statName);
	lastNumUsedPlayers = numUsedPlayers;
	gi_PickBestPlayers(numForStat, pUsedPlayers, &numUsedPlayers, stats, numPlayers);
	for (i = lastNumUsedPlayers; i < numUsedPlayers; i++)
	{
		const size_t playerIndex = pUsedPlayers[i];
		GI_LOG("Best %ss[%d] Player[%d] '%s'", statName, i-lastNumUsedPlayers, playerIndex, 
						gi_Player_GetName(&(pThis->m_squad[playerIndex])));
	}
	*pNumUsedPlayers = numUsedPlayers;
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
	for (i = 0; i < GI_UNIT_NUM; i++)
	{
		size_t p;
		for (p = 0; p < GI_SQUAD_PLAYERS_MAX_SIZE; p++)
		{
			pThis->m_usedPlayers[i][p] = GI_SQUAD_PLAYERS_MAX_SIZE;
		}
	}

	pThis->m_name[0] = '\0';
	pThis->m_numPlayers = 0;
	pThis->m_numOffence = 0;
	pThis->m_numDefence = 0;
	pThis->m_numSpecialTeams = 0;
}

GI_BOOL gi_Team_IsValueValid(const Json_Value* const root)
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

GI_RETURN gi_Team_Load(gi_Team* const pThis, const Json_Value* const root)
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
		else if (it->m_type == JSON_ARRAY)
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
							if (gi_Player_GetUnit(&player) != GI_UNIT_UNKNOWN)
							{
								pThis->m_squad[numPlayers] = player;
								numPlayers++;
							}
						}
					}
				}
				pThis->m_numPlayers = numPlayers;
				gi_Team_SortByPosition(pThis);
				gi_Team_UpdatePositionArrays(pThis);
			}
			else if (strcmp(it->m_name, "Injuries") == 0)
			{
				Json_Value* injuryRoot;
				for (injuryRoot = it->m_first_child; injuryRoot != NULL; injuryRoot = injuryRoot->m_next_sibling)
				{
					if (injuryRoot->m_type == JSON_OBJECT)
					{
						Json_Value* const injuryObject = injuryRoot->m_first_child;
						if (injuryObject->m_type == JSON_STRING)
						{
							const char* const injuryPlayerName = injuryObject->m_name;
							const char* const injuryString = injuryObject->m_value_data.string_value;
							const GI_INJURY injury = gi_GetInjuryFromName(injuryString);
							GI_LOG("Injury '%s' : '%s' -> %d '%s'", injuryPlayerName, injuryString, injury, gi_GetInjuryName(injury));
							if (gi_Team_SetPlayerInjury(pThis, injuryPlayerName, injury) == GI_RETURN_ERROR)
							{
								GI_FATAL_ERROR("gi_Team_SetPlayerInjury failed Injury:'%s' %d '%s'", injuryPlayerName, injury, gi_GetInjuryName(injury));
								return GI_RETURN_ERROR;
							}
						}
						else
						{
							GI_FATAL_ERROR("Wrong type found for injury data found:%d", injuryObject->m_type);
							return GI_RETURN_ERROR;
						}
					}
					else
					{
						GI_FATAL_ERROR("Wrong type found for Injuries object found:%d", injuryRoot->m_type);
						return GI_RETURN_ERROR;
					}
				}
			}
			else 
			{
				GI_FATAL_ERROR("Unknown ARRAY object found:'%s'", it->m_name);
				return GI_RETURN_ERROR;
			}
		}
		else if (it->m_type == JSON_OBJECT)
		{
			if (strcmp(it->m_name, "UsedPlayers") == 0)
			{
				Json_Value* usedUnitRoot;
				for (usedUnitRoot = it->m_first_child; usedUnitRoot != NULL; usedUnitRoot = usedUnitRoot->m_next_sibling)
				{
					if (usedUnitRoot->m_type == JSON_ARRAY)
					{
						Json_Value* usedPlayersRoot;
						GI_UNIT usedPlayersUnit = GI_UNIT_UNKNOWN;
						const char* const usedPlayersUnitName = usedUnitRoot->m_name;
						if (usedPlayersUnitName)
						{
							if (strcmp(usedPlayersUnitName, "Offence") == 0)
							{
								usedPlayersUnit = GI_UNIT_OFFENCE;
							}
							else if (strcmp(usedPlayersUnitName, "Defence") == 0)
							{
								usedPlayersUnit = GI_UNIT_DEFENCE;
							}
							else if (strcmp(usedPlayersUnitName, "Special Teams") == 0)
							{
								usedPlayersUnit = GI_UNIT_SPECIALTEAMS;
							}
						}
						else
						{
							GI_FATAL_ERROR("Used Players object name is NULL");
							return GI_RETURN_ERROR;
						}
						if (usedPlayersUnit == GI_UNIT_UNKNOWN)
						{
							GI_FATAL_ERROR("Unknown Used Players unit name:'%s'", usedPlayersUnitName);
							return GI_RETURN_ERROR;
						}
						for (usedPlayersRoot = usedUnitRoot->m_first_child; usedPlayersRoot != NULL; usedPlayersRoot = usedPlayersRoot->m_next_sibling)
						{
							if (usedPlayersRoot->m_type == JSON_STRING)
							{
								const char* const usedPlayerName = usedPlayersRoot->m_value_data.string_value;
								GI_LOG("Used Player '%s' Unit '%s'", usedPlayerName, gi_GetUnitName(usedPlayersUnit));
								if (gi_Team_AddUsedPlayer(pThis, usedPlayerName, usedPlayersUnit) == GI_RETURN_ERROR)
								{
									GI_FATAL_ERROR("gi_Team_AddUsedPlayer failed Player:'%s' Unit:'%s'", usedPlayerName, gi_GetUnitName(usedPlayersUnit));
									return GI_RETURN_ERROR;
								}
							}
							else
							{
								GI_FATAL_ERROR("Wrong type found for Used Players data found:%d unit:'%s'", usedPlayersRoot->m_type, usedPlayersUnitName);
								return GI_RETURN_ERROR;
							}
						}
					}
					else
					{
						GI_FATAL_ERROR("Wrong type found for Used Players object found:%d", usedUnitRoot->m_type);
						return GI_RETURN_ERROR;
					}
				}
			}
		}
	}

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

void gi_Team_PrintBestSpecialTeams(const gi_Team* const pThis, const gi_PlayInfo* const pPlayInfo, FILE* const pFile)
{
	size_t i;
	const size_t numPlayers = pThis->m_numPlayers;
	size_t usedPlayers[GI_SQUAD_PLAYERS_MAX_SIZE];
	size_t numUsedPlayers = 0;
	const size_t numBlockers = 7;
	const size_t numGunners = 1;
	const size_t numProtectors = 1;
	const size_t numRunners = 4;

	if (numPlayers == 0)
	{
		return;
	}
	for (i = 0; i < numPlayers; i++)
	{
		usedPlayers[i] = GI_SQUAD_PLAYERS_MAX_SIZE;
	}

	gi_Team_BestStatHelper(pThis, pPlayInfo, &numUsedPlayers, usedPlayers, gi_SpecialTeamsValues_GetBlocker, numBlockers, "Blocker", pFile);
	gi_Team_BestStatHelper(pThis, pPlayInfo, &numUsedPlayers, usedPlayers, gi_SpecialTeamsValues_GetGunner, numGunners, "Gunner", pFile);
	gi_Team_BestStatHelper(pThis, pPlayInfo, &numUsedPlayers, usedPlayers, gi_SpecialTeamsValues_GetProtector, numProtectors, "Protector", pFile);
	gi_Team_BestStatHelper(pThis, pPlayInfo, &numUsedPlayers, usedPlayers, gi_SpecialTeamsValues_GetRunner, numRunners, "Runner", pFile);
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
			const float baseValue = gi_PlayInfo_GetOffenceStatsBaseValue(pPlayInfo, p, i);
			if (baseValue > 0.0f)
			{
				const gi_OffencePlay* const pOffencePlay = gi_PlayInfo_GetOffencePlay(pPlayInfo, p);
				GI_LOG("Play '%s' Defence:'%s' Player[%d] '%s' %s value %f", 
								gi_OffencePlay_GetName(pOffencePlay), gi_OffencePlay_GetDefence(pOffencePlay, 0),
								i, gi_Player_GetName(pPlayer), gi_Player_GetPositionName(pPlayer), baseValue);
			}
		}
	}
}

void gi_Team_ComputeDefenceBase(const gi_Team* const pThis, gi_PlayInfo* const pPlayInfo)
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
		gi_PlayInfo_ComputeDefenceBase(pPlayInfo, pPlayer, i);
		for (p = 0; p < GI_DEFENCE_PLAYS_MAX_SIZE; p++)
		{
			const float baseValue = gi_PlayInfo_GetDefenceStatsBaseValue(pPlayInfo, p, i);
			if (baseValue > 0.0f)
			{
				const gi_DefencePlay* const pDefencePlay = gi_PlayInfo_GetDefencePlay(pPlayInfo, p);
				GI_LOG("Play '%s' Zone:'%s' Player[%d] '%s' %s value %f", 
								gi_DefencePlay_GetName(pDefencePlay), gi_DefencePlay_GetZone(pDefencePlay),
								i, gi_Player_GetName(pPlayer), gi_Player_GetPositionName(pPlayer), baseValue);
			}
		}
	}
}

const char* gi_Team_GetName(const gi_Team* const pThis)
{
	return pThis->m_name;
}
