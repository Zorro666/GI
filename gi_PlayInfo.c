#include "gi_PlayInfo.h"

void gi_PlayInfo_Init(gi_PlayInfo* const pThis)
{
	size_t i;
	for (i = 0; i < GI_MAX_NUM_OFFENCE_PLAYS; i++)
	{
		gi_OffencePlay_Init(&pThis->m_offencePlays[i]);
	}
	pThis->m_numOffencePlays = 0;
	pThis->m_numDefencePlays = 0;
}

