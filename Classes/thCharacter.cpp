/********************************************************
* Filename		: thCharacter.cpp
* Creator		: ac
* Date time		: 2024.04.14
* Description	: base character.
********************************************************/

#include "thBaseMacro.h"
#include "thBaseAnimation.h"


CThBaseCharacterAction* CThBaseCharacterAction::m_pSelf;
CThCharacterLoadHandler* CThCharacterLoadHandler::m_pSelf;


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
		ptCharFrame->pSpCharacter = Sprite::create(pDesc->szarrSpriteTexPath);
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
	ptCharFrame->nAttackRadius = pDesc->nAttackRadius;
	ptCharFrame->nDefense = pDesc->nDefense;
	ptCharFrame->nCDResurrection = pDesc->nCDResurrection;
	ptCharFrame->nLastestDieTime = 0;
	ptCharFrame->nSupportDuration = pDesc->nSupportDuration;
	ptCharFrame->emCharacterType = pDesc->emCharacterType;
	ptCharFrame->emMoveSpeed = pDesc->emMoveSpeed;
	ptCharFrame->emMaxLevel = pDesc->emMaxLevel;
	strcpy_s(ptCharFrame->szarrDesc, strlen(pDesc->szarrSpriteName) + 1, pDesc->szarrSpriteName);

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
		/* 在循环之前0位置插入了主动作，所以从1开始，判断条件改成 <= . */
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


CThCharacterLoadHandler::CThCharacterLoadHandler()
{}

CThCharacterLoadHandler::~CThCharacterLoadHandler()
{}

CThCharacterLoadHandler* CThCharacterLoadHandler::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = THNEW_CLASS(CThCharacterLoadHandler);
	}
	return m_pSelf;
}

thBool CThCharacterLoadHandler::getCharaterDescFromIni(const char* cszpFilename, CHARACTER_DESC_PTR* ppRet)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	const char* cszpSelCharacterDesc = "CHARACTER_DESC";
	const char* cszpSelAniMap = "CHARACTER_ANI_MAP";
	char szarrTmpStr[MAX_PATH] = { 0 };
	char szarrTmpFloat[32] = { 0 };
	CHARACTER_DESC_PTR pRet = THMALLOC(CHARACTER_DESC, sizeof(CHARACTER_DESC));
	TH_PROCESS_ERROR(pRet);
	CHARACTER_ANI_MAP_PTR pAniMap = THMALLOC(CHARACTER_ANI_MAP, sizeof(CHARACTER_ANI_MAP));
	TH_PROCESS_ERROR(pAniMap);

	memset(pRet, 0, sizeof(CHARACTER_DESC));
	memset(pAniMap, 0, sizeof(CHARACTER_ANI_MAP));

	/* 加载 CHARACTER_DESC */
	GetPrivateProfileStringA(cszpSelCharacterDesc, "szarrSpriteName", "NA", szarrTmpStr, 32, cszpFilename);
	strcpy_s(pRet->szarrSpriteName, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "szarrSpriteTexPath", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pRet->szarrSpriteTexPath, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "fPosX", "0.0", szarrTmpFloat, 32, cszpFilename);
	pRet->fPosX = (float)atof(szarrTmpFloat);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "fPosY", "0.0", szarrTmpFloat, 32, cszpFilename);
	pRet->fPosY = (float)atof(szarrTmpFloat);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "fScale", "0.0", szarrTmpFloat, 32, cszpFilename);
	pRet->fScale = (float)atof(szarrTmpFloat);

	pRet->bFlipX = GetPrivateProfileIntA(cszpSelCharacterDesc, "bFileX", THFALSE, cszpFilename);
	pRet->bFlipY = GetPrivateProfileIntA(cszpSelCharacterDesc, "bFileY", THFALSE, cszpFilename);
	pRet->nHP = GetPrivateProfileIntA(cszpSelCharacterDesc, "nHP", 100, cszpFilename);
	pRet->nMP = GetPrivateProfileIntA(cszpSelCharacterDesc, "nMP", 100, cszpFilename);
	pRet->nAttack = GetPrivateProfileIntA(cszpSelCharacterDesc, "nAttack", 10, cszpFilename);
	pRet->nAttackCD = GetPrivateProfileIntA(cszpSelCharacterDesc, "nAttackCD", 2, cszpFilename);
	pRet->nAttackRadius = GetPrivateProfileIntA(cszpSelCharacterDesc, "nAttackRadius", 35, cszpFilename);
	pRet->nDefense = GetPrivateProfileIntA(cszpSelCharacterDesc, "nDefense", 10, cszpFilename);
	pRet->nCDResurrection = GetPrivateProfileIntA(cszpSelCharacterDesc, "nCDResurrection", 15, cszpFilename);
	pRet->nSupportDuration = GetPrivateProfileIntA(cszpSelCharacterDesc, "nSupportDuration", 15, cszpFilename);
	pRet->emMoveSpeed = (enum THEM_CHARARCTERLEVEL_MOVESPEED)GetPrivateProfileIntA(cszpSelCharacterDesc, "emMoveSpeed", 2, cszpFilename);
	pRet->emCharacterType = (enum THEM_CHARACTER_TYPE)GetPrivateProfileIntA(cszpSelCharacterDesc, "emCharacterType", 0, cszpFilename);
	pRet->emMaxLevel = (enum THEM_CHARACTER_LEVEL)GetPrivateProfileIntA(cszpSelCharacterDesc, "emMaxLevel", 1, cszpFilename);

	/* 加载 CHARACTER_ANI_MAP */
	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniStandby", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniStandby, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniMoveTransverse", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniMoveTransverse, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniMoveUp", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniMoveUp, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniMoveDown", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniMoveDown, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniAttack", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniAttack, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniSkill", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniSkill, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniDie", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniDie, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniOpenTheDoor", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniOpenTheDoor, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelAniMap, "szarrAniCloseTheDoor", "NA", szarrTmpStr, MAX_PATH, cszpFilename);
	strcpy_s(pAniMap->szarrAniCloseTheDoor, strlen(szarrTmpStr) + 1, szarrTmpStr);

	pRet->ptAniMap = pAniMap;
	*ppRet = pRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCharacterLoadHandler::getCharacterAniDescFromIni(const char* cszpFilename, CHARACTER_ANI_DESC_PTR* ppRet)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	const char* cszpSel = "CHARACTER_ANIMATION_DESC";
	char sztmpNum[32] = { 0 };
	CHARACTER_ANI_DESC_PTR pRet = THMALLOC(CHARACTER_ANI_DESC, sizeof(CHARACTER_ANI_DESC));
	TH_PROCESS_ERROR(pRet);

	pRet->nFrameAniCount = GetPrivateProfileIntA(cszpSel, "unFrameAniCount", 1, cszpFilename);
	pRet->nFrameAniBegin = GetPrivateProfileIntA(cszpSel, "unFrameAniBegin", 1, cszpFilename);
	pRet->nFrameAniEnd = GetPrivateProfileIntA(cszpSel, "unFrameAniEnd", 1, cszpFilename);
	pRet->bResFirstFrame = GetPrivateProfileIntA(cszpSel, "bResFirstFrame", THFALSE, cszpFilename);
	GetPrivateProfileStringA(cszpSel, "fDelayPerUnit", "0.5", sztmpNum, 32, cszpFilename);
	pRet->fDelayPerUnit = atof(sztmpNum);

	GetPrivateProfileStringA(cszpSel, "nLoops", "1", sztmpNum, 32, cszpFilename);
	pRet->nLoops = atoi(sztmpNum);
	
	GetPrivateProfileStringA(cszpSel, "szBasicFrameAniPlistPath", "NA", pRet->szarrBasicFrameAniPlistPath, MAX_PATH, cszpFilename);
	GetPrivateProfileStringA(cszpSel, "szPlistPngPath", "NA", pRet->szarrPlistPngPath, 64, cszpFilename);
	GetPrivateProfileStringA(cszpSel, "szAniDesc", "NA", pRet->szarrAniDesc, 32, cszpFilename);

	*ppRet = pRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCharacterLoadHandler::getDefTowerDescFromIni(const char* cszpFilename, DEFTOWER_DESC_PTR* ppRet)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	const char* cszpSel = "DEFTOWER_DESC";
	DEFTOWER_DESC_PTR pRet = THMALLOC(DEFTOWER_DESC, sizeof(DEFTOWER_DESC));
	TH_PROCESS_ERROR(pRet);

	pRet->sCurWarriors = 0;
	pRet->sMaxBullets = GetPrivateProfileIntA(cszpSel, "sMaxBullets", 1, cszpFilename);
	pRet->sMaxWarriors = GetPrivateProfileIntA(cszpSel, "sMaxWarriors", THMAX_DEFTOWER_TARLEVEL_WARRIORS, cszpFilename);
	if (THMAX_DEFTOWER_TARLEVEL_WARRIORS < pRet->sMaxWarriors)
	{
		pRet->sMaxWarriors = THMAX_DEFTOWER_TARLEVEL_WARRIORS;
	}
	pRet->sSummonWarriorsCD = GetPrivateProfileIntA(cszpSel, "sSummonWarriorsCD", 10, cszpFilename);
	pRet->emBulletType = (enum THEM_BULLET_TYPE)GetPrivateProfileIntA(cszpSel, "nBulletType", THEM_BULLET_TYPE::SHOOTCHAT_TRACKING, cszpFilename);

	*ppRet = pRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThCharacterLoadHandler::uninitCharacterDesc(CHARACTER_DESC_PTR p)
{
	THFREE(p->ptAniMap);
	THFREE(p);
	return;
}

void CThCharacterLoadHandler::uninitCharacterAniDesc(CHARACTER_ANI_DESC_PTR p)
{
	THFREE(p);
	return;
}

void CThCharacterLoadHandler::uninitDefTowerDesc(DEFTOWER_DESC_PTR p)
{
	THFREE(p);
	return;
}
