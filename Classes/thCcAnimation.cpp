/********************************************************
* Filename		: thCcAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module.
********************************************************/

#include "thBaseMacro.h"
#include "thCcAnimation.h"


CthCcAnimation* CthCcAnimation::m_pSelf;

CthCcAnimation::CthCcAnimation()
{
}

CthCcAnimation::~CthCcAnimation()
{
}

CthCcAnimation* CthCcAnimation::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CthCcAnimation();
	}
	return m_pSelf;
}

thBool CthCcAnimation::createPlayAnimationWithPList(CHARACTER_ANI_DESC_PTR ptAniDesc, Animate** ppRet)
{
	thBool bRet = THFALSE;
	int nFrameCount = 0;
	char szarrSprite[128] = { 0 };
	SpriteFrame* pSpFrame = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::getInstance();
	TH_PROCESS_ERROR(pSpFrameCache);
	Animation* pAni = Animation::create();
	TH_PROCESS_ERROR(pAni);

	for (
		int i = ptAniDesc->nFrameAniBegin, j = 0;
		j < ptAniDesc->nFrameAniCount; 
		ptAniDesc->nFrameAniBegin > ptAniDesc->nFrameAniEnd ? i-- : i++, j++
		)
	{
		sprintf_s(szarrSprite, "%s%d.png", ptAniDesc->szarrPlistPngPath, i);
		pSpFrame = pSpFrameCache->getSpriteFrameByName(szarrSprite);
		TH_PROCESS_ERROR(pSpFrame);
		
		pAni->addSpriteFrame(pSpFrame);
		nFrameCount++;
	}

	pAni->setDelayPerUnit(ptAniDesc->fDelayPerUnit);
	pAni->setLoops(ptAniDesc->nLoops);
	pAni->setRestoreOriginalFrame(ptAniDesc->bResFirstFrame);
	
	*ppRet = Animate::create(pAni);
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CthCcAnimation::createLoadingBar(Sprite* pSpLoading, const float cfInterval, const float cfPersent, CHARACTER_DESC_PTR ptSpLoading, ProgressTo** ppActionRet, ProgressTimer** ppLoadingPicRet)
{
	thBool bRet = THFALSE;
	ProgressTo* pLoading = ProgressTo::create(cfInterval, cfPersent);
	TH_PROCESS_ERROR(pLoading);
	ProgressTimer* pLoadingPic = ProgressTimer::create(pSpLoading);;
	TH_PROCESS_ERROR(pLoadingPic);

	pLoadingPic->setScale(ptSpLoading->fScale);
	pLoadingPic->setType(ProgressTimer::Type::BAR);
	pLoadingPic->setBarChangeRate(Vec2(1, 0));
	pLoadingPic->setMidpoint(Vec2(0, 0));
	pLoadingPic->setPosition(ptSpLoading->fPosX, ptSpLoading->fPosY);

	*ppActionRet = pLoading;
	*ppLoadingPicRet = pLoadingPic;

	bRet = THTRUE;
Exit0:
	return bRet;
}
