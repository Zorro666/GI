#include "gi_PlayInfo.h"

void gi_PlayInfo_Init(gi_PlayInfo* const pThis)
{
	size_t i;
	for (i = 0; i < GI_OFFENCE_PLAYS_MAX_SIZE; i++)
	{
		gi_OffencePlay_Init(&pThis->m_offencePlays[i]);
	}
	pThis->m_numOffencePlays = 0;
	pThis->m_numDefencePlays = 0;
}

GI_Return gi_PlayInfo_AddOffencePlay(gi_PlayInfo* const pThis, gi_OffencePlay* const pOffencePlay)
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
		const float baseValue = gi_OffencePlay_ComputeBase(pOffencePlay, pPlayer);
		pThis->m_offenceStatsBase[i][playerIndex] = baseValue;
	}
}

