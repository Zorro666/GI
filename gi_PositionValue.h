#ifndef GI_POSITIONVALUE_HH
#define GI_POSITIONVALUE_HH

#include <stdio.h>
#include "gi.h"

void gi_PositionValue_Init(gi_PositionValue* const pThis);
void gi_PositionValue_Print(const gi_PositionValue* const pThis, FILE* const pFile);
size_t gi_PositionValueArray_Parse(gi_PositionValue* const positionValue, const size_t maxSize, const Json_Value* const root, const GI_TYPE dataType);

const char* gi_PositionValue_GetPositionName(const gi_PositionValue* const pThis);
GI_POSITION gi_PositionValue_GetPosition(const gi_PositionValue* const pThis);
GI_POSITION gi_PositionValue_GetBasePosition(const gi_PositionValue* const pThis);
float gi_PositionValue_GetFloatValue(const gi_PositionValue* const pThis);
size_t gi_PositionValue_GetSizetValue(const gi_PositionValue* const pThis);
GI_QST gi_PositionValue_GetQST(const gi_PositionValue* const pThis);
GI_TYPE gi_PositionValue_GetValueType(const gi_PositionValue* const pThis);

#endif /* #ifndef GI_POSITIONVALUE_HH */
