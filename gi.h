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

#define MAX_TEAMNAME_SIZE (128)
#define MAX_PLAYERNAME_SIZE (128)
#define MAX_OFFENCENAME_SIZE (16)
#define MAX_DEFENCENAME_SIZE (16)
#define MAX_POSITION_SIZE (8)

#define MAX_NUM_OFFENCE_FORMATIONS (32)
#define MAX_NUM_DEFENCE_FORMATIONS (32)

#define MAX_NUM_OFFENCE_POSITIONS (32)
#define MAX_NUM_DEFENCE_POSITIONS (32)

#define MAX_NUM_OFFENCE_PLAYS (128)
#define MAX_NUM_DEFENCE_PLAYS (128)

#define MAX_NUM_OFFENCE_PLAYERS (64)
#define MAX_NUM_DEFENCE_PLAYERS (64)
#define MAX_NUM_SPECIALTEAMS_PLAYERS (16)
#define MAX_NUM_SQUAD_PLAYERS (64)

typedef struct gi_PositionValue gi_PositionValue;
typedef struct gi_OffencePlay gi_OffencePlay;
typedef struct gi_DefencePlay gi_DefencePlay;
typedef struct gi_Player gi_Player;
typedef struct gi_Team gi_Team;
typedef struct gi_PlayInfo gi_PlayInfo;
typedef struct gi_GlobalInfo gi_GlobalInfo;
typedef struct gi_SpecialTeamsValues gi_SpecialTeamsValues;

struct gi_SpecialTeamsValues
{
	float m_protector;
	float m_blocker;
	float m_runner;
	float m_gunner;
};

void gi_Init(void);
Json_Value* gi_ParseFile(const char* const fileName, const int debug);
GI_Return gi_LoadFile(const char* const fileName, const int debug);

#endif /* #ifndef GI_HH */
