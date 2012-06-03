#ifndef GI_OFFENCEPLAY_HH
#define GI_OFFENCEPLAY_HH

#include <stdio.h>
#include "gi.h"
#include "gi_PositionValue.h"

struct gi_OffencePlay
{
	char m_name[GI_MAX_OFFENCENAME_SIZE];
	char m_defense[GI_MAX_NUM_DEFENCE_FORMATIONS][GI_MAX_DEFENCENAME_SIZE];
	gi_PositionValue m_base[GI_MAX_NUM_OFFENCE_POSITIONS];
	gi_PositionValue m_bc[GI_MAX_NUM_OFFENCE_POSITIONS];
	gi_PositionValue m_weighting[GI_MAX_NUM_OFFENCE_POSITIONS];
};

void gi_OffencePlay_Init(gi_OffencePlay* const pThis);
GI_Bool gi_OffencePlay_IsValueValid(const Json_Value* const root);
GI_Return gi_OffencePlay_Load(gi_OffencePlay* const pThis, const Json_Value* const root);
void gi_OffencePlay_Print(gi_OffencePlay* const pThis, FILE* const pFile);

#endif /* #ifndef GI_OFFENCEPLAY_HH */
