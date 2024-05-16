/********************************************************
* Filename		: thCcCharacter.cpp
* Creator		: ac
* Date time		: 2024.04.14
* Description	: base character.
********************************************************/

#include "thBaseMacro.h"
#include "thCcAnimation.h"


CThBaseCharacterAction* CThBaseCharacterAction::m_pSelf;
CthCcCharacterLoadHandler* CthCcCharacterLoadHandler::m_pSelf;


CThBaseCharacter::CThBaseCharacter()
{
}

CThBaseCharacter::~CThBaseCharacter()
{
}

thBool CThBaseCharacter::initCharacter(std::string strSpFile, CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet, thBool bInitSp)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR ptCharFrame = THMALLOC(CHARACTER_FRAMEINFO, sizeof(CHARACTER_FRAMEINFO));
	TH_PROCESS_ERROR(ptCharFrame);
	
	if (THTRUE == bInitSp)
	{
		ptCharFrame->pSpCharacter = Sprite::create(strSpFile);
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

	_initCharacterDescInfo(pDesc, ptCharFrame);

	*ppRet = ptCharFrame;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThBaseCharacter::initCharacterWithPlist(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	SpriteFrame* pSpFrame = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::getInstance();
	CHARACTER_FRAMEINFO_PTR ptCharFrame = THMALLOC(CHARACTER_FRAMEINFO, sizeof(CHARACTER_FRAMEINFO));
	TH_PROCESS_ERROR(ptCharFrame);
	char szarrSp[128] = { 0 };

	if (0 != strcmp(pDesc->szarrDefaultTexPlistPos, "NA"))
	{
		sprintf_s(szarrSp, "%s.png", pDesc->szarrDefaultTexPlistPos);
	}
	else
	{
		sprintf_s(szarrSp, "%s%d.png", pDesc->szarrSpriteTex, pDesc->nDefaultTexPlistPos);
	}
	
	pSpFrame = pSpFrameCache->getSpriteFrameByName(szarrSp);
	TH_PROCESS_ERROR(pSpFrame);
	ptCharFrame->pSpCharacter = Sprite::createWithSpriteFrame(pSpFrame);
	TH_PROCESS_ERROR(ptCharFrame->pSpCharacter);

	ptCharFrame->pSpCharacter->setFlippedX(pDesc->bFlipX);
	ptCharFrame->pSpCharacter->setFlippedY(pDesc->bFlipY);
	TH_RUN_SUCCESS(pDesc->fScale, ptCharFrame->pSpCharacter->setScale(pDesc->fScale));
	ptCharFrame->pSpCharacter->setPositionX(pDesc->fPosX);
	ptCharFrame->pSpCharacter->setPositionY(pDesc->fPosY);

	_initCharacterDescInfo(pDesc, ptCharFrame);

	*ppRet = ptCharFrame;
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThBaseCharacter::_initCharacterDescInfo(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR ptCharFrame)
{
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
	ptCharFrame->emCurLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1;
	strcpy_s(ptCharFrame->szarrDesc, strlen(pDesc->szarrSpriteName) + 1, pDesc->szarrSpriteName);
	return;
}

thBool CThBaseCharacter::initCharacterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet) const
{
	thBool bRet = THFALSE;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::getInstance();
	TH_PROCESS_ERROR(pSpFrameCache);
	bRet = CthCcAnimation::getInstance()->createPlayAnimationWithPList(pAniDesc, ppRet);
	TH_PROCESS_ERROR(bRet);

	(*ppRet)->retain();
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThBaseCharacter::uninitCharacter(CHARACTER_FRAMEINFO_PTR pCharater)
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

void CThBaseCharacter::getCharacterMoveSpeed(const float cfPlayerX, const float cfPlayerY, const float cfDstX, const float cfDstY, enum THEM_CHARARCTERLEVEL_MOVESPEED emSpeed, float* pfRet)
{
	float fRet = 0.f;
	float fSpeed = 80.f;
	const float cfMoveDis = sqrt(pow(cfDstX - cfPlayerX, 2) + pow(cfDstY - cfPlayerY, 2));

	switch (emSpeed)
	{
	case THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_HIGH:
		fSpeed = 90.f;
		break;

	case THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_HIGHEX:
		fSpeed = 100.f;
		break;

	default:
		break;
	}

	fRet = cfMoveDis / fSpeed;
	*pfRet = fRet;
	return;
}

thBool CThBaseCharacter::getCharacterAnimateFrameInfo(CHARACTER_ANI_DESC_PTR pAniDesc, CHARACTER_ANI_FRAMEINFO_PTR pRet) const
{
	thBool bRet = THFALSE;
	pRet->pAnimate = NULL;
	bRet = CthCcAnimation::getInstance()->createPlayAnimationWithPList(pAniDesc, &pRet->pAnimate);
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


CthCcCharacterLoadHandler::CthCcCharacterLoadHandler()
{}

CthCcCharacterLoadHandler::~CthCcCharacterLoadHandler()
{}

CthCcCharacterLoadHandler* CthCcCharacterLoadHandler::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = THNEW_CLASS(CthCcCharacterLoadHandler);
	}
	return m_pSelf;
}

thBool CthCcCharacterLoadHandler::getCharaterDescFromIni(const char* cszpFilename, CHARACTER_DESC_PTR* ppRet)
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
	GetPrivateProfileStringA(cszpSelCharacterDesc, "szarrSpriteName", "NA", szarrTmpStr, 64, cszpFilename);
	strcpy_s(pRet->szarrSpriteName, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "szarrSpriteTex", "NA", szarrTmpStr, 64, cszpFilename);
	strcpy_s(pRet->szarrSpriteTex, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "szarrDefaultTexPlistPos", "NA", szarrTmpStr, 64, cszpFilename);
	strcpy_s(pRet->szarrDefaultTexPlistPos, strlen(szarrTmpStr) + 1, szarrTmpStr);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "fPosX", "0.0", szarrTmpFloat, 32, cszpFilename);
	pRet->fPosX = (float)atof(szarrTmpFloat);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "fPosY", "0.0", szarrTmpFloat, 32, cszpFilename);
	pRet->fPosY = (float)atof(szarrTmpFloat);

	GetPrivateProfileStringA(cszpSelCharacterDesc, "fScale", "0.0", szarrTmpFloat, 32, cszpFilename);
	pRet->fScale = (float)atof(szarrTmpFloat);

	pRet->bFlipX = GetPrivateProfileIntA(cszpSelCharacterDesc, "bFileX", THFALSE, cszpFilename);
	pRet->bFlipY = GetPrivateProfileIntA(cszpSelCharacterDesc, "bFileY", THFALSE, cszpFilename);
	pRet->nDefaultTexPlistPos = GetPrivateProfileIntA(cszpSelCharacterDesc, "nDefaultTexPlistPos", 0, cszpFilename);
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

thBool CthCcCharacterLoadHandler::getCharacterAniDescFromIni(const char* cszpFilename, CHARACTER_ANI_DESC_PTR* ppRet)
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

thBool CthCcCharacterLoadHandler::getDefTowerDescFromIni(const char* cszpFilename, DEFTOWER_DESC_PTR* ppRet)
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
	pRet->nDefTowerProfessional = GetPrivateProfileIntA(cszpSel, "nDefTowerProfessional", 0, cszpFilename);
	pRet->emBulletType = (enum THEM_BULLET_TYPE)GetPrivateProfileIntA(cszpSel, "nBulletType", THEM_BULLET_TYPE::SHOOTCHAT_TRACKING, cszpFilename);

	*ppRet = pRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CthCcCharacterLoadHandler::getSubsoilFromIni(const char* cszpFilename, SUBSOIL_DESC_PTR* ppRet)
{
	thBool bRet = THFALSE;
	const char* cszpSel = "SUBSOIL_DESC";
	SUBSOIL_DESC_PTR pRet = THMALLOC(SUBSOIL_DESC, sizeof(SUBSOIL_DESC));
	TH_PROCESS_ERROR(pRet);

	pRet->nDefaultTexPlistPos = GetPrivateProfileIntA(cszpSel, "nDefaultTexPlistPos", 1, cszpFilename);
	pRet->nHoverTexPlistPos = GetPrivateProfileIntA(cszpSel, "nHoverTexPlistPos", 1, cszpFilename);
	pRet->nActiveDefaultTexPlistPos = GetPrivateProfileIntA(cszpSel, "nActiveDefaultTexPlistPos", 1, cszpFilename);
	pRet->nActiveHoverTexPlistPos = GetPrivateProfileIntA(cszpSel, "nActiveHoverTexPlistPos", 1, cszpFilename);

	*ppRet = pRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CthCcCharacterLoadHandler::uninitCharacterDesc(CHARACTER_DESC_PTR p)
{
	THFREE(p->ptAniMap);
	THFREE(p);
	return;
}

void CthCcCharacterLoadHandler::uninitCharacterAniDesc(CHARACTER_ANI_DESC_PTR p)
{
	THFREE(p);
	return;
}

void CthCcCharacterLoadHandler::uninitDefTowerDesc(DEFTOWER_DESC_PTR p)
{
	THFREE(p);
	return;
}

void CthCcCharacterLoadHandler::uninitSubsoilDesc(SUBSOIL_DESC_PTR p)
{
	THFREE(p);
	return;
}
