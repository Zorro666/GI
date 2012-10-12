#ifndef GI_HH
#define GI_HH

#include "Json.h"

typedef enum GI_TYPE
{
	GI_TYPE_FLOAT,
	GI_TYPE_INT,
	GI_TYPE_UNKNOWN
} GI_TYPE;

typedef enum GI_RETURN
{
	GI_RETURN_ERROR,
	GI_RETURN_SUCCESS
} GI_RETURN;

typedef enum GI_BOOL
{
	GI_TRUE,
	GI_FALSE
} GI_BOOL;

typedef enum GI_INJURY
{
	GI_INJURY_A = 0,
	GI_INJURY_B = 1,
	GI_INJURY_C = 2,
	GI_INJURY_D = 3,
	GI_INJURY_E = 4,
	GI_INJURY_F = 5,
	GI_INJURY_G = 6,
	GI_INJURY_H = 7,
	GI_INJURY_UNKNOWN = 8,
	GI_INJURY_NUM = GI_INJURY_UNKNOWN+1
} GI_INJURY;

typedef enum GI_UNIT
{
	GI_UNIT_OFFENCE = 0,
	GI_UNIT_DEFENCE = 1,
	GI_UNIT_SPECIALTEAMS = 2,
	GI_UNIT_UNKNOWN = 3,
	GI_UNIT_NUM = GI_UNIT_UNKNOWN+1
} GI_UNIT;

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
	GI_POSITION_TE1 = 20,
	GI_POSITION_TE2 = 21,
	GI_POSITION_LG = 22,
	GI_POSITION_RG = 23,
	GI_POSITION_LT = 24,
	GI_POSITION_RT = 25,
	GI_POSITION_HB = 26,
	GI_POSITION_FB = 27,
	GI_POSITION_FS = 28,
	GI_POSITION_SS = 29,
	GI_POSITION_C = 30,
	GI_POSITION_NT = 31,
	GI_POSITION_LE = 32,
	GI_POSITION_RE = 33,
	GI_POSITION_LILB = 34,
	GI_POSITION_MILB = 35,
	GI_POSITION_RILB = 36,
	GI_POSITION_LLB = 37,
	GI_POSITION_RLB = 38,
	GI_POSITION_UNKNOWN = 39,
	GI_POSITION_NUM = GI_POSITION_UNKNOWN+1
} GI_POSITION;

typedef enum GI_QST
{
	GI_QST_Q = 0,
	GI_QST_S = 1,
	GI_QST_T = 2,
	GI_QST_NONE = 3,
	GI_QST_UNKNOWN = 4,
	GI_QST_NUM = GI_QST_UNKNOWN+1
} GI_QST;

#define GI_TEAMNAME_MAX_SIZE (128)
#define GI_PLAYERNAME_MAX_SIZE (128)
#define GI_ZONENAME_MAX_SIZE (16)
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

#define GI_FORMATION_NUM_PLAYERS (11)

typedef struct gi_PositionValue gi_PositionValue;
typedef struct gi_OffencePlay gi_OffencePlay;
typedef struct gi_DefencePlay gi_DefencePlay;
typedef struct gi_Player gi_Player;
typedef struct gi_Team gi_Team;
typedef struct gi_PlayInfo gi_PlayInfo;
typedef struct gi_GlobalInfo gi_GlobalInfo;
typedef struct gi_SpecialTeamsValues gi_SpecialTeamsValues;

void gi_Init(void);
Json_Value* gi_ParseFile(const char* const fileName, const size_t debug);
GI_RETURN gi_LoadFile(const char* const fileName, const size_t debug);
void gi_Compute(void);
void gi_Output(void);

const char* gi_GetPositionName(const GI_POSITION position);
const char* gi_GetUnitName(const GI_UNIT unit);
const char* gi_GetQSTName(const GI_QST qst);
const char* gi_GetInjuryName(const GI_INJURY injury);

GI_POSITION gi_GetPositionFromName(const char* const position);
GI_QST gi_GetQSTFromName(const char* const position);
GI_QST gi_GetBasePosition(const GI_POSITION position);
GI_INJURY gi_GetInjuryFromName(const char* const injury);

#endif /* #ifndef GI_HH */
