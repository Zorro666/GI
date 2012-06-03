#ifndef GI_POSITIONVALUE_HH
#define GI_POSITIONVALUE_HH

#include <stdio.h>
#include "gi.h"

struct gi_PositionValue
{
	char m_position[GI_MAX_POSITION_SIZE];
	union PositionValue_Value 
	{
		float f;
		int i;
	} m_value;
	GI_Type m_valueType;
};

void gi_PositionValue_Init(gi_PositionValue* const pThis);
void gi_PositionValue_Print(gi_PositionValue* const pThis, FILE* const pFile);
void gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const size_t maxSize, Json_Value* const root);

#endif /* #ifndef GI_POSITIONVALUE_HH */
