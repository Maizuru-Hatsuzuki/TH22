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
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::getInstance();
	SpriteFrame* pSpFrameExists = NULL;
	char szarrSpFrame[64] = { 0 };
	char szarrActiveTex[64] = { 0 };
	EventListenerMouse* pMouse = EventListenerMouse::create();

	m_ptSubSoilStatus = NULL;
	m_pDefTower = NULL;
	m_pLoading = NULL;
	m_ptConstruction = NULL;
	m_pSpLoading = NULL;
	m_pSpLoadingBg = NULL;
	m_emDefTowerProfessionalType = THEM_DEFTOWER_TYPE::DEFTOWERTYPE_UNKNOW;

	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szpSubsoilCharacterDescPath, &ptCharacterDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getSubsoilFromIni(szpSubsoilCharacterDescPath, &m_ptSubSoilStatus);
	TH_PROCESS_ERROR(bFnRet);
	m_ptSubSoilStatus->fFacingEnemyAngle = cfFacingEnemyAngle;

	/* 创建地基纹理. */
	bFnRet = initCharacterWithPlist(ptCharacterDesc, &m_ptSubsoil);
	TH_PROCESS_ERROR(bFnRet);
	m_ptSubsoil->pSpCharacter->setAnchorPoint(Vec2(0.5, 0));

	sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nDefaultTexPlistPos);
	m_pSpFrDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pSpFrDefaultSubsoil);

	sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nHoverTexPlistPos);
	m_pSpFrHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
	TH_PROCESS_ERROR(m_pSpFrHoverSubsoil);
	

	if (0 != strcmp(THINI_DEFAULT_STR, m_ptSubSoilStatus->szarrActiveTex))
	{
		/* 判一下有没有特殊贴图, 创建建筑后地基纹理可能不在默认纹理中. */
		sprintf_s(szarrSpFrame, 64, "%s%d.png", m_ptSubSoilStatus->szarrActiveTex, m_ptSubSoilStatus->nActiveDefaultTexPlistPos);
		m_pSpFrActiveDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
		TH_PROCESS_ERROR(m_pSpFrActiveDefaultSubsoil);
		sprintf_s(szarrSpFrame, 64, "%s%d.png", m_ptSubSoilStatus->szarrActiveTex, m_ptSubSoilStatus->nActiveHoverTexPlistPos);
		m_pSpFrActiveHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
		TH_PROCESS_ERROR(m_pSpFrActiveHoverSubsoil);
	}
	else
	{
		sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nActiveDefaultTexPlistPos);
		m_pSpFrActiveDefaultSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
		TH_PROCESS_ERROR(m_pSpFrActiveDefaultSubsoil);
		sprintf_s(szarrSpFrame, 64, "%s%d.png", ptCharacterDesc->szarrSpriteTex, m_ptSubSoilStatus->nActiveHoverTexPlistPos);
		m_pSpFrActiveHoverSubsoil = pSpFrameCache->getSpriteFrameByName(szarrSpFrame);
		TH_PROCESS_ERROR(m_pSpFrActiveHoverSubsoil);
	}

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTowerSubsoil::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_ptSubsoil->pSpCharacter);

	scheduleUpdate();

	this->addChild(m_ptSubsoil->pSpCharacter, 0, "SP_DefTowerSubsoil");

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
	bRet = CthCcAnimation::getInstance()->createLoadingBar(m_pSpLoading, 1.f, 100, &tLoadingDesc, &pActionLoading, &m_pLoading);
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
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::getInstance();
	CHARACTER_DESC_PTR ptSubsoilDesc = NULL;
	char szarrConstructionIni[MAX_PATH] = { 0 };

	m_ptSubsoil->pSpCharacter->setSpriteFrame(m_pSpFrActiveDefaultSubsoil);
	
	CThDefTower::getTowerInfoArcher(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, NULL, NULL, NULL, NULL, NULL, szarrConstructionIni);
	TH_PROCESS_ERROR(0 != strcmp(szarrConstructionIni, "\0"));

	/* 创建建造中建筑纹理. */
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrConstructionIni, &ptSubsoilDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initCharacterWithPlist(ptSubsoilDesc, &m_ptConstruction);
	TH_PROCESS_ERROR(bFnRet);
	m_ptConstruction->pSpCharacter->setAnchorPoint(Vec2(0.5, 0));
	bFnRet = initDefTowerConstructionLoadingBar();
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(m_ptConstruction->pSpCharacter);
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerSubsoil::uninitDefTowerConstruction()
{
	this->removeChild(m_pSpLoadingBg);
	this->removeChild(m_pSpLoadingBg);
	this->removeChild(m_ptConstruction->pSpCharacter);
	this->removeChild(m_pLoading);
	return;
}

thBool CThDefTowerSubsoil::initDefTower(enum THEM_CHARACTER_LEVEL emLevel)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	char szarrLvPath[16] = { 0 };
	char szarrProfessional[MAX_PATH] = { 0 };
	char* arrszpAni[THMAX_ANI_COUNT] = { 0 };
	char* arrszpWarriors[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	short sAniSize = 0;
	short sWarriorsRetSize = 0;
	CHARACTER_DESC_PTR arrpChacWarrios[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	DEFTOWER_WARRIORS tWarrior = { arrpChacWarrios, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, 0 };

	switch (emLevel)
	{
	case CHARACTER_LEVEL_1:
		strcpy_s(szarrLvPath, strlen("LV1") + 1, "LV1");
		break;
	case CHARACTER_LEVEL_2:
		strcpy_s(szarrLvPath, strlen("LV2") + 1, "LV2");
		break;
	case CHARACTER_LEVEL_3:
		strcpy_s(szarrLvPath, strlen("LV3") + 1, "LV3");
		break;
	case CHARACTER_LEVEL_4:
		strcpy_s(szarrLvPath, strlen("LV4") + 1, "LV4");
		break;
	case CHARACTER_LEVEL_5:
		strcpy_s(szarrLvPath, strlen("LV5") + 1, "LV5");
		break;
	case CHARACTER_MAXLEVEL:
		break;
	default:
		break;
	}

	switch (m_emDefTowerProfessionalType)
	{
	case DEFTOWERTYPE_UNKNOW:
		break;

	case DEFTOWERTYPE_ARCHER:
		CThDefTower::getTowerInfoArcher(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, szarrProfessional, arrszpAni, &sAniSize, NULL, NULL, NULL);
		break;

	case DEFTOWERTYPE_WARRIORS:
		bFnRet = CThDefTower::getTowerInfoArcherWarriors(szarrProfessional, arrszpAni, &sAniSize, arrszpWarriors, &sWarriorsRetSize, NULL, szarrLvPath);
		TH_PROCESS_ERROR(bFnRet);

		for (short j = 0; j < sWarriorsRetSize; j++)
		{
			bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
				arrszpWarriors[j], &(tWarrior.arrpTowerWarriorsDesc[j])
			);
			TH_PROCESS_ERROR(bFnRet);
		}
		tWarrior.sSize = sWarriorsRetSize;
		break;

	case DEFTOWERTYPE_ARCHER_WARRIORS:
		break;

	default:
		break;
	}
	
	if (0 != strcmp("\0", szarrProfessional))
	{
		this->addChild(m_pDefTower);
		bFnRet = m_pDefTower->init(
			szarrProfessional,
			THEM_BULLET::BULLET_BUTTERFLY,
			arrszpAni,
			sAniSize,
			&tWarrior,
			m_ptSubSoilStatus->fFacingEnemyAngle
		);
		TH_PROCESS_ERROR(bFnRet);
		bRet = THTRUE;
	}
	else
	{
		CCLOG("Can not found szarrProfessional, check the m_emDefTowerProfessionalType setting.");
	}
	
Exit0:
	/* TODO: 路径的释放. */
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

void CThDefTowerSubsoil::setDefTowerProfessionalType(enum THEM_DEFTOWER_TYPE emType)
{
	m_emDefTowerProfessionalType = emType;
	return;
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
			m_pDefTower = new CThDefTower;
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
		if (NULL == m_pLoading)
		{
			m_ptSubsoil->pSpCharacter->setSpriteFrame(m_pSpFrHoverSubsoil);
		}
		else
		{
			m_ptSubsoil->pSpCharacter->setSpriteFrame(m_pSpFrActiveHoverSubsoil);
		}
	}
	else
	{
		if (NULL == m_pLoading)
		{
			m_ptSubsoil->pSpCharacter->setSpriteFrame(m_pSpFrDefaultSubsoil);
		}
		else
		{
			m_ptSubsoil->pSpCharacter->setSpriteFrame(m_pSpFrActiveDefaultSubsoil);
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
		initDefTower(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4);
	}
	return;
}
