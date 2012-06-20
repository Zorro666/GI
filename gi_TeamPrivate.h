#ifndef GI_TEAMPRIVATE_HH
#define GI_TEAMPRIVATE_HH

#include "gi_PlayerPrivate.h"

struct gi_Team
{
	gi_Player m_squad[GI_SQUAD_PLAYERS_MAX_SIZE];
	gi_Player* m_offence[GI_OFFENCE_PLAYERS_MAX_SIZE];
	gi_Player* m_defence[GI_DEFENCE_PLAYERS_MAX_SIZE];
	gi_Player* m_specialTeams[GI_SPECIALTEAMS_PLAYERS_MAX_SIZE];
	size_t m_usedPlayers[GI_UNIT_NUM][GI_SQUAD_PLAYERS_MAX_SIZE];
	size_t m_positionStarts[GI_POSITION_NUM];
	size_t m_positionCounts[GI_POSITION_NUM];
	char m_name[GI_TEAMNAME_MAX_SIZE];
	size_t m_numPlayers;
	size_t m_numOffence;
	size_t m_numDefence;
	size_t m_numSpecialTeams;
};

#endif /*#ifndef GI_TEAMPRIVATE_HH*/

