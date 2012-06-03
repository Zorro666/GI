#ifndef GI_PLAYINFO_HH
#define GI_PLAYINFO_HH

#include "gi.h"
#include "gi_OffencePlay.h"

struct gi_PlayInfo
{
	gi_OffencePlay m_offencePlays[GI_MAX_NUM_OFFENCE_PLAYS];
	/*gi_DefencePlay m_defencePlays[GI_MAX_NUM_OFFENCE_PLAYS];*/
	int m_numOffencePlays;
	int m_numDefencePlays;
};

void gi_PlayInfo_Init(gi_PlayInfo* const pThis);

#endif /* #ifndef GI_PLAYINFO_HH */
