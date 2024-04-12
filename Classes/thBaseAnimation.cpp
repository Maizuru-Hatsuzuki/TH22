/********************************************************
* Filename		: thBaseAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module.
********************************************************/

#include "thBaseMacro.h"
#include "thBaseAnimation.h"


CTHBaseAnimation* CTHBaseAnimation::m_pSelf;

CTHBaseAnimation::CTHBaseAnimation()
{
}

CTHBaseAnimation::~CTHBaseAnimation()
{
}

CTHBaseAnimation* CTHBaseAnimation::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CTHBaseAnimation();
	}
	return m_pSelf;
}

thBool CTHBaseAnimation::createPlayAnimationWithPList(THANIMATION_DESC_PTR ptAniDesc, Animate** ppRet)
{
	thBool bRet = THFALSE;
	int nFrameCount = 0;
	char szarrSprite[128] = { 0 };
	SpriteFrame* ptmpTex = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	TH_PROCESS_ERROR(pSpFrameCache);
	Animation* pAni = Animation::create();
	TH_PROCESS_ERROR(pAni);

	for (unsigned int i = ptAniDesc->nFrameAniBegin; i <= ptAniDesc->nFrameAniEnd; i++)
	{
		sprintf_s(szarrSprite, "%s%d.png", ptAniDesc->szarrFrameAni, i);
		ptmpTex = pSpFrameCache->spriteFrameByName(szarrSprite);
		TH_PROCESS_ERROR(ptmpTex);
		pAni->addSpriteFrame(ptmpTex);
		nFrameCount++;
	}

	if (true == ptAniDesc->bReverseReturn)
	{
		for (unsigned int i = ptAniDesc->nFrameAniEnd - 1; i <= ptAniDesc->nFrameAniBegin; i--)
		{
			sprintf_s(szarrSprite, "%s%d.png", ptAniDesc->szarrFrameAni, i);
			ptmpTex = pSpFrameCache->spriteFrameByName(szarrSprite);
			TH_PROCESS_ERROR(ptmpTex);
			pAni->addSpriteFrame(ptmpTex);
			nFrameCount++;
		}
	}

	pAni->setDelayPerUnit(ptAniDesc->fDelayPerUnit);
	pAni->setLoops(ptAniDesc->nLoops);
	pAni->setRestoreOriginalFrame(ptAniDesc->bResFirstFrame);
	
	*ppRet = Animate::create(pAni);
	CCLOG("Total %d frame count.\n", nFrameCount);

	bRet = THTRUE;
Exit0:
	return bRet;
}
