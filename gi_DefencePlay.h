#ifndef GI_DEFENCEPLAY_HH
#define GI_DEFENCEPLAY_HH

#include <stdio.h>
#include "gi.h"
#include "gi_PositionValue.h"

struct gi_DefencePlay
{
	char m_name[GI_DEFENCENAME_MAX_SIZE];
	char m_zone[GI_ZONENAME_MAX_SIZE];		
	gi_PositionValue m_base[GI_POSITION_NUM];
	gi_PositionValue m_tackler[GI_POSITION_NUM];
	gi_PositionValue m_weighting[GI_POSITION_NUM];
	size_t m_numBase;
	size_t m_numTackler;
	size_t m_numWeighting;
};

void gi_DefencePlay_Init(gi_DefencePlay* const pThis);
GI_Bool gi_DefencePlay_IsValueValid(const Json_Value* const root);
GI_Return gi_DefencePlay_Load(gi_DefencePlay* const pThis, const Json_Value* const root);
void gi_DefencePlay_Print(const gi_DefencePlay* const pThis, FILE* const pFile);
float gi_DefencePlay_ComputeBase(const gi_DefencePlay* const pThis, const gi_Player* const pPlayer);

#endif /* #ifndef GI_DEFENCEPLAY_HH */
