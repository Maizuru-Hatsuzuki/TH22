/********************************************************
* Filename		: thBaseAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module.
********************************************************/

#include "thBaseMacro.h"
#include "thBaseAnimation.h"


CThBaseAnimation* CThBaseAnimation::m_pSelf;

CThBaseAnimation::CThBaseAnimation()
{
}

CThBaseAnimation::~CThBaseAnimation()
{
}

CThBaseAnimation* CThBaseAnimation::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CThBaseAnimation();
	}
	return m_pSelf;
}

thBool CThBaseAnimation::createPlayAnimationWithPList(CHARACTER_ANI_DESC_PTR ptAniDesc, Animate** ppRet)
{
	thBool bRet = THFALSE;
	int nFrameCount = 0;
	char szarrSprite[128] = { 0 };
	SpriteFrame* pSpFrame = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	TH_PROCESS_ERROR(pSpFrameCache);
	Animation* pAni = Animation::create();
	TH_PROCESS_ERROR(pAni);

	for (unsigned int i = ptAniDesc->nFrameAniBegin; i <= ptAniDesc->nFrameAniEnd; i++)
	{
		sprintf_s(szarrSprite, "%s%d.png", ptAniDesc->szarrBasicFrameAniPath, i);
		pSpFrame = pSpFrameCache->spriteFrameByName(szarrSprite);
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
