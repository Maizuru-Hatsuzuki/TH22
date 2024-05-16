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

thBool CThDefTowerSubsoil::init(char* szpSubsoilCharacterDescPath, const float cfFacingEnemyAngle)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_DESC_PTR ptCharacterDesc = NULL;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	SpriteFrame* pSpFrameExists = NULL;
	char szarrSpFrame[64] = { 0 };
	char szarrSpPlistTex[MAX_PATH] = { 0 };
	EventListenerMouse* pMouse = EventListenerMouse::create();

	m_ptSubSoilStatus = NULL;
	m_pDefTower = NULL;
	m_pLoading = NULL;
	m_pSpFrameCons = NULL;
	m_pSpConstruction = NULL;
	m_pSpLoading = NULL;
	m_pSpLoadingBg = NULL;

	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szpSubsoilCharacterDescPath, &ptCharacterDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getSubsoilFromIni(szpSubsoilCharacterDescPath, &m_ptSubSoilStatus);
	TH_PROCESS_ERROR(bFnRet);
	m_ptSubSoilStatus->fFacingEnemyAngle = cfFacingEnemyAngle;

	bFnRet = initCharacterWithPlist(ptCharacterDesc, &m_ptSubsoil);
	TH_PROCESS_ERROR(bFnRet);

	CTHCcBaseHandler::getInstance()->splitFileSuffix(szpSubsoilCharacterDescPath, ".ini", szarrSpPlistTex);

	sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nHoverTexPlistPos);
	m_pHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pHoverSubsoil);

	sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nActiveHoverTexPlistPos);
	m_pActiveHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pActiveHoverSubsoil);

	sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nDefaultTexPlistPos);
	m_pDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pDefaultSubsoil);

	sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nActiveDefaultTexPlistPos);
	m_pActiveDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pActiveDefaultSubsoil);

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_ptSubsoil->pSpCharacter);

	scheduleUpdate();

	this->addChild(m_ptSubsoil->pSpCharacter);
	bRet = THTRUE;
Exit0:
	CthCcCharacterLoadHandler::getInstance()->uninitCharacterDesc(ptCharacterDesc);
	return bRet;
}

void CThDefTowerSubsoil::uninit()
{
	unscheduleUpdate();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	CthCcCharacterLoadHandler::getInstance()->uninitSubsoilDesc(m_ptSubSoilStatus);
	return;
}

thBool CThDefTowerSubsoil::initDefTowerConstructionLoadingBar()
{
	thBool bRet = THFALSE;
	ProgressTo* pActionLoading = NULL;
	CHARACTER_DESC tLoadingDesc = {};
	tLoadingDesc.fPosX = m_ptSubsoil->pSpCharacter->getPositionX();
	tLoadingDesc.fPosY = m_ptSubsoil->pSpCharacter->getPositionY() + 50;
	tLoadingDesc.fScale = m_ptSubsoil->pSpCharacter->getScale();

	/* 创建进度条背景. */
	m_pSpLoadingBg = Sprite::createWithSpriteFrameName("Tower Construction Material7.png");
	TH_PROCESS_ERROR(m_pSpLoadingBg);
	m_pSpLoadingBg->setPosition(m_ptSubsoil->pSpCharacter->getPositionX(), m_ptSubsoil->pSpCharacter->getPositionY() + 50);
	m_pSpLoadingBg->setScale(m_ptSubsoil->pSpCharacter->getScale());

	/* 创建进度条 */
	m_pSpLoading = Sprite::createWithSpriteFrameName("Tower Construction Material8.png");
	TH_PROCESS_ERROR(m_pSpLoading);
	bRet = CthCcAnimation::getInstance()->createLoadingBar(m_pSpLoading, 2.f, 100, &tLoadingDesc, &pActionLoading, &m_pLoading);
	TH_PROCESS_ERROR(bRet);

	this->addChild(m_pSpLoadingBg);
	this->addChild(m_pLoading);

	m_pLoading->runAction(pActionLoading);
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerSubsoil::initDefTowerConstruction()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();

	char szarrConstructionIni[MAX_PATH] = { 0 };
	char szarrTexPath[MAX_PATH] = { 0 };
	char szarrPlist[MAX_PATH] = { 0 };
	char szarrPlistPng[MAX_PATH] = { 0 };
	char szarrSprite[64] = { 0 };
	int nTexPos = 0;

	CThDefTower::getTowerInfoArcher(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, NULL, NULL, NULL, NULL, NULL, szarrConstructionIni);
	TH_PROCESS_ERROR(0 != strcmp(szarrConstructionIni, "\0"));

	GetPrivateProfileStringA("TEX", "szPlistPath", THINI_DEFAULT_STR, szarrTexPath, MAX_PATH, szarrConstructionIni);
	TH_PROCESS_SUCCESS(0 == strcmp(THINI_DEFAULT_STR, szarrTexPath));
	GetPrivateProfileStringA("TEX", "szSpriteName", THINI_DEFAULT_STR, szarrSprite, 64, szarrConstructionIni);
	TH_PROCESS_SUCCESS(0 == strcmp(THINI_DEFAULT_STR, szarrSprite));
	nTexPos = GetPrivateProfileIntA("TEX", "nPos", 0, szarrConstructionIni);

	sprintf_s(szarrPlist, "%s.plist", szarrTexPath);
	sprintf_s(szarrPlistPng, "%s.png", szarrTexPath);
	sprintf_s(szarrSprite, "%s%d.png", szarrSprite, nTexPos);

	m_pSpFrameCons = pSpFrameCache->getSpriteFrameByName(szarrSprite);
	m_pSpConstruction = Sprite::createWithSpriteFrame(m_pSpFrameCons);
	TH_PROCESS_ERROR(m_pSpConstruction);

	/* 创建建造中建筑纹理. */
	m_pSpConstruction->setPositionX(m_ptSubsoil->pSpCharacter->getPositionX());
	m_pSpConstruction->setPositionY(m_ptSubsoil->pSpCharacter->getPositionY());
	m_pSpConstruction->setScale(m_ptSubsoil->pSpCharacter->getScale());
	m_ptSubsoil->pSpCharacter->setDisplayFrame(m_pActiveDefaultSubsoil);

	bFnRet = initDefTowerConstructionLoadingBar();
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(m_pSpConstruction);
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerSubsoil::uninitDefTowerConstruction()
{
	this->removeChild(m_pSpLoadingBg);
	this->removeChild(m_pSpLoadingBg);
	this->removeChild(m_pSpConstruction);
	this->removeChild(m_pLoading);
	return;
}

thBool CThDefTowerSubsoil::initDefTower()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	/* debug use. init archer */
	char szpArcher[MAX_PATH] = { 0 };
	char* arrszpAni[9] = { 0 };
	char* arrszpWarriors[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	short sAniSize = 0;
	short sWarriorsRetSize = 0;
	CHARACTER_DESC_PTR arrpChacWarrios[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	DEFTOWER_WARRIORS tWarrior = { arrpChacWarrios, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, sWarriorsRetSize };
	/* loading banner. */
	
	m_pDefTower = new CThDefTower;
	CThDefTower::getTowerInfoArcher(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, szpArcher, arrszpAni, &sAniSize, arrszpWarriors, &sWarriorsRetSize, NULL);

	for ( short j = 0; j < sWarriorsRetSize; j++ )
	{
		bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
			arrszpWarriors[j], &(tWarrior.arrpTowerWarriorsDesc[j])
		);
		TH_PROCESS_ERROR(bFnRet);
	}



	/*
		1. 进度条播放
		2. init塔加入队列
		3. update里循环如果播放100就销毁建造中和进度条贴图
		4. 执行队列任务
	*/
	
	bFnRet = m_pDefTower->init(
		szpArcher,
		"data\\CharacterConfig\\SaigyoSakura\\ChacBullet.ini",
		"image\\sprite\\kr\\smallstar.png",
		arrszpAni,
		sAniSize,
		&tWarrior
	);
	TH_PROCESS_ERROR(bFnRet);
	
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
			initDefTowerConstruction();
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

void CThDefTowerSubsoil::update(float dt)
{
	if (NULL != m_pLoading && 100.f == m_pLoading->getPercentage())
	{
		uninitDefTowerConstruction();
		initDefTower();
	}
	return;
}
