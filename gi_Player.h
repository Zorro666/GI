#ifndef GI_PLAYER_HH
#define GI_PLAYER_HH

#include <stdio.h>
#include "gi.h"
#include "gi_SpecialTeamsValues.h"

struct gi_Player
{
	char m_name[GI_PLAYERNAME_MAX_SIZE];
	GI_POSITION m_position;
	int m_rawLevel;
	int m_rawQST[GI_QST_NUM];
	float m_experience;
	GI_SQUAD m_unit;
	int m_age;
	gi_SpecialTeamsValues m_specialTeamsValues;
	float m_level;
	float m_QST[GI_QST_NUM];
};

void gi_Player_Init(gi_Player* const pThis);
GI_Bool gi_Player_IsValueValid(const Json_Value* const root);
GI_Return gi_Player_Load(gi_Player* const pThis, const Json_Value* const root);
void gi_Player_Print(const gi_Player* const pThis, FILE* const pFile);
void gi_Player_ComputeSpecialTeams(gi_Player* const pThis);

#endif /* #ifndef GI_PLAYER_HH */
