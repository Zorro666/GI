#include "gi_PlayInfo.h"
#include "gi_PlayInfoPrivate.h"

#include "gi_Player.h"
#include "gi_Logger.h"

void gi_PlayInfo_Init(gi_PlayInfo* const pThis)
{
	size_t i;

	pThis->m_numOffencePlays = 0;
	for (i = 0; i < GI_OFFENCE_PLAYS_MAX_SIZE; i++)
	{
		gi_OffencePlay_Init(&pThis->m_offencePlays[i]);
	}
	for (i = 0; i < GI_OFFENCE_PLAYS_MAX_SIZE; i++)
	{
		size_t p;
		for (p = 0; p < GI_SQUAD_PLAYERS_MAX_SIZE; p++)
		{
			pThis->m_offenceStatsBase[i][p] = 0.0f;
			pThis->m_offenceStatsBC[i][p] = 0.0f;
		}
	}

	pThis->m_numDefencePlays = 0;
	for (i = 0; i < GI_DEFENCE_PLAYS_MAX_SIZE; i++)
	{
		gi_DefencePlay_Init(&pThis->m_defencePlays[i]);
	}
	for (i = 0; i < GI_DEFENCE_PLAYS_MAX_SIZE; i++)
	{
		size_t p;
		for (p = 0; p < GI_SQUAD_PLAYERS_MAX_SIZE; p++)
		{
			pThis->m_defenceStatsBase[i][p] = 0.0f;
			pThis->m_defenceStatsTackler[i][p] = 0.0f;
		}
	}

	for (i = 0; i < GI_SQUAD_PLAYERS_MAX_SIZE; i++)
	{
		gi_SpecialTeamsValues_Init(&pThis->m_specialTeamsValues[i]);
	}
}

GI_RETURN gi_PlayInfo_AddOffencePlay(gi_PlayInfo* const pThis, gi_OffencePlay* const pOffencePlay)
{
	size_t index = pThis->m_numOffencePlays;
	if (pThis->m_numOffencePlays >= GI_OFFENCE_PLAYS_MAX_SIZE)
	{
		return GI_RETURN_ERROR;
	}

	pThis->m_offencePlays[index] = *pOffencePlay;
	pThis->m_numOffencePlays++;
	return GI_RETURN_SUCCESS;
}

GI_RETURN gi_PlayInfo_AddDefencePlay(gi_PlayInfo* const pThis, gi_DefencePlay* const pDefencePlay)
{
	size_t index = pThis->m_numDefencePlays;
	if (pThis->m_numDefencePlays >= GI_DEFENCE_PLAYS_MAX_SIZE)
	{
		return GI_RETURN_ERROR;
	}

	pThis->m_defencePlays[index] = *pDefencePlay;
	pThis->m_numDefencePlays++;
	return GI_RETURN_SUCCESS;
}

void gi_PlayInfo_ComputeOffenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex)
{
	size_t i;
	for (i = 0; i < GI_OFFENCE_PLAYS_MAX_SIZE; i++)
	{
		pThis->m_offenceStatsBase[i][playerIndex] = 0.0f;
		pThis->m_offenceStatsBC[i][playerIndex] = 0.0f;
	}
	for (i = 0; i < pThis->m_numOffencePlays; i++)
	{
		const gi_OffencePlay* const pOffencePlay = &pThis->m_offencePlays[i];
		const GI_POSITION playerPosition = gi_Player_GetPosition(pPlayer);
		const float* const playerQST = gi_Player_GetQST(pPlayer);
		const float baseValue = gi_OffencePlay_ComputeBase(pOffencePlay, playerPosition, playerQST);
		pThis->m_offenceStatsBase[i][playerIndex] = baseValue;
	}
}

void gi_PlayInfo_ComputeDefenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex)
{
	size_t i;
	for (i = 0; i < GI_DEFENCE_PLAYS_MAX_SIZE; i++)
	{
		pThis->m_defenceStatsBase[i][playerIndex] = 0.0f;
		pThis->m_defenceStatsTackler[i][playerIndex] = 0.0f;
	}
	for (i = 0; i < pThis->m_numDefencePlays; i++)
	{
		const gi_DefencePlay* const pDefencePlay = &pThis->m_defencePlays[i];
		const GI_POSITION playerPosition = gi_Player_GetPosition(pPlayer);
		const float* const playerQST = gi_Player_GetQST(pPlayer);
		const float baseValue = gi_DefencePlay_ComputeBase(pDefencePlay, playerPosition, playerQST);
		pThis->m_defenceStatsBase[i][playerIndex] = baseValue;
	}
}

void gi_PlayInfo_ComputeSpecialTeams(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex)
{
	gi_SpecialTeamsValues_Compute(&pThis->m_specialTeamsValues[playerIndex], pPlayer);
}

