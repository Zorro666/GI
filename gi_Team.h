#ifndef GI_TEAM_HH
#define GI_TEAM_HH

#include <stdio.h>
#include "gi.h"
#include "gi_Player.h"

struct gi_Team
{
	gi_Player m_squad[GI_SQUAD_PLAYERS_MAX_SIZE];
	gi_Player* m_offence[GI_OFFENCE_PLAYERS_MAX_SIZE];
	gi_Player* m_defence[GI_DEFENCE_PLAYERS_MAX_SIZE];
	gi_Player* m_specialTeams[GI_SPECIALTEAMS_PLAYERS_MAX_SIZE];
	size_t m_positionStarts[GI_POSITION_NUM];
	size_t m_positionCounts[GI_POSITION_NUM];
	char m_name[GI_TEAMNAME_MAX_SIZE];
	size_t m_numPlayers;
	size_t m_numOffence;
	size_t m_numDefence;
	size_t m_numSpecialTeams;
};

void gi_Team_Init(gi_Team* const pThis);
GI_Bool gi_Team_IsValueValid(const Json_Value* const root);
GI_Return gi_Team_Load(gi_Team* const pThis, const Json_Value* const root);
void gi_Team_Print(const gi_Team* const pThis, FILE* const pFile);
void gi_Team_ComputeSpecialTeams(gi_Team* const pThis);
void gi_Team_PrintBestSpecialTeams(const gi_Team* const pThis, FILE* const pFile);

#endif /* #ifndef GI_TEAM_HH */
