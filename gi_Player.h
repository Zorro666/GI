#ifndef GI_PLAYER_HH
#define GI_PLAYER_HH

#include <stdio.h>
#include "gi.h"

void gi_Player_Init(gi_Player* const pThis);
GI_BOOL gi_Player_IsValueValid(const Json_Value* const root);
GI_RETURN gi_Player_Load(gi_Player* const pThis, const Json_Value* const root);
void gi_Player_Print(const gi_Player* const pThis, FILE* const pFile);
void gi_Player_ComputeSpecialTeams(const gi_Player* const pThis, gi_SpecialTeamsValues* const pSpecialTeamsValues);

const char* gi_Player_GetName(const gi_Player* const pThis);
GI_POSITION gi_Player_GetPosition(const gi_Player* const pThis);
size_t gi_Player_GetRawLevel(const gi_Player* const pThis);
const size_t* gi_Player_GetRawQST(const gi_Player* const pThis);
float gi_Player_GetExperience(const gi_Player* const pThis);
GI_UNIT gi_Player_GetUnit(const gi_Player* const pThis);
size_t gi_Player_GetAge(const gi_Player* const pThis);
float gi_Player_GetLevel(const gi_Player* const pThis);
const float* gi_Player_GetQST(const gi_Player* const pThis);
GI_INJURY gi_Player_GetInjury(const gi_Player* const pThis);

const char* gi_Player_GetPositionName(const gi_Player* const pThis);

void gi_Player_SetInjury(gi_Player* const pThis, const GI_INJURY injury);

#endif /* #ifndef GI_PLAYER_HH */
