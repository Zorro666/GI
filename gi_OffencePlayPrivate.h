#ifndef GI_OFFENCEPLAYPRIVATE_HH
#define GI_OFFENCEPLAYPRIVATE_HH

#include "gi_PositionValue.h"

struct gi_OffencePlay
{
	char m_name[GI_OFFENCENAME_MAX_SIZE];
	char m_defense[GI_DEFENCE_FORMATIONS_MAX_SIZE][GI_DEFENCENAME_MAX_SIZE];
	gi_PositionValue m_base[GI_POSITION_NUM];
	gi_PositionValue m_bc[GI_POSITION_NUM];
	gi_PositionValue m_weighting[GI_POSITION_NUM];
	size_t m_numBase;
	size_t m_numBC;
	size_t m_numWeighting;
};

#endif /*#ifndef GI_OFFENCEPLAYPRIVATE_HH*/
