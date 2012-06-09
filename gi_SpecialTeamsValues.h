#ifndef GI_SPECIALTEAMSVALUES_HH
#define GI_SPECIALTEAMSVALUES_HH

#include <stdio.h>
#include "gi.h"

struct gi_SpecialTeamsValues
{
	float m_protector;
	float m_blocker;
	float m_runner;
	float m_gunner;
};

void gi_SpecialTeamsValues_Init(gi_SpecialTeamsValues* const pThis);
void gi_SpecialTeamsValues_Compute(gi_SpecialTeamsValues* const pThis, const gi_Player* const pPlayer);

#endif /* #ifndef GI_SPECIALTEAMSVALUES_HH */
