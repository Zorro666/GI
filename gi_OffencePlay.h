#ifndef OFFENCEPLAY_HH
#define OFFENCEPLAY_HH

#include "gi.h"
#include "gi_PositionValue.h"

struct gi_OffencePlay
{
	char m_name[MAX_OFFENCENAME_SIZE];
	char m_defense[MAX_NUM_DEFENCE_FORMATIONS][MAX_DEFENCENAME_SIZE];
	gi_PositionValue m_base[MAX_NUM_OFFENCE_POSITIONS];
	gi_PositionValue m_bc[MAX_NUM_OFFENCE_POSITIONS];
	gi_PositionValue m_weighting[MAX_NUM_OFFENCE_POSITIONS];
};

void gi_OffencePlay_Init(gi_OffencePlay* const pThis);
GI_Bool gi_OffencePlay_IsValueValid(const Json_Value* const root);
GI_Return gi_OffencePlay_Load(gi_OffencePlay* const pThis, const Json_Value* const root);
void gi_OffencePlay_Print(gi_OffencePlay* const pThis);

#endif /* #ifndef OFFENCEPLAY_HH */
