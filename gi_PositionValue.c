#include <stdio.h>
#include <string.h>

#include "gi_PositionValue.h"

void gi_PositionValue_Init(gi_PositionValue* const pThis)
{
	pThis->m_positionName[0] = '\0';
	pThis->m_position = GI_POSITION_UNKNOWN;
	pThis->m_basePosition = GI_POSITION_UNKNOWN;
	pThis->m_value.f = 0.0f;
	pThis->m_valueType = GI_TYPE_UNKNOWN;
}

void gi_PositionValue_Print(const gi_PositionValue* const pThis, FILE* const pFile)
{
	if (pThis->m_positionName[0] != '\0')
	{
		if ((pThis->m_valueType == GI_TYPE_FLOAT) || (pThis->m_valueType == GI_TYPE_INT))
		{
			fprintf(pFile, "'%s' %s %s", pThis->m_positionName, 
								gi_GetPositionName(pThis->m_basePosition), gi_GetPositionName(pThis->m_position));
			if (pThis->m_qst != GI_QST_NONE)
			{
				fprintf(pFile, ".%s", gi_GetQSTName(pThis->m_qst));
			}
			fprintf(pFile, " = ");
		}
		if (pThis->m_valueType == GI_TYPE_FLOAT)
		{
			fprintf(pFile, "%.2f ", pThis->m_value.f);
		}
		else if (pThis->m_valueType == GI_TYPE_INT)
		{
			fprintf(pFile, "%d ", pThis->m_value.i);
		}
	}
}

size_t gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const size_t maxSize, Json_Value* const root)
{
	Json_Value* it;
	size_t i;

	i = 0;
	for (it = root->m_first_child; it != NULL; it = it->m_next_sibling)
	{
		if (it->m_type == JSON_OBJECT)
		{
			Json_Value* value = it->m_first_child;
			if (value->m_type == JSON_FLOAT)
			{
				positionValue[i].m_value.f = value->m_value_data.float_value;
				positionValue[i].m_valueType = GI_TYPE_FLOAT;
			}
			else if (value->m_type == JSON_INT)
			{
				positionValue[i].m_value.i = (size_t)(value->m_value_data.int_value);
				positionValue[i].m_valueType = GI_TYPE_INT;
			}
			if ((value->m_type == JSON_FLOAT) || (value->m_type == JSON_INT))
			{
				char szPositionnName[GI_POSITIONNAME_MAX_SIZE];
				char* pSepStart = NULL;

				strncpy(szPositionnName, value->m_name, GI_POSITIONNAME_MAX_SIZE);
				pSepStart = strchr(szPositionnName, '[');
				if (pSepStart != NULL)
				{
					char szQSTName[GI_POSITIONNAME_MAX_SIZE];
					char* const pSepEnd = strchr(pSepStart, ']');
					pSepStart[0] = '\0';
					pSepEnd[0] = '\0';
					strcpy(szQSTName, pSepStart+1);
					positionValue[i].m_qst = gi_GetQSTFromName(szQSTName);
				}
				else
				{
					positionValue[i].m_qst = GI_QST_NONE;
				}

				strncpy(positionValue[i].m_positionName, value->m_name, GI_POSITIONNAME_MAX_SIZE);
				positionValue[i].m_position = gi_GetPositionFromName(szPositionnName);
				positionValue[i].m_basePosition = gi_GetBasePosition(positionValue[i].m_position);
				i++;
			}
			if (i >= maxSize)
			{
				return maxSize;
			}
		}
	}
	return i;
}

