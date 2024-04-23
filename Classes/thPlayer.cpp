/********************************************************
* Filename		: thPlayerAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: player module.
********************************************************/


#include "thBaseAnimation.h"
#include "thBaseMacro.h"
#include "thPlayerFSM.h"


CThPlayerAction* CThPlayerAction::m_pSelf;


CThPlayer::CThPlayer()
{
}

CThPlayer::~CThPlayer()
{
	//TFC::uninitPlayerFSMAllEvent(m_ptPlayerFsmDesc);
	//THFREE(m_ptPlayerDesc);
}

thBool CThPlayer::init(CHARACTER_DESC_PTR pDesc)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThPlayer::initCharaterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet)
{

}

void CThPlayer::uninit()
{

}

void CThPlayer::getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{

}

void CThPlayer::getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
{

}

void CThPlayer::onMouseUp(EventMouse* pEvent)
{

}

void CThPlayer::onMouseDown(EventMouse* pEvent)
{

}

void CThPlayer::onMouseMove(EventMouse* pEvent)
{

}

/*
thBool CThPlayer::init(PLAYER_DESC_PTR ptPlayer)
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
	bFnRet = TecInitBitmap(THMAX_PLAYERFSMSTATUS, &m_pbmPlayerStatus);
	TH_PROCESS_ERROR(bFnRet);

	/* Init cocos. 
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

	/* Init thPlayer. 
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
		m_pPlayer->setFlippedX(true);
		break;

	case CMS_RIGHTMOVE:
		if (true == m_pPlayer->isFlippedX())
		{
			m_pPlayer->setFlippedX(false);
		}
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

void CThPlayer::doRunAction(Action* pAction)
{
	m_pPlayer->runAction(pAction);
	return;
}

void CThPlayer::doFsmCtrlUpdate()
{
	thBool bFnRet = THFALSE;

	m_ptPlayerFsmDesc->emCurPlayerStatus = m_emCurStatus;
	m_pPlayerFsmCtrl->thFsmMain(m_ptPlayerFsmDesc);
	return;
}

void CThPlayer::getPlayerPosition(float* pfX, float* pfY)
{
	*pfX = m_pPlayer->getPositionX();
	*pfY = m_pPlayer->getPositionY();
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

void CThPlayer::setPlayerFsmCurStatus(enum THEM_CHARACTERFSM_STATUS emStatus) noexcept
{
	m_emCurStatus = emStatus;
	return;
}
*/


CThPlayerAction::CThPlayerAction() {};
CThPlayerAction::~CThPlayerAction() {};

CThPlayerAction* CThPlayerAction::getInstance() noexcept
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CThPlayerAction;
	}
	return m_pSelf;
}

thBool CThPlayerAction::setPlayerMoveTo(enum THEM_CHARARCTERLEVEL_MOVESPEED emSpeed, enum THEM_CHARACTERFSM_STATUS emDirection, CThPlayer* pPlayer) noexcept
{
	thBool bRet = THFALSE;
	float fCurX = 0.0f;
	float fCurY = 0.0f;
	float fSpeed = 0.0f;
	Sequence* pMoveSeq = NULL;

	pPlayer->getPlayerPosition(&fCurX, &fCurY);

	switch (emSpeed)
	{
	case MOVESPEED_LOW:
		fSpeed = THMOVESPEED_LOW_PLAYER;
		break;
	case MOVESPEED_NARMAL:
		fSpeed = THMOVESPEED_NORMAL_PLAYER;
		break;
	case MOVESPEED_HIGH:
		fSpeed = THMOVESPEED_HIGH_PLAYER;
		break;
	case MOVESPEED_HIGHEX:
		fSpeed = THMOVESPEED_HIGHEX_PLAYER;
		break;
	default:
		break;
	}

	switch (emDirection)
	{
	case CMS_UNKNOW:
		break;
	case CMS_STANDBY:
		break;
	case CMS_LEFTMOVE:
		fCurX = fCurX - fSpeed;
		break;
	case CMS_RIGHTMOVE:
		fCurX = fCurX + fSpeed;
		break;
	case CMS_JUMP:
		break;
	case CMS_FLAG:
		break;
	default:
		break;
	}

	CThBaseCharacterAction::getInstance()->createActionMoveTo(0.01f, fCurX, fCurY, NULL, &pMoveSeq);
	TH_PROCESS_ERROR(pMoveSeq);
	pPlayer->doRunAction(pMoveSeq);

	bRet = THTRUE;
Exit0:
	return bRet;
}
