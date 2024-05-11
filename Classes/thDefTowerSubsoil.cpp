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

thBool CThDefTowerSubsoil::initDefTower()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	/* debug use. init archer */
	char* szpArcher = "";
	char* arrpAni[] = { 0 };
	char* arrpWarriors[] = { 0 };
	short sAniSize = 0;
	short sWarriorsRetSize = 0;
	CHARACTER_DESC_PTR arrpChacWarrios[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { 0 };
	DEFTOWER_WARRIORS tWarrior = { arrpChacWarrios, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, sWarriorsRetSize };
	CThDefTower* pHunterCabinObject = new CThDefTower;

	CThDefTower::getTowerInfoArcher(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, szpArcher, arrpAni, &sAniSize, arrpWarriors, &sWarriorsRetSize);

	for (short j = 0; j < sWarriorsRetSize; j++ )
	{
		bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
			arrpWarriors[j], &(tWarrior.arrpTowerWarriorsDesc[j])
		);
		TH_PROCESS_ERROR(bFnRet);
	}

	bFnRet = pHunterCabinObject->init(
		szpArcher,
		"data\\CharacterConfig\\SaigyoSakura\\ChacBullet.ini",
		arrpAni,
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