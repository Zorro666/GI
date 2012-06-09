#include "gi_GlobalInfo.h"
#include "gi_Logger.h"

void gi_GlobalInfo_Init(gi_GlobalInfo* const pThis)
{
	gi_PlayInfo_Init(&pThis->m_playInfo);
	gi_Team_Init(&pThis->m_team);
}

GI_Return gi_GlobalInfo_AddTeam(gi_GlobalInfo* const pThis, gi_Team* const pTeam)
{
	if (pThis->m_team.m_name[0] != '\0')
	{
		return GI_RETURN_ERROR;
	}
	pThis->m_team = *pTeam;
	return GI_RETURN_SUCCESS;
}

GI_Return gi_GlobalInfo_AddOffencePlay(gi_GlobalInfo* const pThis, gi_OffencePlay* const pOffencePlay)
{
	return gi_PlayInfo_AddOffencePlay(&pThis->m_playInfo, pOffencePlay);
}

GI_Return gi_GlobalInfo_AddDefencePlay(gi_GlobalInfo* const pThis, gi_DefencePlay* const pDefencePlay)
{
	return gi_PlayInfo_AddDefencePlay(&pThis->m_playInfo, pDefencePlay);
}

void gi_GlobalInfo_Compute(gi_GlobalInfo* const pThis)
{
	const gi_Team* const pTeam = &pThis->m_team;
	gi_PlayInfo* const pPlayInfo = &pThis->m_playInfo;
	gi_Team_ComputeSpecialTeams(pTeam, pPlayInfo);
	gi_Team_ComputeOffenceBase(pTeam, pPlayInfo);
}
