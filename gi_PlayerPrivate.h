#ifndef GI_PLAYERPRIVATE_HH
#define GI_PLAYERPRIVATE_HH

struct gi_Player
{
	char m_name[GI_PLAYERNAME_MAX_SIZE];
	GI_POSITION m_position;
	size_t m_rawLevel;
	size_t m_rawQST[GI_QST_NUM];
	float m_experience;
	GI_UNIT m_unit;
	size_t m_age;
	float m_level;
	float m_QST[GI_QST_NUM];
	GI_INJURY m_injury;
};

#endif /*#ifndef GI_PLAYERPRIVATE_HH*/

