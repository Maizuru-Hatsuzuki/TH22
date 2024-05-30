/********************************************************
* Filename		: thCcAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module.
********************************************************/

#include "thBaseMacro.h"
#include "thCcAnimation.h"


CthCcFrameByFrameAnimation* CthCcFrameByFrameAnimation::m_pSelf;

CthCcFrameByFrameAnimation::CthCcFrameByFrameAnimation()
{
}

CthCcFrameByFrameAnimation::~CthCcFrameByFrameAnimation()
{
}

CthCcFrameByFrameAnimation* CthCcFrameByFrameAnimation::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CthCcFrameByFrameAnimation();
	}
	return m_pSelf;
}

thBool CthCcFrameByFrameAnimation::createAnimationWithPList(CHARACTER_ANI_DESC_PTR ptAniDesc, Animate** ppRet)
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
		sprintf_s(szarrSprite, "%s%d.png", ptAniDesc->szarrSpriteTex, i);
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

thBool CthCcFrameByFrameAnimation::createAnimationWithPListIni(const char* cszpIniPath, CHARACTER_ANI_FRAMEINFO_PTR* ppRet)
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_DESC_PTR ptmpAniDesc = NULL;
	CHARACTER_ANI_FRAMEINFO_PTR pRet = NULL;

	bRet = CthCcCharacterLoadHandler::getInstance()->getCharacterAniDescFromIni(cszpIniPath, &ptmpAniDesc);
	TH_PROCESS_ERROR(bRet);
	
	pRet = THMALLOC(CHARACTER_ANI_FRAMEINFO, sizeof(CHARACTER_ANI_FRAMEINFO));
	TH_PROCESS_ERROR(pRet);
	pRet->pAnimate = NULL;
	strcpy_s(pRet->szarrDesc, strlen(ptmpAniDesc->szarrAniDesc) + 1, ptmpAniDesc->szarrAniDesc);

	bRet = createAnimationWithPList(ptmpAniDesc, &pRet->pAnimate);
	TH_PROCESS_ERROR(bRet);

	CthCcCharacterLoadHandler::getInstance()->uninitCharacterAniDesc(ptmpAniDesc);
	
	*ppRet = pRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CthCcFrameByFrameAnimation::createLoadingBar(Sprite* pSpLoading, const float cfInterval, const float cfPersent, CHARACTER_DESC_PTR ptSpLoading, ProgressTo** ppActionRet, ProgressTimer** ppLoadingPicRet)
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

