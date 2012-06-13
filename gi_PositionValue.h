#ifndef GI_POSITIONVALUE_HH
#define GI_POSITIONVALUE_HH

#include <stdio.h>
#include "gi.h"

struct gi_PositionValue
{
	char m_positionName[GI_POSITIONNAME_MAX_SIZE];
	GI_POSITION m_position;
	GI_POSITION m_basePosition;
	union PositionValue_Value 
	{
		float f;
		size_t i;
	} m_value;
	GI_QST m_qst;
	GI_Type m_valueType;
};

void gi_PositionValue_Init(gi_PositionValue* const pThis);
void gi_PositionValue_Print(const gi_PositionValue* const pThis, FILE* const pFile);
size_t gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const size_t maxSize, Json_Value* const root, const GI_Type dataType);

#endif /* #ifndef GI_POSITIONVALUE_HH */
