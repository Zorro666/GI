#ifndef GI_PLAYER_HH
#define GI_PLAYER_HH

#include "gi.h"

typedef enum GI_QST
{
	GI_Q = 0,
	GI_S = 1,
	GI_T = 2
} GI_QST;

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
	GI_POSITION_UNKNOWN = 16
} GI_POSITION;

struct gi_Player
{
	char m_name[MAX_PLAYERNAME_SIZE];
	GI_POSITION m_position;
	int m_level;
	int m_qst[3];
	float m_experience;
	GI_SQUAD_UNIT m_unit;
	int m_age;
};

void gi_Player_Init(gi_Player* const pThis);
GI_Bool gi_Player_IsValueValid(const Json_Value* const root);
GI_Return gi_Player_Load(gi_Player* const pThis, const Json_Value* const root);
void gi_Player_Print(gi_Player* const pThis);
void gi_Player_ComputeSpecialTeams(const gi_Player* const pThis, gi_SpecialTeamsValues* const pSpecialTeamsValues);

#endif /* #ifndef GI_PLAYER_HH */
