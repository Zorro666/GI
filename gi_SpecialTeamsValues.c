#include "gi_SpecialTeamsValues.h"
#include "gi_SpecialTeamsValuesPrivate.h"
#include "gi_Player.h"
#include "gi_Logger.h"

void gi_SpecialTeamsValues_Init(gi_SpecialTeamsValues* const pThis)
{
	pThis->m_blocker = 0.0f;
	pThis->m_gunner = 0.0f;
	pThis->m_protector = 0.0f;
	pThis->m_runner = 0.0f;
}

void gi_SpecialTeamsValues_Compute(gi_SpecialTeamsValues* const pThis, const gi_Player* const pPlayer)
{
	float Q = 0.0f;
	float S = 0.0f;
	float T = 0.0f;
	const float level = gi_Player_GetSpecialTeamsLevelQST(pPlayer, &Q, &S, &T);
	const GI_POSITION position = gi_Player_GetPosition(pPlayer);

	float blocker = 0.0f;
	float gunner = 0.0f;
	float protector = 0.0f;
	float runner = 0.0f;

	/* Personal Protector: RB, SF, TE, IB, OB: S x 1.5 */
	switch (position)
	{
		case GI_POSITION_RB:
		case GI_POSITION_SF:
		case GI_POSITION_TE:
		case GI_POSITION_IB:
		case GI_POSITION_OB:
			protector = 1.5f*S;
			break;
		default:
			protector = 0.0f;
			break;
	};

	/* Blocker: */
	/* DE, DT, C, OT, OG: L x 0.8 */
	/* IB, OB: L x 0.7 */
	/* TE, RB, SF: S + T x 0.33 */
	switch (position)
	{
		case GI_POSITION_DE:
		case GI_POSITION_DT:
		case GI_POSITION_OC:
		case GI_POSITION_OG:
			blocker = 0.8f*level;
			break;
		case GI_POSITION_IB:
		case GI_POSITION_OB:
			blocker = 0.7f*level;
			break;
		case GI_POSITION_TE:
		case GI_POSITION_RB:
		case GI_POSITION_SF:
			blocker = 0.33f*(S+T);
			break;
		default:
			blocker = 0.0f;
			break;
	};

	/* Runner: */
	/* R: L x 0.9 */
	/* CB, WR: L x 0.8 */
	/* TE, RB, SF: Q + T x 0.33 */
	switch (position)
	{
		case GI_POSITION_R:
			runner = 0.9f*level;
			break;
		case GI_POSITION_CB:
		case GI_POSITION_WR:
			runner = 0.8f*level;
			break;
		case GI_POSITION_TE:
		case GI_POSITION_RB:
		case GI_POSITION_SF:
			runner = 0.33f*(Q+T);
			break;
		default:
			runner = 0.0f;
			break;
	};

	/* Gunner: SF, WR, CB, R: Q x 1.5 */
	switch (position)
	{
		case GI_POSITION_SF:
		case GI_POSITION_WR:
		case GI_POSITION_CB:
		case GI_POSITION_R:
			gunner = 1.5f*Q;
			break;
		default:
			gunner = 0.0f;
			break;
	};

	pThis->m_protector = protector;
	pThis->m_blocker = blocker;
	pThis->m_runner = runner;
	pThis->m_gunner = gunner;
}

float gi_SpecialTeamsValues_GetProtector(const gi_SpecialTeamsValues* const pThis)
{
	return pThis->m_protector;
}

float gi_SpecialTeamsValues_GetBlocker(const gi_SpecialTeamsValues* const pThis)
{
	return pThis->m_blocker;
}

float gi_SpecialTeamsValues_GetRunner(const gi_SpecialTeamsValues* const pThis)
{
	return pThis->m_runner;
}

float gi_SpecialTeamsValues_GetGunner(const gi_SpecialTeamsValues* const pThis)
{
	return pThis->m_gunner;
}

