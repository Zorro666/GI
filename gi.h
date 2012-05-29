#ifndef GI_HH
#define GI_HH

#include "json.h"

typedef enum GI_Type
{
	GI_FLOAT,
	GI_INT
} GI_Type;

#define MAX_OFFENCENAME_SIZE (16)
#define MAX_DEFENCENAME_SIZE (16)

#define MAX_NUM_DEFENCE_FORMATIONS (32)

#define MAX_NUM_OFFENCE_POSITIONS (32)

typedef struct PositionValue PositionValue;
typedef struct OffencePlay OffencePlay;

Json_Value* FindOffencePlay(Json_Value* const value);

#endif /* #ifndef GI_HH */
