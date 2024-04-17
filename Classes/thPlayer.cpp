/********************************************************
* Filename		: thPlayerAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: player module.
********************************************************/


#include "thBaseAnimation.h"
#include "thBaseMacro.h"
#include "thPlayerFSM.h"


CThPlayer::CThPlayer()
{
}

CThPlayer::~CThPlayer()
{
	TFC::uninitPlayerFSMAllEvent(m_ptPlayerFsmDesc);
	THFREE(m_ptPlayerDesc);
}

thBool CThPlayer::init(PLAYER_DESC_PTR ptPlayer, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	char szarrPListPath[MAX_PATH] = { 0 };
	char szarrPListPngPath[MAX_PATH] = { 0 };
	char szarrBasicSpritePath[MAX_PATH] = { 0 };
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	TH_PROCESS_ERROR(pSpFrameCache);

	m_pAniStandby = NULL;
	m_pPlayerFsmCtrl = new CThFSMCtrl;

	/* Init cocos. */
	sprintf_s(szarrPListPath, "image\\sprite\\%s\\cocos2d\\%s.plist", ptPlayer->cszpSpriteName, ptPlayer->cszpPListName);
	sprintf_s(szarrPListPngPath, "image\\sprite\\%s\\cocos2d\\%s.png", ptPlayer->cszpSpriteName, ptPlayer->cszpPListName);
	sprintf_s(szarrBasicSpritePath, "image\\sprite\\%s\\cocos2d\\%s.png", ptPlayer->cszpSpriteName, ptPlayer->cszpFirstFrame);

	pSpFrameCache->addSpriteFramesWithFile(szarrPListPath, szarrPListPngPath);

	m_pPlayer = Sprite::create(szarrBasicSpritePath);
	TH_PROCESS_ERROR(m_pPlayer);

	m_pPlayer->setPosition(ptPlayer->fPosX, ptPlayer->fPosY);
	m_pPlayer->setScale(ptPlayer->fScale);
	m_pPlayer->setFlippedX(ptPlayer->bFlipX);
	m_pPlayer->setFlippedY(ptPlayer->bFlipY);
	this->addChild(m_pPlayer);

	scheduleUpdate();

	/* Init thPlayer. */
	m_ptPlayerDesc = (PLAYER_DESC_PTR)malloc(sizeof(PLAYER_DESC));
	memcpy_s(m_ptPlayerDesc, sizeof(PLAYER_DESC), ptPlayer, sizeof(PLAYER_DESC));

	m_emCurStatus = THEM_CHARACTERFSM_STATUS::CMS_UNKNOW;

	bFnRet = TFC::initPlayerFSMAllEvent(&m_ptPlayerFsmDesc);
	TH_PROCESS_ERROR(m_ptPlayerFsmDesc);
	m_ptPlayerFsmDesc->emCurPlayerStatus = m_emCurStatus;
	m_ptPlayerFsmDesc->emLastPlayerStatus = m_emCurStatus;
	m_ptPlayerFsmDesc->vpEnv = (void*)this;

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThPlayer::update(float dt)
{
	doFsmCtrlUpdate();
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
		0.f,
		-1,
		false,
	};
	strcpy_s(tAniDesc.szarrFrameAni, strlen(szpFrameAni) + 1, szpFrameAni);
	
	if (NULL == m_pAniStandby)
	{
		
	}
	bFnRet = CThBaseAnimation::getInstance()->createPlayAnimationWithPList(&tAniDesc, &m_pAniStandby);
	TH_PROCESS_ERROR(bFnRet);
	m_pPlayer->runAction(m_pAniStandby);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::doPlayAnimationMove(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd, THEM_CHARACTERFSM_STATUS emMoveDirection)
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
		0.f,
		-1,
		false,
	};

	strcpy_s(tAniDesc.szarrFrameAni, strlen(szpFrameAni) + 1, szpFrameAni);

	switch (emMoveDirection)
	{
	case CMS_LEFTMOVE:
		m_pPlayer->setFlipX(true);
		break;

	case CMS_RIGHTMOVE:
		break;

	default:
		break;
	}

	bFnRet = CThBaseAnimation::getInstance()->createPlayAnimationWithPList(&tAniDesc, &m_pAniMove);
	TH_PROCESS_ERROR(bFnRet);
	m_pPlayer->runAction(m_pAniMove);
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThPlayer::doFsmCtrlUpdate()
{
	thBool bFnRet = THFALSE;

	m_ptPlayerFsmDesc->emCurPlayerStatus = m_emCurStatus;
	m_pPlayerFsmCtrl->thFsmMain(m_ptPlayerFsmDesc);
	return;
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

thBool CThPlayer::setPlayerMoveTo(const float fX, const float fY)
{
	thBool bRet = THFALSE;
	MoveTo* pPlayerMove = MoveTo::create(0.01f, Vec2(m_pPlayer->getPositionX() + fX, m_pPlayer->getPositionY() + fY));
	TH_PROCESS_ERROR(pPlayerMove);
	Sequence* pPlayerMoveAction = Sequence::create(pPlayerMove, NULL);
	TH_PROCESS_ERROR(pPlayerMoveAction);

	m_pPlayer->runAction(pPlayerMoveAction);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::setPlayerStopAllAction()
{
	thBool bRet = THFALSE;
	m_pPlayer->stopAllActions();

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThPlayer::setPlayerFsmCurStatus(enum THEM_CHARACTERFSM_STATUS emStatus)
{
	m_emCurStatus = emStatus;
	return;
}
