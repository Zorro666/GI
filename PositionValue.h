#ifndef POSITIONVALUE_HH
#define POSITIONVALUE_HH

#define MAX_POSITION_SIZE (8)

#include "gi.h"

struct PositionValue
{
	char m_position[MAX_POSITION_SIZE];
	union PositionValue_Value 
	{
		float f;
		int i;
	} m_value;
	GI_Type m_valueType;
};

void PositionValue_Init(PositionValue* const pThis);
void PositionValue_Print(PositionValue* const pThis);
void PositionValueArray_Parse(PositionValue positionValue[], const int maxSize, Json_Value* const root);

#endif /* #ifndef POSITIONVALUE_HH */
