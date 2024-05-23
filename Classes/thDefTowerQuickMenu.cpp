/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.05.23
* Description	: def tower quick menu.
* *******************************************************/


#include "thCcDefTower.h"
#include "thBaseMacro.h"


CThDefTowerQuickMenu* CThDefTowerQuickMenu::m_pSelf;


CThDefTowerQuickMenu::CThDefTowerQuickMenu()
{
}

CThDefTowerQuickMenu::~CThDefTowerQuickMenu()
{
}

CThDefTowerQuickMenu* CThDefTowerQuickMenu::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = THNEW_CLASS(CThDefTowerQuickMenu);
	}
	return m_pSelf;
}

thBool CThDefTowerQuickMenu::createBasicQm(const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm)
{
	thBool bRet = THFALSE;
	CHARACTER_DESC tQuickMenuBg = { 0 };
	CHARACTER_DESC tCommandMovement = { 0 };
	CHARACTER_FRAMEINFO_PTR ptChacFrameQuickMenuBg = NULL;
	ScaleTo* pAcQuickMenuBgScale = NULL;

	if (NULL != ptDefTowerQm)
	{
		pAcQuickMenuBgScale = ScaleTo::create(0.1f, cfTagScale, cfTagScale);
		strcpy_s(tQuickMenuBg.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
		strcpy_s(tQuickMenuBg.szarrSpriteTex, 64, "Quickmenu Material_");
		strcpy_s(tQuickMenuBg.szarrSpriteName, 64, "Quickmenu Material bg");
		tQuickMenuBg.nDefaultTexPlistPos = 96;
		tQuickMenuBg.fPosX = cfX;
		tQuickMenuBg.fPosY = cfY;
		tQuickMenuBg.fScale = 0.4;

		bRet = initCharacterWithPlist(&tQuickMenuBg, &ptChacFrameQuickMenuBg);
		TH_PROCESS_ERROR(bRet);
		ptChacFrameQuickMenuBg->pSpCharacter->setAnchorPoint(Vec2(0.5, 0.5));
		ptChacFrameQuickMenuBg->pSpCharacter->runAction(pAcQuickMenuBgScale);

		ptDefTowerQm->pBg = ptChacFrameQuickMenuBg;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::destoryBasicQm(DEFTOWER_QUICKMENU_PTR ptDefTowerQm)
{
	ptDefTowerQm->pBg->pSpCharacter->removeFromParentAndCleanup(THTRUE);
	this->removeChild(ptDefTowerQm->pBg->pSpCharacter);
	THFREE(ptDefTowerQm->pBg);
}

thBool CThDefTowerQuickMenu::createQmWarriorLevel4(const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm)
{
	thBool bRet = THFALSE;
	
	bRet = createBasicQm(cfX, cfY, cfTagScale, ptDefTowerQm);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::destoryQmWarriorLevel4(DEFTOWER_QUICKMENU_PTR ptDefTowerQm)
{
	thBool bRet = THFALSE;


	bRet = THTRUE;
Exit0:
	return bRet;
}
