#ifndef GI_POSITIONVALUE_HH
#define GI_POSITIONVALUE_HH

#include <stdio.h>
#include "gi.h"

struct gi_PositionValue
{
	char m_positionName[GI_POSITIONNAME_MAX_SIZE];
	GI_POSITION m_position;
	GI_QST m_stat;
	union PositionValue_Value 
	{
		float f;
		int i;
	} m_value;
	GI_Type m_valueType;
};

void gi_PositionValue_Init(gi_PositionValue* const pThis);
void gi_PositionValue_Print(const gi_PositionValue* const pThis, FILE* const pFile);
void gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const size_t maxSize, Json_Value* const root);

#endif /* #ifndef GI_POSITIONVALUE_HH */
