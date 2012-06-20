#ifndef GI_DEFENCEPLAYPRIVATE_HH
#define GI_DEFENCEPLAYPRIVATE_HH

#include "gi_PositionValue.h"

struct gi_DefencePlay
{
	char m_name[GI_DEFENCENAME_MAX_SIZE];
	char m_zone[GI_ZONENAME_MAX_SIZE];		
	gi_PositionValue m_base[GI_POSITION_NUM];
	gi_PositionValue m_tackler[GI_POSITION_NUM];
	gi_PositionValue m_weighting[GI_POSITION_NUM];
	size_t m_numBase;
	size_t m_numTackler;
	size_t m_numWeighting;
};

#endif /*#ifndef GI_DEFENCEPLAYPRIVATE_HH*/
