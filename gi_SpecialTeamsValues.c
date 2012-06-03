#include "gi_SpecialTeamsValues.h"

void gi_SpecialTeamsValues_Init(gi_SpecialTeamsValues* const pThis)
{
	gi_SpecialTeamsValues_Set(pThis, 0.0f, 0.0f, 0.0f, 0.0f);
}

void gi_SpecialTeamsValues_Set(gi_SpecialTeamsValues* const pThis, const float blocker, const float gunner, 
															 const float protector, const float runner)
{
	pThis->m_blocker = blocker;
	pThis->m_gunner = gunner;
	pThis->m_protector = protector;
	pThis->m_runner = runner;
}

