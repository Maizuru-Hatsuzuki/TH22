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

thBool CThBaseCharacter::initCharater(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet, thBool bInitSp) const
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR ptCharFrame = THMALLOC(CHARACTER_FRAMEINFO, sizeof(CHARACTER_FRAMEINFO));
	TH_PROCESS_ERROR(ptCharFrame);
	
	if (THTRUE == bInitSp)
	{
		ptCharFrame->pSpCharacter = Sprite::create(pDesc->cszpSpriteTexPath);
		TH_PROCESS_ERROR(ptCharFrame->pSpCharacter);
		ptCharFrame->pSpCharacter->setFlippedX(pDesc->bFlipX);
		ptCharFrame->pSpCharacter->setFlippedY(pDesc->bFlipY);
		ptCharFrame->pSpCharacter->setScale(pDesc->fScale);
		ptCharFrame->pSpCharacter->setPositionX(pDesc->fPosX);
		ptCharFrame->pSpCharacter->setPositionY(pDesc->fPosY);
	}
	else
	{
		ptCharFrame->pSpCharacter = NULL;
	}

	ptCharFrame->nHP = pDesc->nHP;
	ptCharFrame->nMP = pDesc->nMP;
	ptCharFrame->nAttack = pDesc->nAttack;
	ptCharFrame->nAttackCD = pDesc->nAttackCD;
	ptCharFrame->nDefense = pDesc->nDefense;
	ptCharFrame->nCDResurrection = pDesc->nCDResurrection;
	ptCharFrame->nLastestDieTime = pDesc->nLastestDieTime;
	ptCharFrame->nDuration = pDesc->nDuration;
	ptCharFrame->emCharacterType = pDesc->emCharacterType;
	ptCharFrame->emMoveSpeed = pDesc->emMoveSpeed;
	strcpy_s(ptCharFrame->szarrDesc, strlen(pDesc->cszpSpriteName) + 1, pDesc->cszpSpriteName);

	*ppRet = ptCharFrame;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThBaseCharacter::initCharaterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet) const
{
	thBool bRet = THFALSE;
	char szarrPlistPath[MAX_PATH] = { 0 };
	char szarrPlistPngPath[MAX_PATH] = { 0 };
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	TH_PROCESS_ERROR(pSpFrameCache);

	sprintf_s(szarrPlistPath, "%s.plist", pAniDesc->szarrBasicFrameAniPlistPath);
	sprintf_s(szarrPlistPngPath, "%s.png", pAniDesc->szarrBasicFrameAniPlistPath);

	pSpFrameCache->addSpriteFramesWithFile(szarrPlistPath, szarrPlistPngPath);
	bRet = CThBaseAnimation::getInstance()->createPlayAnimationWithPList(pAniDesc, ppRet);
	TH_PROCESS_ERROR(bRet);

	(*ppRet)->retain();
	bRet = THTRUE;
Exit0:
	pSpFrameCache->removeSpriteFramesFromFile(szarrPlistPath);
	return bRet;
}

void CThBaseCharacter::uninitCharater(CHARACTER_FRAMEINFO_PTR pCharater)
{
	if (0 != pCharater->pSpCharacter->getReferenceCount())
	{
		pCharater->pSpCharacter->removeAllChildren();
	}
	THFREE(pCharater);
	return;
}

void CThBaseCharacter::setPlayerRunAction(Action* pAction, Sprite* pSp)
{
	pSp->runAction(pAction);
	return;
}

void CThBaseCharacter::setPlayerStopAllAction(Sprite* pSp)
{
	pSp->stopAllActions();
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

thBool CThBaseCharacterAction::createActionMoveTo(float fSpeed, float fDstX, float fDstY, FiniteTimeAction** arrfnCallback, const short csCallbackSize, Sequence** ppRet) noexcept
{
	thBool bRet = THFALSE;
	unsigned short ustmpFnCount = 1;
	MoveTo* pMoveTo = MoveTo::create(fSpeed, Vec2(fDstX, fDstY));
	cocos2d::Vector<FiniteTimeAction*> vecSeq(THMAX_SEQACTION);
	Sequence* pActionSeq = NULL;

	TH_PROCESS_SUCCESS(THMAX_SEQACTION < csCallbackSize);
	vecSeq.insert(0, pMoveTo);

	if (NULL != arrfnCallback)
	{
		/* ��ѭ��֮ǰ0λ�ò����������������Դ�1��ʼ���ж������ĳ� <= . */
		while (ustmpFnCount <= csCallbackSize)
		{
			if (NULL != arrfnCallback[ustmpFnCount - 1])
			{
				vecSeq.insert(ustmpFnCount, arrfnCallback[ustmpFnCount - 1]);
			}
			ustmpFnCount++;
		}
	}

	pActionSeq = Sequence::create(vecSeq);
	TH_PROCESS_ERROR(pActionSeq);

	*ppRet = pActionSeq;
	bRet = THTRUE;
Exit0:
	return bRet;
}
