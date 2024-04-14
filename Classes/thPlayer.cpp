/********************************************************
* Filename		: thPlayerAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: player module.
********************************************************/

#include "thPlayer.h"
#include "thBaseAnimation.h"
#include "thBaseMacro.h"


CThPlayer::CThPlayer()
{
}

CThPlayer::~CThPlayer()
{
}

thBool CThPlayer::init(PLAYER_DESC_PTR ptPlayer, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	char szarrPListPath[MAX_PATH] = { 0 };
	char szarrPListPngPath[MAX_PATH] = { 0 };
	SpriteFrame* ptmpTex = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	TH_PROCESS_ERROR(pSpFrameCache);

	/* Init thPlayer. */
	bFnRet = initPlayerFsm();
	TH_PROCESS_ERROR(bFnRet);

	/* Init cocos. */
	sprintf_s(szarrPListPath, "image\\sprite\\%s\\cocos2d\\%s.plist", ptPlayer->cszpSpriteName, ptPlayer->cszpPListName);
	sprintf_s(szarrPListPngPath, "image\\sprite\\%s\\cocos2d\\%s.png", ptPlayer->cszpSpriteName, ptPlayer->cszpPListName);

	pSpFrameCache->addSpriteFramesWithFile(szarrPListPath, szarrPListPngPath);
	ptmpTex = pSpFrameCache->spriteFrameByName(ptPlayer->cszpFirstFrame);
	TH_PROCESS_ERROR(ptmpTex);
	m_pPlayer = Sprite::createWithSpriteFrame(ptmpTex);
	TH_PROCESS_ERROR(m_pPlayer);

	m_pPlayer->setPosition(ptPlayer->fPosX, ptPlayer->fPosY);
	m_pPlayer->setScale(ptPlayer->fScale);

	m_ptPlayerDesc = ptPlayer;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::initPlayerFsm()
{
	thBool bRet = THFALSE;
	THCHARACTERFSM_DESC ptPlayerStatusStand =
	{
		THEM_CHARACTERFSM_STATUS::CMS_STANDBY,
		"Player standby.",
		& CThPlayer::_playerFsmStandbyInit,
		& CThPlayer::_playerFsmStandbyUpdate,
		& CThPlayer::_playerFsmStandbyRelease,
		NULL
	};
	THCHARACTERFSM_DESC ptPlayerStatusMove =
	{
		THEM_CHARACTERFSM_STATUS::CMS_MOVE,
		"Player move.",
		& CThPlayer::_playerFsmMoveInit,
		& CThPlayer::_playerFsmMoveUpdate,
		& CThPlayer::_playerFsmMoveRelease,
		NULL
	};

	/* Stand fsm */
	m_arrpPlayerFsm[0] = THNEW_CLASS(CTHCharaterFSM);
	m_arrpPlayerFsm[0]->init(&ptPlayerStatusStand);
	/* Move fsm */
	m_arrpPlayerFsm[1] = THNEW_CLASS(CTHCharaterFSM);
	m_arrpPlayerFsm[1]->init(&ptPlayerStatusMove);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::doPlayAnimationStandby(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	THANIMATION_DESC tAniDesc =
	{
		{ 0 },
		nAniCount,
		nAniBegin,
		nAniEnd,
		0.1f,
		-1,
		false,
		true
	};
	strcpy_s(tAniDesc.szarrFrameAni, strlen(szpFrameAni) + 1, szpFrameAni);
	
	bFnRet = CTHBaseAnimation::getInstance()->createPlayAnimationWithPList(&tAniDesc, &m_pAniStandby);
	TH_PROCESS_ERROR(bFnRet);

	m_pPlayer->runAction(m_pAniStandby);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::doPlayAnimationMove(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd)
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThPlayer::getPlayer(Sprite** pRet)
{
	*pRet = m_pPlayer;
	return;
}

void CThPlayer::getPlayerDesc(PLAYER_DESC_PTR* pptRet)
{
	*pptRet = m_ptPlayerDesc;
	return;
}

thBool CThPlayer::_playerFsmStandbyInit(void* vpArgs)
{
	thBool bRet = THFALSE;
	PLAYER_FSM_ARGS_PTR pArgs = static_cast<PLAYER_FSM_ARGS_PTR>(vpArgs);
	PLAYER_DESC_PTR ptPlayerDesc = NULL;
	pArgs->pEnv->getPlayerDesc(&ptPlayerDesc);

	bRet = pArgs->pEnv->doPlayAnimationStandby(ptPlayerDesc->cszpPListName, 4, 1, 5);
	TH_PROCESS_ERROR(bRet);
	CCLOG("DEBUG LOG: player standby init.");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::_playerFsmStandbyUpdate(void* vpArgs)
{
	thBool bRet = THFALSE;
	//CCLOG("DEBUG LOG: player standby UPDATE.");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::_playerFsmStandbyRelease(void* vpArgs)
{
	thBool bRet = THFALSE;
	CCLOG("DEBUG LOG: player standby release.");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::_playerFsmMoveInit(void* vpArgs)
{
	thBool bRet = THFALSE;
	CCLOG("DEBUG LOG: player move init.");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::_playerFsmMoveUpdate(void* vpArgs)
{
	thBool bRet = THFALSE;
	CCLOG("DEBUG LOG: player move update.");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::_playerFsmMoveRelease(void* vpArgs)
{
	thBool bRet = THFALSE;
	CCLOG("DEBUG LOG: player move release.");

	bRet = THTRUE;
Exit0:
	return bRet;
}
