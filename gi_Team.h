#ifndef GI_TEAM_HH
#define GI_TEAM_HH

#include <stdio.h>
#include "gi.h"
#include "gi_Player.h"

void gi_Team_Init(gi_Team* const pThis);
GI_BOOL gi_Team_IsValueValid(const Json_Value* const root);
GI_RETURN gi_Team_Load(gi_Team* const pThis, const Json_Value* const root);
void gi_Team_Print(const gi_Team* const pThis, FILE* const pFile);
void gi_Team_PrintBestSpecialTeams(const gi_Team* const pThis, const gi_PlayInfo* const pPlayInfo, FILE* const pFile);

void gi_Team_ComputeSpecialTeams(const gi_Team* const pThis, gi_PlayInfo* const pPlayInfo);
void gi_Team_ComputeOffenceBase(const gi_Team* const pThis, gi_PlayInfo* const pPlayInfo);
void gi_Team_ComputeDefenceBase(const gi_Team* const pThis, gi_PlayInfo* const pPlayInfo);

const char* gi_Team_GetName(const gi_Team* const pThis);
GI_BOOL gi_Team_IsPlayerUsedByName(const gi_Team* const pThis, const char* const playerName);
GI_BOOL gi_Team_IsPlayerUsedByIndex(const gi_Team* const pThis, size_t playerIndex);

#endif /* #ifndef GI_TEAM_HH */
