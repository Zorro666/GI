#ifndef GI_GLOBALINFO_HH
#define GI_GLOBALINFO_HH

#include "gi.h"

void gi_GlobalInfo_Init(gi_GlobalInfo* const pThis);
GI_RETURN gi_GlobalInfo_AddTeam(gi_GlobalInfo* const pThis, gi_Team* const pTeam);
GI_RETURN gi_GlobalInfo_AddOffencePlay(gi_GlobalInfo* const pThis, gi_OffencePlay* const pOffencePlay);
GI_RETURN gi_GlobalInfo_AddDefencePlay(gi_GlobalInfo* const pThis, gi_DefencePlay* const pDefencePlay);
void gi_GlobalInfo_Compute(gi_GlobalInfo* const pThis);

const gi_Team* gi_GlobalInfo_GetTeam(const gi_GlobalInfo* const pThis);
const gi_PlayInfo* gi_GlobalInfo_GetPlayInfo(const gi_GlobalInfo* const pThis);

#endif /* #ifndef GI_GLOBALINFO_HH */
