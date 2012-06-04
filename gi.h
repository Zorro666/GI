#ifndef GI_HH
#define GI_HH

#include "Json.h"

typedef enum GI_Type
{
	GI_TYPE_FLOAT,
	GI_TYPE_INT,
	GI_TYPE_UNKNOWN
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

typedef enum GI_SQUAD
{
	GI_SQUAD_OFFENCE,
	GI_SQUAD_DEFENCE,
	GI_SQUAD_SPECIALTEAMS,
	GI_SQUAD_UNKNOWN
} GI_SQUAD;

typedef enum GI_POSITION
{
	GI_POSITION_QB = 0,
	GI_POSITION_RB = 1,
	GI_POSITION_WR = 2,
	GI_POSITION_TE = 3,
	GI_POSITION_OC = 4,
	GI_POSITION_OG = 5,
	GI_POSITION_OT = 6,
	GI_POSITION_DE = 7,
	GI_POSITION_DT = 8,
	GI_POSITION_IB = 9,
	GI_POSITION_OB = 10,
	GI_POSITION_CB = 11,
	GI_POSITION_SF = 12,
	GI_POSITION_R = 13,
	GI_POSITION_K = 14,
	GI_POSITION_P = 15,
	GI_POSITION_SE = 16,
	GI_POSITION_FL = 17,
	GI_POSITION_RB1 = 18,
	GI_POSITION_RB2 = 19,
	GI_POSITION_HB = 20,
	GI_POSITION_FB = 21,
	GI_POSITION_FS = 22,
	GI_POSITION_SS = 23,
	GI_POSITION_UNKNOWN = 24,
	GI_POSITION_NUM = GI_POSITION_UNKNOWN+1
} GI_POSITION;

typedef enum GI_QST
{
	GI_QST_Q = 0,
	GI_QST_S = 1,
	GI_QST_T = 2,
	GI_QST_UNKNOWN = 3,
	GI_QST_NUM = GI_QST_UNKNOWN+1
} GI_QST;

#define GI_TEAMNAME_MAX_SIZE (128)
#define GI_PLAYERNAME_MAX_SIZE (128)
#define GI_OFFENCENAME_MAX_SIZE (16)
#define GI_DEFENCENAME_MAX_SIZE (16)
#define GI_POSITIONNAME_MAX_SIZE (8)
#define GI_OUTPUTFILENAME_MAX_SIZE (256)

#define GI_OFFENCE_FORMATIONS_MAX_SIZE (32)
#define GI_DEFENCE_FORMATIONS_MAX_SIZE (32)

#define GI_OFFENCE_PLAYS_MAX_SIZE (128)
#define GI_DEFENCE_PLAYS_MAX_SIZE (128)

#define GI_OFFENCE_PLAYERS_MAX_SIZE (64)
#define GI_DEFENCE_PLAYERS_MAX_SIZE (64)
#define GI_SPECIALTEAMS_PLAYERS_MAX_SIZE (16)
#define GI_SQUAD_PLAYERS_MAX_SIZE (64)

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
const char* gi_GetSquadName(const GI_SQUAD unit);
GI_POSITION gi_GetPositionFromName(const char* const position);
void gi_Output(void);

#endif /* #ifndef GI_HH */
