#include <stdio.h>
#include <string.h>

#include "gi_PositionValue.h"
#include "gi_PositionValuePrivate.h"
#include "gi_Logger.h"

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

size_t gi_PositionValueArray_Parse(gi_PositionValue positionValue[], const size_t maxSize, const Json_Value* const root, const GI_TYPE dataType)
{
	const Json_Value* it;
	size_t i;

	i = 0;
	for (it = Json_Value_GetFirstChild(root); it != NULL; it = Json_Value_GetNextSibling(it))
	{
		if (Json_Value_GetType(it) == JSON_OBJECT)
		{
			const Json_Value* value = Json_Value_GetFirstChild(it);
			const Json_Type valueType = Json_Value_GetType(value);	
			const char* const valueName = Json_Value_GetName(value);	
			if (valueType == JSON_FLOAT)
			{
				if (dataType == GI_TYPE_INT)
				{
					GI_FATAL_ERROR("Found float data but expecting int data name:'%s'", valueName);
					return 0;
				}
				positionValue[i].m_value.f = Json_Value_GetFloatValue(value);
				positionValue[i].m_valueType = GI_TYPE_FLOAT;
			}
			else if (valueType == JSON_INT)
			{
				if (dataType == GI_TYPE_FLOAT)
				{
					GI_FATAL_ERROR("Found int data but expecting float data name:'%s'", valueName);
					return 0;
				}
				positionValue[i].m_value.i = (size_t)(Json_Value_GetIntValue(value));
				positionValue[i].m_valueType = GI_TYPE_INT;
			}
			if ((valueType == JSON_FLOAT) || (valueType == JSON_INT))
			{
				char szPositionnName[GI_POSITIONNAME_MAX_SIZE];
				char* pSepStart = NULL;

				strncpy(szPositionnName, valueName, GI_POSITIONNAME_MAX_SIZE);
				pSepStart = strchr(szPositionnName, '[');
				if (pSepStart != NULL)
				{
					char szQSTName[GI_POSITIONNAME_MAX_SIZE];
					char* const pSepEnd = strchr(pSepStart, ']');
					pSepStart[0] = '\0';
					pSepEnd[0] = '\0';
					strcpy(szQSTName, pSepStart+1);
					positionValue[i].m_qst = gi_GetQSTFromName(szQSTName);
					if (positionValue[i].m_qst == GI_QST_UNKNOWN)
					{
						GI_FATAL_ERROR("unknown QST '%s'", szQSTName);
						return 0;
					}
				}
				else
				{
					positionValue[i].m_qst = GI_QST_NONE;
				}

				strncpy(positionValue[i].m_positionName, valueName, GI_POSITIONNAME_MAX_SIZE);
				positionValue[i].m_position = gi_GetPositionFromName(szPositionnName);
				if (positionValue[i].m_position == GI_POSITION_UNKNOWN)
				{
					GI_FATAL_ERROR("unknown position '%s'", szPositionnName);
					return 0;
				}
				positionValue[i].m_basePosition = gi_GetBasePosition(positionValue[i].m_position);
				if (positionValue[i].m_basePosition == GI_POSITION_UNKNOWN)
				{
					GI_FATAL_ERROR("unknown basePosition '%s'", positionValue[i].m_position);
					return 0;
				}
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

const char* gi_PositionValue_GetPositionName(const gi_PositionValue* const pThis)
{
	return pThis->m_positionName;
}

GI_POSITION gi_PositionValue_GetPosition(const gi_PositionValue* const pThis)
{
	return pThis->m_position;
}

GI_POSITION gi_PositionValue_GetBasePosition(const gi_PositionValue* const pThis)
{
	return pThis->m_basePosition;
}

float gi_PositionValue_GetFloatValue(const gi_PositionValue* const pThis)
{
	return pThis->m_value.f;
}

size_t gi_PositionValue_GetSizetValue(const gi_PositionValue* const pThis)
{
	return pThis->m_value.i;
}

GI_QST gi_PositionValue_GetQST(const gi_PositionValue* const pThis)
{
	return pThis->m_qst;
}

GI_TYPE gi_PositionValue_GetValueType(const gi_PositionValue* const pThis)
{
	return pThis->m_valueType;
}

