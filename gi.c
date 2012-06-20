#include <stdio.h>
#include <string.h>

#include "gi.h"

#include "gi_OffencePlay.h"
#include "gi_DefencePlay.h"
#include "gi_Team.h"
#include "gi_GlobalInfo.h"

#include "gi_Logger.h"

static BlockAllocator s_json_allocator;
extern gi_GlobalInfo* s_pGlobalInfo;
extern gi_Team* s_pTeam ;
extern gi_OffencePlay* s_pOffencePlay;
extern gi_DefencePlay* s_pDefencePlay;


static const char* const s_injuryNames[GI_INJURY_NUM] = {
	"A",				/*GI_INJURY_A = 0,*/
	"B",				/*GI_INJURY_B = 1,*/
	"C",				/*GI_INJURY_C = 2,*/
	"D",				/*GI_INJURY_D = 3,*/
	"E",				/*GI_INJURY_E = 4,*/
	"F",				/*GI_INJURY_F = 5,*/
	"G",				/*GI_INJURY_G = 6,*/
	"H",				/*GI_INJURY_H = 7,*/
	"UNKNOWN",	/*GI_INJURY_UNKNOWN = 8,*/
	};

static const char* const s_positionNames[GI_POSITION_NUM] = {
	"QB",     	/*GI_POSITION_QB = 0,*/
	"RB",     	/*GI_POSITION_RB = 1,*/
	"WR",     	/*GI_POSITION_WR = 2,*/
	"TE",     	/*GI_POSITION_TE = 3,*/
	"OC",     	/*GI_POSITION_OC = 4,*/
	"OG",     	/*GI_POSITION_OG = 5,*/
	"OT",     	/*GI_POSITION_OT = 6,*/
	"DE",     	/*GI_POSITION_DE = 7,*/
	"DT",     	/*GI_POSITION_DT = 8,*/
	"IB",     	/*GI_POSITION_IB = 9,*/
	"OB",     	/*GI_POSITION_OB = 10,*/
	"CB",     	/*GI_POSITION_CB = 11,*/
	"SF",     	/*GI_POSITION_SF = 12,*/
	"R",      	/*GI_POSITION_R = 13,*/
	"K",      	/*GI_POSITION_K = 14,*/
	"P",      	/*GI_POSITION_P = 15,*/
	"SE",     	/*GI_POSITION_SE = 16,*/
	"FL",     	/*GI_POSITION_FL = 17,*/
	"RB1",    	/*GI_POSITION_RB1 = 18,*/
	"RB2",    	/*GI_RPOSITION_B2 = 19,*/
	"TE1",			/*GI_POSITION_TE1 = 20,*/
	"TE2",			/*GI_POSITION_TE2 = 21,*/
	"LG",				/*GI_POSITION_LG = 22,*/
	"RG",				/*GI_POSITION_RG = 23,*/
	"LT",				/*GI_POSITION_LT = 24,*/
	"RT",				/*GI_POSITION_RT = 25,*/
	"HB",     	/*GI_POSITION_HB = 26,*/
	"FB",     	/*GI_POSITION_FB = 27,*/
	"FS",     	/*GI_POSITION_FS = 28,*/
	"SS",     	/*GI_POSITION_SS = 29,*/
	"C",     		/*GI_POSITION_C = 30,*/
	"NT",    		/*GI_POSITION_NT = 31,*/
	"LE",    		/*GI_POSITION_LE = 33,*/
	"RE",    		/*GI_POSITION_RE = 34,*/
	"LILB",    	/*GI_POSITION_LILB = 35,*/
	"RILB",    	/*GI_POSITION_RILB = 36,*/
	"LLB",    	/*GI_POSITION_LLB = 36,*/
	"RLB",    	/*GI_POSITION_RLB = 37,*/
	"UNKNOWN" 	/*GI_POSITION_UNKNOWN = 38,*/
	};

static const char* const s_unitNames[GI_UNIT_NUM] = {
	"Offence",     		/*GI_UNIT_OFFENCE = 0,*/
	"Defence",     		/*GI_UNIT_DEFENCE = 1,*/
	"Special Teams", 	/*GI_UNIT_SPECIALTEAMS = 2,*/
	"UNKNOWN"     		/*GI_UNIT_UNKNOWN = 3,*/
	};

static const char* const s_qstNames[GI_QST_NUM] = {
	"Q",     		/*GI_QST_Q = 0,*/
	"S",     		/*GI_QST_S = 1,*/
	"T", 				/*GI_QST_T = 2,*/
	"NONE", 		/*GI_QST_NONE = 3,*/
	"UNKNOWN"  	/*GI_QST_UNKNOWN = 4,*/
	};

void gi_Init(void)
{
	gi_Logger_Init();
	BlockAllocator_Init(&s_json_allocator, 128*1024);
	gi_GlobalInfo_Init(s_pGlobalInfo);
}

const char* gi_GetInjuryName(const GI_INJURY injury)
{
	if (injury > GI_INJURY_NUM)
	{
		GI_FATAL_ERROR("gi_GetInjuryName invalid injury %d range:%d-%d", injury, 0, GI_INJURY_NUM);
		return NULL;
	}
	return s_injuryNames[injury];
}

GI_INJURY gi_GetInjuryFromName(const char* const injury)
{
	size_t i;
	for (i = 0; i < GI_INJURY_NUM; i++)
	{
		if (strcmp(s_injuryNames[i], injury) == 0)
		{
			return i;
		}
	}
	return GI_INJURY_UNKNOWN;
}

const char* gi_GetPositionName(const GI_POSITION position)
{
	if (position > GI_POSITION_NUM)
	{
		GI_FATAL_ERROR("gi_GetPositionName invalid position %d range:%d-%d", position, 0, GI_POSITION_NUM);
		return NULL;
	}
	return s_positionNames[position];
}

GI_POSITION gi_GetPositionFromName(const char* const position)
{
	size_t i;
	for (i = 0; i < GI_POSITION_NUM; i++)
	{
		if (strcmp(s_positionNames[i], position) == 0)
		{
			return i;
		}
	}
	return GI_POSITION_UNKNOWN;
}

const char* gi_GetUnitName(const GI_UNIT unit)
{
	if (unit > GI_UNIT_NUM)
	{
		GI_FATAL_ERROR("gi_GetUnitName invalid unit %d range:%d-%d", unit, 0, GI_UNIT_NUM);
		return NULL;
	}
	return s_unitNames[unit];
}

const char* gi_GetQSTName(const GI_QST qst)
{
	if (qst > GI_QST_NUM)
	{
		GI_FATAL_ERROR("gi_GetQSTName invalid qst %d range:%d-%d", qst, 0, GI_QST_NUM);
		return NULL;
	}
	return s_qstNames[qst];
}

GI_QST gi_GetQSTFromName(const char* const qst)
{
	size_t i;
	for (i = 0; i < GI_QST_NUM; i++)
	{
		if (strcmp(s_qstNames[i], qst) == 0)
		{
			return i;
		}
	}
	return GI_QST_UNKNOWN;
}

GI_QST gi_GetBasePosition(const GI_POSITION position)
{
	GI_POSITION base = position;
	switch (position)
	{
		case GI_POSITION_QB:
			base = GI_POSITION_QB;
			break;
		case GI_POSITION_WR:
		case GI_POSITION_SE:
		case GI_POSITION_FL:
			base = GI_POSITION_WR;
			break;
		case GI_POSITION_RB:
		case GI_POSITION_RB1:
		case GI_POSITION_RB2:
		case GI_POSITION_HB:
		case GI_POSITION_FB:
			base = GI_POSITION_RB;
			break;
		case GI_POSITION_TE:
		case GI_POSITION_TE1:
		case GI_POSITION_TE2:
			base = GI_POSITION_TE;
			break;
		case GI_POSITION_OC:
		case GI_POSITION_C:
			base = GI_POSITION_OC;
			break;
		case GI_POSITION_OG:
		case GI_POSITION_LG:
		case GI_POSITION_RG:
			base = GI_POSITION_OG;
			break;
		case GI_POSITION_OT:
		case GI_POSITION_LT:
		case GI_POSITION_RT:
			base = GI_POSITION_OT;
			break;
		case GI_POSITION_DE:
		case GI_POSITION_LE:
		case GI_POSITION_RE:
			base = GI_POSITION_DE;
			break;
		case GI_POSITION_DT:
		case GI_POSITION_NT:
			base = GI_POSITION_DT;
			break;
		case GI_POSITION_IB:
		case GI_POSITION_LILB:
		case GI_POSITION_RILB:
			base = GI_POSITION_IB;
			break;
		case GI_POSITION_OB:
		case GI_POSITION_LLB:
		case GI_POSITION_RLB:
			base = GI_POSITION_OB;
			break;
		case GI_POSITION_CB:
			base = GI_POSITION_CB;
			break;
		case GI_POSITION_SF:
		case GI_POSITION_FS:
		case GI_POSITION_SS:
			base = GI_POSITION_SF;
			break;
		case GI_POSITION_R:
			base = GI_POSITION_R;
			break;
		case GI_POSITION_K:
			base = GI_POSITION_K;
			break;
		case GI_POSITION_P:
			base = GI_POSITION_P;
			break;
		default:
			break;
	};
	return base;
}

Json_Value* gi_ParseFile(const char* const fileName, const size_t debug)
{
	Json_Value* root;
	char* error_pos;
	char* error_desc;
	int error_line;

	root = Json_ParseFile(fileName, &error_pos, &error_desc, &error_line, &s_json_allocator);
	if (root)
	{
		if (debug == 1)
		{
			Json_Print(root, stdout, 0);
		}
	}
	else
	{
		GI_FATAL_ERROR("parsing file '%s'\nline:%d pos:'%s' desc:'%s'\n", fileName, error_line, error_pos, error_desc);
		return NULL;
	}

	return root;
}

GI_RETURN gi_HandleValue(Json_Value* const value, const size_t debug)
{
	if (value == NULL)
	{
		return GI_RETURN_ERROR;
	}
	if (value->m_type == JSON_OBJECT)
	{
		if (value->m_name != NULL) 
		{
			if (gi_OffencePlay_IsValueValid(value) == GI_TRUE)
			{
				gi_OffencePlay* const pOffencePlay = s_pOffencePlay;
				if (gi_OffencePlay_Load(pOffencePlay, value) == GI_RETURN_SUCCESS)
				{
					if (debug == 1)
					{
						gi_OffencePlay_Print(pOffencePlay, stdout);
					}
					gi_GlobalInfo_AddOffencePlay(s_pGlobalInfo, pOffencePlay);
					return GI_RETURN_SUCCESS;
				}
			}
			if (gi_DefencePlay_IsValueValid(value) == GI_TRUE)
			{
				gi_DefencePlay* const pDefencePlay = s_pDefencePlay;
				if (gi_DefencePlay_Load(pDefencePlay, value) == GI_RETURN_SUCCESS)
				{
					if (debug == 1)
					{
						gi_DefencePlay_Print(pDefencePlay, stdout);
					}
					gi_GlobalInfo_AddDefencePlay(s_pGlobalInfo, pDefencePlay);
					return GI_RETURN_SUCCESS;
				}
			}
			if (gi_Team_IsValueValid(value) == GI_TRUE)
			{
				gi_Team* const pTeam = s_pTeam;
				if (gi_Team_Load(pTeam, value) == GI_RETURN_SUCCESS)
				{
					if (debug == 1)
					{
						gi_Team_Print(pTeam, stdout);
					}
					gi_GlobalInfo_AddTeam(s_pGlobalInfo, pTeam);
					return GI_RETURN_SUCCESS;
				}
			}
		}
	}
	if ((value->m_type == JSON_OBJECT) || (value->m_type == JSON_ARRAY))
	{
		Json_Value* it;
		for (it = value->m_first_child; it; it = it->m_next_sibling)
		{
			gi_HandleValue(it, debug);
		}
	}
	return GI_RETURN_ERROR;
}

GI_RETURN gi_LoadFile(const char* const fileName, const size_t debug)
{
	Json_Value* value = gi_ParseFile(fileName, debug);
	if (value == NULL)
	{
		return GI_RETURN_ERROR;
	}
	while (value != NULL)
	{
		if (gi_HandleValue(value, debug) == GI_RETURN_SUCCESS)
		{
			return GI_RETURN_SUCCESS;
		}
		break;
	}
	return GI_RETURN_ERROR;
}

static FILE* openOutputFile(const char* const fileName)
{
	FILE* pFile = NULL;
	pFile = fopen(fileName, "wa");
	if (pFile == NULL)
	{
		GI_FATAL_ERROR("opening file '%s'\n", fileName);
	}
	return pFile;
}

static GI_RETURN gi_Output_SpecialTeamsStats(const gi_Team* const pTeam, const gi_PlayInfo* const pPlayInfo)
{
	FILE* pFile;
	const char* const fileName = "SpecialTeams_output.txt";
	pFile = openOutputFile(fileName);
	if (pFile == NULL)
	{
		return GI_RETURN_ERROR;
	}
	gi_Team_PrintBestSpecialTeams(pTeam, pPlayInfo, pFile);
	fclose(pFile);
	return GI_RETURN_SUCCESS;
}

static GI_RETURN gi_Output_Team(const gi_Team* const pTeam)
{
	FILE* pFile;
	char fileName[GI_OUTPUTFILENAME_MAX_SIZE];
	const char* const teamName = gi_Team_GetName(pTeam);

	if (teamName[0] == '\0')
	{
		GI_FATAL_ERROR("gi_Output_Team: team name is empty\n");
		return GI_RETURN_ERROR;
	}
	fileName[0] = '\0';
	strcpy(fileName, teamName);
	strcat(fileName, "_output.txt");
	pFile = openOutputFile(fileName);
	if (pFile == NULL)
	{
		return GI_RETURN_ERROR;
	}
	gi_Team_Print(pTeam, pFile);
	fclose(pFile);
	return GI_RETURN_SUCCESS;
}

void gi_Compute(void)
{
	gi_GlobalInfo_Compute(s_pGlobalInfo);
}

void gi_Output(void)
{
	const gi_Team* const pTeam = gi_GlobalInfo_GetTeam(s_pGlobalInfo);
	const gi_PlayInfo* const pPlayInfo = gi_GlobalInfo_GetPlayInfo(s_pGlobalInfo);
	if (gi_Output_SpecialTeamsStats(pTeam, pPlayInfo) == GI_RETURN_ERROR)
	{
		GI_FATAL_ERROR("outputting special teams stats\n");
	}
	if (gi_Output_Team(pTeam) == GI_RETURN_ERROR)
	{
		GI_FATAL_ERROR("outputting team\n");
	}
}
