#include "gi_GlobalInfo.h"

void gi_GlobalInfo_Init(gi_GlobalInfo* const pThis)
{
	gi_PlayInfo_Init(&pThis->m_playInfo);
	gi_Team_Init(&pThis->m_team);
}

