#ifndef GI_HH
#define GI_HH

#include "Json.h"

typedef enum GI_Type
{
	GI_FLOAT,
	GI_INT
} GI_Type;

typedef enum GI_Return
{
	GI_ERROR,
	GI_SUCCESS
} GI_Return;

#define MAX_OFFENCENAME_SIZE (16)
#define MAX_DEFENCENAME_SIZE (16)

#define MAX_NUM_DEFENCE_FORMATIONS (32)

#define MAX_NUM_OFFENCE_POSITIONS (32)

typedef struct PositionValue PositionValue;
typedef struct OffencePlay OffencePlay;

void gi_Init(void);
Json_Value* gi_ParseFile(const char* const fileName, const int debug);
GI_Return gi_LoadFile(const char* const fileName, const int debug);
Json_Value* gi_FindOffencePlay(Json_Value* const value);

#endif /* #ifndef GI_HH */
