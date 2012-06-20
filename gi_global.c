#include "gi.h"

#include "gi_PlayerPrivate.h"
#include "gi_TeamPrivate.h"
#include "gi_OffencePlay.h"
#include "gi_DefencePlay.h"
#include "gi_SpecialTeamsValues.h"
#include "gi_PlayInfoPrivate.h"
#include "gi_GlobalInfoPrivate.h"

static gi_GlobalInfo s_globalInfo;
gi_GlobalInfo* s_pGlobalInfo = &s_globalInfo;
