#ifndef GI_PLAYER_HH
#define GI_PLAYER_HH

#include "gi.h"

typedef enum GI_QST
{
	GI_Q = 0,
	GI_S = 1,
	GI_T = 2
} GI_QST;

struct gi_Player
{
	char m_name[MAX_PLAYERNAME_SIZE];
	char m_position[MAX_POSITION_SIZE];
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
