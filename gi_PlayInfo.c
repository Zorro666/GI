#include "gi_PlayInfo.h"

void gi_PlayInfo_Init(gi_PlayInfo* const pThis)
{
	int i;
	for (i = 0; i < MAX_NUM_OFFENCE_PLAYS; i++)
	{
		gi_OffencePlay_Init(&pThis->m_offencePlays[i]);
	}
	pThis->m_numOffencePlays = 0;
	pThis->m_numDefencePlays = 0;
}

