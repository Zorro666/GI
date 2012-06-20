#ifndef GI_OFFENCEPLAY_HH
#define GI_OFFENCEPLAY_HH

#include <stdio.h>
#include "gi.h"
#include "gi_PositionValue.h"

void gi_OffencePlay_Init(gi_OffencePlay* const pThis);
GI_BOOL gi_OffencePlay_IsValueValid(const Json_Value* const root);
GI_RETURN gi_OffencePlay_Load(gi_OffencePlay* const pThis, const Json_Value* const root);
void gi_OffencePlay_Print(const gi_OffencePlay* const pThis, FILE* const pFile);
float gi_OffencePlay_ComputeBase(const gi_OffencePlay* const pThis, const GI_POSITION playerPosition, const float* const playerQST);

#endif /* #ifndef GI_OFFENCEPLAY_HH */
