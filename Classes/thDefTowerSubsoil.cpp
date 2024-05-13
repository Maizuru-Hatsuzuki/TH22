/********************************************************
* Filename		: thDefTowerSubsoil.cpp
* Creator		: ac
* Date time		: 2024.05.09
* Description	: def tower subsoil.
********************************************************/


#include "thCcDefTower.h"
#include "thCcAnimation.h"
#include "thBaseMacro.h"


CThDefTowerSubsoil::CThDefTowerSubsoil()
{
}

CThDefTowerSubsoil::~CThDefTowerSubsoil()
{
}

thBool CThDefTowerSubsoil::init(const char* cszpSubsoilCharacterDescPath, const float cfFacingEnemyAngle)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_DESC_PTR ptCharacterDesc = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	SpriteFrame* pSpFrameExists = NULL;
	char szarrSpFrame[64] = { 0 };
	char szarrSpPlistPath[MAX_PATH] = { 0 };
	char szarrSpPlistPngPath[MAX_PATH] = { 0 };
	EventListenerMouse* pMouse = EventListenerMouse::create();

	m_ptSubSoilStatus = NULL;
	m_pDefTower = NULL;

	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(cszpSubsoilCharacterDescPath, &ptCharacterDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getSubsoilFromIni(cszpSubsoilCharacterDescPath, &m_ptSubSoilStatus);
	TH_PROCESS_ERROR(bFnRet);
	m_ptSubSoilStatus->fFacingEnemyAngle = cfFacingEnemyAngle;

	sprintf_s(szarrSpFrame, "%s0.png", ptCharacterDesc->szarrSpritePlistTex);
	pSpFrameExists = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	if (NULL == pSpFrameExists)
	{
		sprintf_s(szarrSpPlistPath, "%s.plist", ptCharacterDesc->szarrSpritePlistPath);
		sprintf_s(szarrSpPlistPngPath, "%s.png", ptCharacterDesc->szarrSpritePlistPath);
		pSpFrameCache->addSpriteFramesWithFile(szarrSpPlistPath, szarrSpPlistPngPath);
	}

	bFnRet = initCharacterWithPlist(ptCharacterDesc->szarrSpritePlistTex, m_ptSubSoilStatus->nDefaultTexPlistPos, ptCharacterDesc, &m_ptSubsoil);
	TH_PROCESS_ERROR(bFnRet);

	sprintf_s(szarrSpFrame, "%s%d.png", ptCharacterDesc->szarrSpritePlistTex, m_ptSubSoilStatus->nHoverTexPlistPos);
	m_pHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pHoverSubsoil);

	sprintf_s(szarrSpFrame, "%s%d.png", ptCharacterDesc->szarrSpritePlistTex, m_ptSubSoilStatus->nActiveHoverTexPlistPos);
	m_pActiveHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pActiveHoverSubsoil);

	sprintf_s(szarrSpFrame, "%s%d.png", ptCharacterDesc->szarrSpritePlistTex, m_ptSubSoilStatus->nDefaultTexPlistPos);
	m_pDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pDefaultSubsoil);

	sprintf_s(szarrSpFrame, "%s%d.png", ptCharacterDesc->szarrSpritePlistTex, m_ptSubSoilStatus->nActiveDefaultTexPlistPos);
	m_pActiveDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pActiveDefaultSubsoil);

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_ptSubsoil->pSpCharacter);

	this->addChild(m_ptSubsoil->pSpCharacter);
	bRet = THTRUE;
Exit0:
	CthCcCharacterLoadHandler::getInstance()->uninitCharacterDesc(ptCharacterDesc);
	return bRet;
}

void CThDefTowerSubsoil::uninit()
{
	CthCcCharacterLoadHandler::getInstance()->uninitSubsoilDesc(m_ptSubSoilStatus);
	return;
}

thBool CThDefTowerSubsoil::initDefTowerConstruction(const char* cszpConstructionIni)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	SpriteFrame* pSpFrameCons = NULL;
	Sprite* pSp = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	char szarrTexPath[MAX_PATH] = { 0 };
	char szarrPlist[MAX_PATH] = { 0 };
	char szarrPlistPng[MAX_PATH] = { 0 };
	char szarrSprite[64] = { 0 };
	int nTexPos = 0;

	GetPrivateProfileStringA("TEX", "szPlistPath", THINI_DEFAULT_STR, szarrTexPath, MAX_PATH, cszpConstructionIni);
	TH_PROCESS_SUCCESS(0 == strcmp(THINI_DEFAULT_STR, szarrTexPath));
	GetPrivateProfileStringA("TEX", "szSpriteName", THINI_DEFAULT_STR, szarrSprite, 64, cszpConstructionIni);
	TH_PROCESS_SUCCESS(0 == strcmp(THINI_DEFAULT_STR, szarrSprite));
	nTexPos = GetPrivateProfileIntA("TEX", "nPos", 0, cszpConstructionIni);

	sprintf_s(szarrPlist, "%s.plist", szarrTexPath);
	sprintf_s(szarrPlistPng, "%s.png", szarrTexPath);
	sprintf_s(szarrSprite, "%s%d.png", szarrSprite, nTexPos);

	pSpFrameCache->addSpriteFramesWithFile(szarrPlist, szarrPlistPng);
	pSpFrameCons = pSpFrameCache->getSpriteFrameByName(szarrSprite);
	pSp = Sprite::createWithSpriteFrame(pSpFrameCons);
	TH_PROCESS_ERROR(pSp);

	pSp->setPositionX(m_ptSubsoil->pSpCharacter->getPositionX());
	pSp->setPositionY(m_ptSubsoil->pSpCharacter->getPositionY());
	pSp->setScale(m_ptSubsoil->pSpCharacter->getScale());

	m_ptSubsoil->pSpCharacter->setDisplayFrame(m_pActiveDefaultSubsoil);
	
	this->addChild(pSp);
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerSubsoil::initDefTower()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	/* debug use. init archer */
	char szpArcher[MAX_PATH] = { 0 };
	char* arrszpAni[9] = { 0 };
	char* arrszpWarriors[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	char szarrConstruction[MAX_PATH] = { 0 };
	short sAniSize = 0;
	short sWarriorsRetSize = 0;
	CHARACTER_DESC_PTR arrpChacWarrios[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	DEFTOWER_WARRIORS tWarrior = { arrpChacWarrios, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, sWarriorsRetSize };
	/* loading banner. */
	Sprite* pSpLoading = NULL;
	Sprite* pSpLoadingBg = NULL;
	ProgressTo* pLoading = ProgressTo::create(1.5f, 100);
	ProgressTimer* pLoadingPic = NULL;
	m_pDefTower = new CThDefTower;

	CThDefTower::getTowerInfoArcher(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, szpArcher, arrszpAni, &sAniSize, arrszpWarriors, &sWarriorsRetSize, szarrConstruction);

	for ( short j = 0; j < sWarriorsRetSize; j++ )
	{
		bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
			arrszpWarriors[j], &(tWarrior.arrpTowerWarriorsDesc[j])
		);
		TH_PROCESS_ERROR(bFnRet);
	}

	bFnRet = initDefTowerConstruction(szarrConstruction);
	TH_PROCESS_ERROR(bFnRet);

	pSpLoadingBg = Sprite::createWithSpriteFrameName("Tower Construction Material7.png");
	pSpLoadingBg->setPosition(m_ptSubsoil->pSpCharacter->getPositionX(), m_ptSubsoil->pSpCharacter->getPositionY() + 50);
	pSpLoading = Sprite::createWithSpriteFrameName("Tower Construction Material8.png");
	TH_PROCESS_ERROR(pSpLoading);
	pLoadingPic = ProgressTimer::create(pSpLoading);
	TH_PROCESS_ERROR(pLoadingPic);
	pLoadingPic->setType(kCCProgressTimerTypeBar);
	pLoadingPic->setBarChangeRate(ccp(1, 0));
	pLoadingPic->setMidpoint(ccp(0, 0));
	pLoadingPic->setPosition(m_ptSubsoil->pSpCharacter->getPositionX() , m_ptSubsoil->pSpCharacter->getPositionY() + 50);
	this->addChild(pSpLoadingBg);
	this->addChild(pLoadingPic);

	pLoadingPic->runAction(pLoading);
	
	pLoadingPic->getPercentage();
	/*
		1. 进度条播放
		2. init塔加入队列
		3. update里循环如果播放100就销毁建造中和进度条贴图
		4. 执行队列任务
	*/
	/*
	bFnRet = m_pDefTower->init(
		szpArcher,
		"data\\CharacterConfig\\SaigyoSakura\\ChacBullet.ini",
		arrszpAni,
		sAniSize,
		&tWarrior
	);
	TH_PROCESS_ERROR(bFnRet);
	*/
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerSubsoil::uninitDefTower()
{

}

void CThDefTowerSubsoil::getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{

}

void CThDefTowerSubsoil::getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
{

}

void CThDefTowerSubsoil::onMouseUp(EventMouse* pEvent)
{
	thBool bRet = THFALSE;

	bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(pEvent->getLocationInView());
	if (bRet)
	{
		if (NULL == m_pDefTower)
		{
			initDefTower();
		}
		else
		{
		}
	}

	return;
}

void CThDefTowerSubsoil::onMouseDown(EventMouse* pEvent)
{

}

void CThDefTowerSubsoil::onMouseMove(EventMouse* pEvent)
{
	thBool bRet = THFALSE;

	bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(pEvent->getLocationInView());
	if (bRet)
	{
		if (NULL == m_pDefTower)
		{
			m_ptSubsoil->pSpCharacter->setDisplayFrame(m_pHoverSubsoil);
		}
		else
		{
			m_ptSubsoil->pSpCharacter->setDisplayFrame(m_pActiveHoverSubsoil);
		}
	}
	else
	{
		if (NULL == m_pDefTower)
		{
			m_ptSubsoil->pSpCharacter->setDisplayFrame(m_pDefaultSubsoil);
		}
		else
		{
			m_ptSubsoil->pSpCharacter->setDisplayFrame(m_pActiveDefaultSubsoil);
		}
	}

	return;
}

thBool CThDefTowerSubsoil::globalMonitoring()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;



	bRet = THTRUE;
Exit0:
	return bRet;
}