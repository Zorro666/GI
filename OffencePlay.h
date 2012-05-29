#ifndef OFFENCEPLAY_HH
#define OFFENCEPLAY_HH

#include "gi.h"
#include "PositionValue.h"

struct OffencePlay
{
	char m_name[MAX_OFFENCENAME_SIZE];
	char m_defense[MAX_NUM_DEFENCE_FORMATIONS][MAX_DEFENCENAME_SIZE];
	PositionValue m_base[MAX_NUM_OFFENCE_POSITIONS];
	PositionValue m_bc[MAX_NUM_OFFENCE_POSITIONS];
	PositionValue m_weighting[MAX_NUM_OFFENCE_POSITIONS];
};

void OffencePlay_Init(OffencePlay* const pThis);
GI_Return OffencePlay_Load(OffencePlay* const pThis, const Json_Value* const playRoot);
void OffencePlay_Print(OffencePlay* const pThis);

#endif /* #ifndef OFFENCEPLAY_HH */
