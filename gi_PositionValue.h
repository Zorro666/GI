#ifndef POSITIONVALUE_HH
#define POSITIONVALUE_HH

#include "gi.h"

struct gi_PositionValue
{
	char m_position[MAX_POSITION_SIZE];
	union PositionValue_Value 
	{
		float f;
		int i;
	} m_value;
	GI_Type m_valueType;
};

void gi_PositionValue_Init(gi_PositionValue* const pThis);
void gi_PositionValue_Print(gi_PositionValue* const pThis);
void gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const int maxSize, Json_Value* const root);

#endif /* #ifndef POSITIONVALUE_HH */
