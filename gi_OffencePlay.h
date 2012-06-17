#ifndef GI_OFFENCEPLAY_HH
#define GI_OFFENCEPLAY_HH

#include <stdio.h>
#include "gi.h"
#include "gi_PositionValue.h"

struct gi_OffencePlay
{
	char m_name[GI_OFFENCENAME_MAX_SIZE];
	char m_defense[GI_DEFENCE_FORMATIONS_MAX_SIZE][GI_DEFENCENAME_MAX_SIZE];
	gi_PositionValue m_base[GI_POSITION_NUM];
	gi_PositionValue m_bc[GI_POSITION_NUM];
	gi_PositionValue m_weighting[GI_POSITION_NUM];
	size_t m_numBase;
	size_t m_numBC;
	size_t m_numWeighting;
};

void gi_OffencePlay_Init(gi_OffencePlay* const pThis);
GI_BOOL gi_OffencePlay_IsValueValid(const Json_Value* const root);
GI_RETURN gi_OffencePlay_Load(gi_OffencePlay* const pThis, const Json_Value* const root);
void gi_OffencePlay_Print(const gi_OffencePlay* const pThis, FILE* const pFile);
float gi_OffencePlay_ComputeBase(const gi_OffencePlay* const pThis, const GI_POSITION playerPosition, const float* const playerQST);

#endif /* #ifndef GI_OFFENCEPLAY_HH */
