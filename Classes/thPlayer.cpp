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

	sprintf_s(szarrPListPath, "image\\sprite\\%s\\cocos2d\\%s.plist", ptPlayer->cszpSpriteName, ptPlayer->cszpPListName);
	sprintf_s(szarrPListPngPath, "image\\sprite\\%s\\cocos2d\\%s.png", ptPlayer->cszpSpriteName, ptPlayer->cszpPListName);

	pSpFrameCache->addSpriteFramesWithFile(szarrPListPath, szarrPListPngPath);
	ptmpTex = pSpFrameCache->spriteFrameByName(ptPlayer->cszpFirstFrame);
	TH_PROCESS_ERROR(ptmpTex);
	m_pPlayer = Sprite::createWithSpriteFrame(ptmpTex);
	TH_PROCESS_ERROR(m_pPlayer);

	m_pPlayer->setPosition(ptPlayer->fPosX, ptPlayer->fPosY);
	m_pPlayer->setScale(ptPlayer->fScale);

	bFnRet = doPlayAnimationStandby(ptPlayer->cszpPListName, 4, 1, 5);
	TH_PROCESS_ERROR(bFnRet);

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

void CThPlayer::getPlayer(Sprite** pRet)
{
	*pRet = m_pPlayer;
	return;
}
