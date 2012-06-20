#ifndef GI_PLAYINFO_HH
#define GI_PLAYINFO_HH

#include "gi.h"

void gi_PlayInfo_Init(gi_PlayInfo* const pThis);

GI_RETURN gi_PlayInfo_AddOffencePlay(gi_PlayInfo* const pThis, gi_OffencePlay* const pOffencePlay);
void gi_PlayInfo_ComputeOffenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

GI_RETURN gi_PlayInfo_AddDefencePlay(gi_PlayInfo* const pThis, gi_DefencePlay* const pDefencePlay);
void gi_PlayInfo_ComputeDefenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

void gi_PlayInfo_ComputeSpecialTeams(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

const gi_OffencePlay* gi_PlayInfo_GetOffencePlay(const gi_PlayInfo* const pThis, const size_t playIndex);
float gi_PlayInfo_GetOffenceStatsBaseValue(const gi_PlayInfo* const pThis, const size_t playIndex, const size_t playerIndex);

const gi_DefencePlay* gi_PlayInfo_GetDefencePlay(const gi_PlayInfo* const pThis, const size_t playIndex);
float gi_PlayInfo_GetDefenceStatsBaseValue(const gi_PlayInfo* const pThis, const size_t playIndex, const size_t playerIndex);

const gi_SpecialTeamsValues* gi_PlayInfo_GetSpecialTeamsValues(const gi_PlayInfo* const pThis);

#endif /* #ifndef GI_PLAYINFO_HH */
