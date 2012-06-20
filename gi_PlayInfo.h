#ifndef GI_PLAYINFO_HH
#define GI_PLAYINFO_HH

#include "gi.h"
#include "gi_OffencePlay.h"
#include "gi_DefencePlay.h"
#include "gi_SpecialTeamsValues.h"

void gi_PlayInfo_Init(gi_PlayInfo* const pThis);

GI_RETURN gi_PlayInfo_AddOffencePlay(gi_PlayInfo* const pThis, gi_OffencePlay* const pOffencePlay);
void gi_PlayInfo_ComputeOffenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

GI_RETURN gi_PlayInfo_AddDefencePlay(gi_PlayInfo* const pThis, gi_DefencePlay* const pDefencePlay);
void gi_PlayInfo_ComputeDefenceBase(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

void gi_PlayInfo_ComputeSpecialTeams(gi_PlayInfo* const pThis, const gi_Player* const pPlayer, const size_t playerIndex);

#endif /* #ifndef GI_PLAYINFO_HH */
