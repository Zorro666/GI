#ifndef GI_PLAYINFO_HH
#define GI_PLAYINFO_HH

#include "gi.h"
#include "gi_OffencePlay.h"
#include "gi_DefencePlay.h"
#include "gi_SpecialTeamsValues.h"

struct gi_PlayInfo
{
	size_t m_numOffencePlays;
	gi_OffencePlay m_offencePlays[GI_OFFENCE_PLAYS_MAX_SIZE];
	float m_offenceStatsBase[GI_OFFENCE_PLAYS_MAX_SIZE][GI_SQUAD_PLAYERS_MAX_SIZE];
	float m_offenceStatsBC[GI_OFFENCE_PLAYS_MAX_SIZE][GI_SQUAD_PLAYERS_MAX_SIZE];

	size_t m_numDefencePlays;
	gi_DefencePlay m_defencePlays[GI_DEFENCE_PLAYS_MAX_SIZE];
	float m_defenceStatsBase[GI_DEFENCE_PLAYS_MAX_SIZE][GI_SQUAD_PLAYERS_MAX_SIZE];
	float m_defenceStatsTackler[GI_DEFENCE_PLAYS_MAX_SIZE][GI_SQUAD_PLAYERS_MAX_SIZE];

	gi_SpecialTeamsValues m_specialTeamsValues[GI_SQUAD_PLAYERS_MAX_SIZE];
};

void gi_PlayInfo_Init(gi_PlayInfo* const pThis);

GI_Return gi_PlayInfo_AddOffencePlay(gi_PlayInfo* const pThis, gi_OffencePlay* const pOffencePlay);
void gi_PlayInfo_ComputeOffenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

GI_Return gi_PlayInfo_AddDefencePlay(gi_PlayInfo* const pThis, gi_DefencePlay* const pDefencePlay);

void gi_PlayInfo_ComputeSpecialTeams(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

#endif /* #ifndef GI_PLAYINFO_HH */
