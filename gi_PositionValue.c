#include <stdio.h>
#include <string.h>

#include "gi_PositionValue.h"

void gi_PositionValue_Init(gi_PositionValue* const pThis)
{
	pThis->m_position[0] = '\0';
	pThis->m_value.f = 0.0f;
}

void gi_PositionValue_Print(gi_PositionValue* const pThis)
{
	if (pThis->m_position[0] != '\0')
	{
		if (pThis->m_valueType == GI_FLOAT)
		{
			printf("%s = %.2f ", pThis->m_position, pThis->m_value.f);
		}
		else if (pThis->m_valueType == GI_INT)
		{
			printf("%s = %d ", pThis->m_position, pThis->m_value.i);
		}
	}
}

void gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const int maxSize, Json_Value* const root)
{
	Json_Value* it;
	int i;

	i = 0;
	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_OBJECT)
		{
			Json_Value* value = it->m_first_child;
			if (value->m_type == JSON_FLOAT)
			{
				strncpy(positionValue[i].m_position, value->m_name, MAX_POSITION_SIZE);
				positionValue[i].m_value.f = value->m_value_data.float_value;
				positionValue[i].m_valueType = GI_FLOAT;
				i++;
			}
			else if (value->m_type == JSON_INT)
			{
				strncpy(positionValue[i].m_position, value->m_name, MAX_POSITION_SIZE);
				positionValue[i].m_value.i = value->m_value_data.int_value;
				positionValue[i].m_valueType = GI_INT;
				i++;
			}
			if (i >= maxSize)
			{
				return;
			}
		}
	}
}

