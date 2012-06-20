#ifndef GI_PLAYINFOPRIVATE_HH
#define GI_PLAYINFOPRIVATE_HH

#include "gi_OffencePlayPrivate.h"
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

#endif /*#ifndef GI_PLAYINFOPRIVATE_HH*/
