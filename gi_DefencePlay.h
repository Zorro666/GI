#ifndef GI_DEFENCEPLAY_HH
#define GI_DEFENCEPLAY_HH

#include <stdio.h>
#include "gi.h"

void gi_DefencePlay_Init(gi_DefencePlay* const pThis);
GI_BOOL gi_DefencePlay_IsValueValid(const Json_Value* const root);
GI_RETURN gi_DefencePlay_Load(gi_DefencePlay* const pThis, const Json_Value* const root);
void gi_DefencePlay_Print(const gi_DefencePlay* const pThis, FILE* const pFile);
float gi_DefencePlay_ComputeBase(const gi_DefencePlay* const pThis, const GI_POSITION playerPosition, const float* const playerQST);

#endif /* #ifndef GI_DEFENCEPLAY_HH */
