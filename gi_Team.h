#ifndef GI_TEAM_HH
#define GI_TEAM_HH

#include <stdio.h>
#include "gi.h"
#include "gi_Player.h"

struct gi_Team
{
	gi_Player m_squad[MAX_NUM_SQUAD_PLAYERS];
	gi_Player* m_offence[MAX_NUM_OFFENCE_PLAYERS];
	gi_Player* m_defence[MAX_NUM_DEFENCE_PLAYERS];
	gi_Player* m_specialTeams[MAX_NUM_SPECIALTEAMS_PLAYERS];
	char m_name[MAX_TEAMNAME_SIZE];
	size_t m_numPlayers;
	size_t m_numOffence;
	size_t m_numDefence;
	size_t m_numSpecialTeams;
};

void gi_Team_Init(gi_Team* const pThis);
GI_Bool gi_Team_IsValueValid(const Json_Value* const root);
GI_Return gi_Team_Load(gi_Team* const pThis, const Json_Value* const root);
void gi_Team_Print(gi_Team* const pThis, FILE* const pFile);
void gi_Team_ComputeSpecialTeams(gi_Team* const pThis);
void gi_Team_PrintBestSpecialTeams(gi_Team* const pThis, FILE* const pFile);

#endif /* #ifndef GI_TEAM_HH */
