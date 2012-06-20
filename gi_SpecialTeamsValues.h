#ifndef GI_SPECIALTEAMSVALUES_HH
#define GI_SPECIALTEAMSVALUES_HH

#include <stdio.h>
#include "gi.h"

void gi_SpecialTeamsValues_Init(gi_SpecialTeamsValues* const pThis);
void gi_SpecialTeamsValues_Compute(gi_SpecialTeamsValues* const pThis, const gi_Player* const pPlayer);

float gi_SpecialTeamsValues_GetProtector(const gi_SpecialTeamsValues* const pThis);
float gi_SpecialTeamsValues_GetBlocker(const gi_SpecialTeamsValues* const pThis);
float gi_SpecialTeamsValues_GetRunner(const gi_SpecialTeamsValues* const pThis);
float gi_SpecialTeamsValues_GetGunner(const gi_SpecialTeamsValues* const pThis);

#endif /* #ifndef GI_SPECIALTEAMSVALUESPRIVATE_HH */
