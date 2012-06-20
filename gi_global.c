#include "gi.h"

#include "gi_GlobalInfoPrivate.h"

static gi_GlobalInfo s_globalInfo;
static gi_Team s_team;
static gi_OffencePlay s_offencePlay;
static gi_DefencePlay s_defencePlay;

gi_GlobalInfo* s_pGlobalInfo = &s_globalInfo;
gi_Team* s_pTeam = &s_team;
gi_OffencePlay* s_pOffencePlay = &s_offencePlay;
gi_DefencePlay* s_pDefencePlay = &s_defencePlay;
