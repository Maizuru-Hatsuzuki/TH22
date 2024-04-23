/********************************************************
* Filename		: thCharacter.cpp
* Creator		: ac
* Date time		: 2024.04.14
* Description	: base character.
********************************************************/

#include "thBaseMacro.h"
#include "thBaseAnimation.h"


CThBaseCharacterAction* CThBaseCharacterAction::m_pSelf;


CThBaseCharacter::CThBaseCharacter()
{
}

CThBaseCharacter::~CThBaseCharacter()
{
}

thBool CThBaseCharacter::initCharater(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR ptCharFrame = THMALLOC(CHARACTER_FRAMEINFO, sizeof(CHARACTER_FRAMEINFO));
	TH_PROCESS_ERROR(ptCharFrame);

	bFnRet = initCharaterFrameInfo(ptCharFrame);
	TH_PROCESS_ERROR(bFnRet);

	ptCharFrame->pSpCharater = Sprite::create(pDesc->cszpSpriteTexPath);
	TH_PROCESS_ERROR(ptCharFrame->pSpCharater);
	ptCharFrame->pSpCharater->setFlippedX(pDesc->bFlipX);
	ptCharFrame->pSpCharater->setFlippedY(pDesc->bFlipY);
	ptCharFrame->pSpCharater->setScale(pDesc->fScale);
	ptCharFrame->pSpCharater->setPositionX(pDesc->fPosX);
	ptCharFrame->pSpCharater->setPositionY(pDesc->fPosY);

	strcpy_s(ptCharFrame->szarrDesc, strlen(pDesc->cszpSpriteName) + 1, pDesc->cszpSpriteName);
	*ppRet = ptCharFrame;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThBaseCharacter::initCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR pRet)
{
	thBool bRet = THFALSE;

	pRet->emMoveSpeed = MOVESPEED_NARMAL;
	pRet->nAttack = 20;
	pRet->nDefense = 20;
	pRet->nHP = 100;
	pRet->nMP = 100;
	pRet->nCDResurrection = 30;
	pRet->nDuration = 30;
	pRet->nLastestDieTime = 0;
	pRet->pSpCharater = NULL;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThBaseCharacter::initCharaterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet)
{
	thBool bRet = THFALSE;
	bRet = CThBaseAnimation::getInstance()->createPlayAnimationWithPList(pAniDesc, ppRet);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThBaseCharacter::uninitCharater(CHARACTER_FRAMEINFO_PTR pCharater)
{
	if (0 != pCharater->pSpCharater->getReferenceCount())
	{
		pCharater->pSpCharater->removeAllChildren();
	}
	THFREE(pCharater);
	return;
}


thBool CThBaseCharacter::getCharaterAnimateFrameInfo(CHARACTER_ANI_DESC_PTR pAniDesc, CHARACTER_ANI_FRAMEINFO_PTR pRet) const
{
	thBool bRet = THFALSE;
	pRet->pAnimate = NULL;
	bRet = CThBaseAnimation::getInstance()->createPlayAnimationWithPList(pAniDesc, &pRet->pAnimate);
	TH_PROCESS_ERROR(pRet->pAnimate);

	bRet = THTRUE;
Exit0:
	return bRet;
}


CThBaseCharacterAction::CThBaseCharacterAction()
{
}

CThBaseCharacterAction::~CThBaseCharacterAction()
{
}

CThBaseCharacterAction* CThBaseCharacterAction::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = THNEW_CLASS(CThBaseCharacterAction);
	}
	return m_pSelf;
}

thBool CThBaseCharacterAction::createActionMoveTo(float fSpeed, float fDstX, float fDstY, FiniteTimeAction** arrfnCallback, Sequence** ppRet) noexcept
{
	thBool bRet = THFALSE;
	unsigned int ntmpFnCount = 1;
	MoveTo* pMoveTo = MoveTo::create(fSpeed, Vec2(fDstX, fDstY));
	cocos2d::Vector<FiniteTimeAction*> vecSeq(THMAX_SEQACTION);
	Sequence* pActionSeq = NULL;

	vecSeq.insert(0, pMoveTo);

	if (NULL != arrfnCallback)
	{
		while (ntmpFnCount < THMAX_SEQACTION - 1)
		{
			if (NULL != arrfnCallback[ntmpFnCount])
			{
				vecSeq.insert(ntmpFnCount, arrfnCallback[ntmpFnCount]);
			}
			ntmpFnCount++;
		}
	}

	pActionSeq = Sequence::create(vecSeq);
	TH_PROCESS_ERROR(pActionSeq);

	*ppRet = pActionSeq;
	bRet = THTRUE;
Exit0:
	return bRet;
}
