#include <stdio.h>
#include <string.h>

#include "gi.h"
#include "gi_Team.h"
#include "gi_GlobalInfo.h"

static BlockAllocator s_json_allocator;
static gi_GlobalInfo s_globalInfo;

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
	"UNKNOWN" 	/*GI_POSITION_UNKNOWN = 31,*/
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
	BlockAllocator_Init(&s_json_allocator, 128*1024);
	gi_GlobalInfo_Init(&s_globalInfo);
}

const char* gi_GetPositionName(const GI_POSITION position)
{
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
	return s_unitNames[unit];
}

const char* gi_GetQSTName(const GI_QST qst)
{
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

Json_Value* gi_ParseFile(const char* const fileName, const int debug)
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
		fprintf(stderr, "ERROR parsing file '%s'\n", fileName);
		fprintf(stderr, "ERROR line:%d pos:'%s' desc:'%s'\n", error_line, error_pos, error_desc);
		return NULL;
	}

	/* TODO: error handling output the error information */
	return root;
}

GI_Return gi_HandleValue(Json_Value* const value, const int debug)
{
	if (value == NULL)
	{
		return GI_ERROR;
	}
	if (value->m_type == JSON_OBJECT)
	{
		if (value->m_name != NULL) 
		{
			if (gi_OffencePlay_IsValueValid(value) == GI_TRUE)
			{
				gi_OffencePlay play;
				if (gi_OffencePlay_Load(&play, value) == GI_SUCCESS)
				{
					if (debug == 1)
					{
						gi_OffencePlay_Print(&play, stdout);
					}
					gi_OffencePlay_Print(&play, stdout);
					
					return GI_SUCCESS;
				}
			}
			if (gi_Team_IsValueValid(value) == GI_TRUE)
			{
				gi_Team* const pTeam = &s_globalInfo.m_team;
				if (gi_Team_Load(pTeam, value) == GI_SUCCESS)
				{
					if (debug == 1)
					{
						gi_Team_Print(pTeam, stdout);
					}
					return GI_SUCCESS;
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
	return GI_ERROR;
}

GI_Return gi_LoadFile(const char* const fileName, const int debug)
{
	Json_Value* value = gi_ParseFile(fileName, debug);
	if (value == NULL)
	{
		return GI_ERROR;
	}
	while (value != NULL)
	{
		if (gi_HandleValue(value, debug) == GI_SUCCESS)
		{
			return GI_SUCCESS;
		}
		break;
	}
	return GI_ERROR;
}

static FILE* openOutputFile(const char* const fileName)
{
	FILE* pFile = NULL;
	pFile = fopen(fileName, "wa");
	if (pFile == NULL)
	{
		fprintf(stderr,"ERROR opening file '%s'\n", fileName);
	}
	return pFile;
}

static GI_Return gi_Output_SpecialTeamsStats(gi_Team* const pTeam)
{
	FILE* pFile;
	const char* const fileName = "SpecialTeams_output.txt";
	pFile = openOutputFile(fileName);
	if (pFile == NULL)
	{
		return GI_ERROR;
	}
	gi_Team_PrintBestSpecialTeams(pTeam, pFile);
	fclose(pFile);
	return GI_SUCCESS;
}

static GI_Return gi_Output_Team(gi_Team* const pTeam)
{
	FILE* pFile;
	char fileName[GI_OUTPUTFILENAME_MAX_SIZE];

	if (pTeam->m_name[0] == '\0')
	{
		fprintf(stderr, "gi_Output_Team team name is empty\n");
		return GI_ERROR;
	}
	fileName[0] = '\0';
	strcpy(fileName, pTeam->m_name);
	strcat(fileName, "_output.txt");
	pFile = openOutputFile(fileName);
	if (pFile == NULL)
	{
		return GI_ERROR;
	}
	gi_Team_Print(pTeam, pFile);
	fclose(pFile);
	return GI_SUCCESS;
}

void gi_Output(void)
{
	gi_Team* const pTeam = &s_globalInfo.m_team;
	if (gi_Output_SpecialTeamsStats(pTeam) == GI_ERROR)
	{
		fprintf(stderr,"ERROR outputting special teams stats\n");
	}
	if (gi_Output_Team(pTeam) == GI_ERROR)
	{
		fprintf(stderr,"ERROR outputting team\n");
	}
}
