#ifndef GI_GLOBALINFO_HH
#define GI_GLOBALINFO_HH

#include "gi.h"
#include "gi_PlayInfo.h"
#include "gi_Team.h"

struct gi_GlobalInfo
{
	gi_PlayInfo m_playInfo;
	gi_Team m_team;
};

void gi_GlobalInfo_Init(gi_GlobalInfo* const pThis);
GI_Return gi_globalInfo_AddTeam(gi_GlobalInfo* const pThis, gi_Team* const pTeam);
GI_Return gi_globalInfo_AddOffencePlay(gi_GlobalInfo* const pThis, gi_OffencePlay* const pOffencePlay);

#endif /* #ifndef GI_GLOBALINFO_HH */
