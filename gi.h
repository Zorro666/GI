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

typedef enum GI_Bool
{
	GI_TRUE,
	GI_FALSE
} GI_Bool;

typedef enum GI_SQUAD_UNIT
{
	GI_OFFENCE,
	GI_DEFENCE,
	GI_SPECIALTEAMS,
	GI_SQUAD_UNKNOWN
} GI_SQUAD_UNIT;

typedef enum GI_POSITION
{
	GI_QB = 0,
	GI_RB = 1,
	GI_WR = 2,
	GI_TE = 3,
	GI_OC = 4,
	GI_OG = 5,
	GI_OT = 6,
	GI_DE = 7,
	GI_DT = 8,
	GI_IB = 9,
	GI_OB = 10,
	GI_CB = 11,
	GI_SF = 12,
	GI_R = 13,
	GI_K = 14,
	GI_P = 15,
	GI_POSITION_UNKNOWN = 16,
	GI_NUM_POSITIONS = 17
} GI_POSITION;

#define GI_MAX_TEAMNAME_SIZE (128)
#define GI_MAX_PLAYERNAME_SIZE (128)
#define GI_MAX_OFFENCENAME_SIZE (16)
#define GI_MAX_DEFENCENAME_SIZE (16)
#define GI_MAX_POSITION_SIZE (8)
#define GI_MAX_OUTPUTFILENAME_SIZE (256)

#define GI_MAX_NUM_OFFENCE_FORMATIONS (32)
#define GI_MAX_NUM_DEFENCE_FORMATIONS (32)

#define GI_MAX_NUM_OFFENCE_POSITIONS (32)
#define GI_MAX_NUM_DEFENCE_POSITIONS (32)

#define GI_MAX_NUM_OFFENCE_PLAYS (128)
#define GI_MAX_NUM_DEFENCE_PLAYS (128)

#define GI_MAX_NUM_OFFENCE_PLAYERS (64)
#define GI_MAX_NUM_DEFENCE_PLAYERS (64)
#define GI_MAX_NUM_SPECIALTEAMS_PLAYERS (16)
#define GI_MAX_NUM_SQUAD_PLAYERS (64)

typedef struct gi_PositionValue gi_PositionValue;
typedef struct gi_OffencePlay gi_OffencePlay;
typedef struct gi_DefencePlay gi_DefencePlay;
typedef struct gi_Player gi_Player;
typedef struct gi_Team gi_Team;
typedef struct gi_PlayInfo gi_PlayInfo;
typedef struct gi_GlobalInfo gi_GlobalInfo;
typedef struct gi_SpecialTeamsValues gi_SpecialTeamsValues;

void gi_Init(void);
Json_Value* gi_ParseFile(const char* const fileName, const int debug);
GI_Return gi_LoadFile(const char* const fileName, const int debug);
const char* gi_GetPositionName(const GI_POSITION position);
void gi_Output(void);

#endif /* #ifndef GI_HH */
