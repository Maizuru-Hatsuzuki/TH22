/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.05.23
* Description	: def tower quick menu.
* *******************************************************/


#include "thCcDefTower.h"
#include "thCcAnimation.h"
#include "thBaseMacro.h"


CThDefTowerQuickMenu* CThDefTowerQuickMenu::m_pSelf;


CThDefTowerQuickMenu::CThDefTowerQuickMenu()
{
	m_ptQm = NULL;
	m_pTaget = NULL;
	m_ptSellHoverBorder = NULL;
	m_ptMoveHoverBorder = NULL;
	m_ptSkillHoverBorder = NULL;
	m_ptMoveRangeHalo = NULL;
	m_ptMoveSelectingMouse = NULL;
	m_ptAniMovePosSelectingMouse = NULL;
	m_pMouse = NULL;
	m_emTagTowerType = THEM_DEFTOWER_TYPE::DEFTOWERTYPE_UNKNOW;
}

CThDefTowerQuickMenu::~CThDefTowerQuickMenu()
{
}

thBool CThDefTowerQuickMenu::init()
{
	thBool bRet = THFALSE;
	const char* cszpSpTex = "Quickmenu Material_";
	CHARACTER_DESC tSellHoverBorder = { 0 };
	CHARACTER_DESC tMoveHoverBorder = { 0 };
	CHARACTER_DESC tMoveRangeHalo = { 0 };
	CHARACTER_DESC tSkillHoverBorder = { 0 };

	/* 如果 m_pMouse 有值, 就代表已经初始化过了. */
	TH_PROCESS_SUCCESS(m_pMouse);
	m_pMouse = EventListenerMouse::create();

	if (NULL == m_ptSellHoverBorder)
	{
		strcpy_s(tSellHoverBorder.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
		strcpy_s(tSellHoverBorder.szarrSpriteTex, 64, cszpSpTex);
		strcpy_s(tSellHoverBorder.szarrSpriteName, 64, "Quickmenu sell border");
		tSellHoverBorder.nDefaultTexPlistPos = 34;

		bRet = initCharacterWithPlist(&tSellHoverBorder, &m_ptSellHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptSellHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptSellHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptMoveHoverBorder)
	{
		strcpy_s(tMoveHoverBorder.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
		strcpy_s(tMoveHoverBorder.szarrSpriteTex, 64, cszpSpTex);
		strcpy_s(tMoveHoverBorder.szarrSpriteName, 64, "Quickmenu move border");
		tMoveHoverBorder.nDefaultTexPlistPos = 63;

		bRet = initCharacterWithPlist(&tMoveHoverBorder, &m_ptMoveHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptSkillHoverBorder)
	{

	}
	if (NULL == m_ptMoveRangeHalo)
	{
		strcpy_s(tSkillHoverBorder.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
		strcpy_s(tSkillHoverBorder.szarrSpriteTex, 64, "HUD Material_");
		strcpy_s(tSkillHoverBorder.szarrSpriteName, 64, "warrior move range");
		tSkillHoverBorder.nDefaultTexPlistPos = 412;

		bRet = initCharacterWithPlist(&tSkillHoverBorder, &m_ptMoveRangeHalo);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveRangeHalo->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveRangeHalo->pSpCharacter);
	}

	bRet = initBasicAni();
	TH_PROCESS_ERROR(bRet);

	/* 创建悬浮和点击事件. */
	m_pMouse->onMouseUp = CC_CALLBACK_1(CThDefTowerQuickMenu::onMouseUp, this);
	m_pMouse->onMouseDown = CC_CALLBACK_1(CThDefTowerQuickMenu::onMouseDown, this);
	m_pMouse->onMouseMove = CC_CALLBACK_1(CThDefTowerQuickMenu::onMouseMove, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_pMouse, TH_EVENTPRIORITY_QUICKMENU);

	scheduleUpdate();
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::initBasicAni()
{
	thBool bRet = THFALSE;
	char szarrPathAniSelecting[MAX_PATH] = { 0 };
	char szarrPathAniSelected[MAX_PATH] = { 0 };
	char szarrPathAniSelectedError[MAX_PATH] = { 0 };
	CHARACTER_DESC tSelectingMouse = { 0 };

	/* 创建动画所需的精灵. */
	if (NULL == m_ptMoveSelectingMouse)
	{
		strcpy_s(tSelectingMouse.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
		strcpy_s(tSelectingMouse.szarrSpriteTex, 64, "posMouse");
		strcpy_s(tSelectingMouse.szarrSpriteName, 64, "sp_selectingMouse");
		tSelectingMouse.nDefaultTexPlistPos = 1;

		initCharacterWithPlist(&tSelectingMouse, &m_ptMoveSelectingMouse);
		m_ptMoveSelectingMouse->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveSelectingMouse->pSpCharacter);
	}
	
	/* 创建动画. */
	TH_GETWINRESPATH(szarrPathAniSelecting, "data\\AnimateConfig\\Basic\\AniMoveSelectingMouse.ini");

	if (NULL == m_ptAniMovePosSelectingMouse)
	{
		bRet = CthCcFrameByFrameAnimation::getInstance()->createAnimationWithPListIni(szarrPathAniSelecting, &m_ptAniMovePosSelectingMouse);
		TH_PROCESS_ERROR(bRet);
		TH_PROCESS_ERROR(m_ptMoveSelectingMouse);
		m_ptMoveSelectingMouse->pSpCharacter->runAction(m_ptAniMovePosSelectingMouse->pAnimate);
	}
	if (NULL == m_ptAniMovePosSelectedMouse)
	{
	}
	if (NULL == m_ptAniMovePosSelectedErrorMouse)
	{
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::uninit()
{
	THFREE(m_ptSellHoverBorder);
	THFREE(m_ptMoveHoverBorder);
	THFREE(m_ptSkillHoverBorder);
	THFREE(m_ptMoveRangeHalo);
	THFREE(m_ptMoveSelectingMouse);
	THFREE(m_ptAniMovePosSelectingMouse);

	unscheduleUpdate();
	CTHCcBaseHandler::getInstance()->setShowWinMouseCursor(THTRUE);
	Director::getInstance()->getEventDispatcher()->removeEventListener(m_pMouse);
	m_pMouse = NULL;
	return;
}

CThDefTowerQuickMenu* CThDefTowerQuickMenu::getInstance()
{
	thBool bRet = THFALSE;
	if (NULL == m_pSelf)
	{
		m_pSelf = THNEW_CLASS(CThDefTowerQuickMenu);
	}

	return m_pSelf;
}

thBool CThDefTowerQuickMenu::createBasicQm(const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm)
{
	/* cfX, cfY 用于定位基础位置. */
	thBool bRet = THFALSE;
	const char* cszpSpTex = "Quickmenu Material_";
	/* 分割 XY 值分为 24 份用于定位, 半边 12 份. */
	const int cnBgLayeredCnt = 12;
	float fBgLayerdX = 0.f;
	float fBgLayerdY = 0.f;
	CHARACTER_DESC tQuickMenuBg = { 0 };
	CHARACTER_DESC tCommandMovement = { 0 };
	CHARACTER_DESC tSellDefTower = { 0 };
	CHARACTER_FRAMEINFO_PTR ptChacFrameQuickMenuBg = NULL;
	CHARACTER_FRAMEINFO_PTR ptChacCommandMovement = NULL;
	CHARACTER_FRAMEINFO_PTR ptChacCommandMovementDisable = NULL;
	CHARACTER_FRAMEINFO_PTR ptChacSellDefTower = NULL;

	this->setScale(0.2f);
	this->setPositionX(cfX);
	this->setPositionY(cfY);

	TH_PROCESS_ERROR(NULL != ptDefTowerQm);
	m_ptQm = ptDefTowerQm;
	/* 快速菜单背景(圈). */
	strcpy_s(tQuickMenuBg.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
	strcpy_s(tQuickMenuBg.szarrSpriteTex, 64, cszpSpTex);
	strcpy_s(tQuickMenuBg.szarrSpriteName, 64, "Quickmenu bg");
	tQuickMenuBg.nDefaultTexPlistPos = 96;

	bRet = initCharacterWithPlist(&tQuickMenuBg, &ptChacFrameQuickMenuBg);
	TH_PROCESS_ERROR(bRet);
	ptDefTowerQm->pBg = ptChacFrameQuickMenuBg;

	/* 根据圈计算一些定位数据. */
	fBgLayerdX = ptChacFrameQuickMenuBg->pSpCharacter->getBoundingBox().size.width / 24;
	fBgLayerdY = ptChacFrameQuickMenuBg->pSpCharacter->getBoundingBox().size.height / 24;

	/* 移动按钮. */
	strcpy_s(tCommandMovement.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
	strcpy_s(tCommandMovement.szarrSpriteTex, 64, cszpSpTex);
	strcpy_s(tCommandMovement.szarrSpriteName, 64, "Quickmenu move");
	tCommandMovement.nDefaultTexPlistPos = 41;
	tCommandMovement.fPosX = fBgLayerdX * 8.5;
	tCommandMovement.fPosY = -fBgLayerdY * 7.5;
	tCommandMovement.fScale = 0.7f;
	
	bRet = initCharacterWithPlist(&tCommandMovement, &ptChacCommandMovement);
	TH_PROCESS_ERROR(bRet);

	/* 移动(禁用)按钮. */
	tCommandMovement.nDefaultTexPlistPos = 48;
	bRet = initCharacterWithPlist(&tCommandMovement, &ptChacCommandMovementDisable);
	TH_PROCESS_ERROR(bRet);

	if (THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS == m_emTagTowerType || THEM_DEFTOWER_TYPE::DEFTOWERTYPE_ARCHER_WARRIORS == m_emTagTowerType)
	{
		/* 可移动单位. */
		ptDefTowerQm->pCommandMovement = ptChacCommandMovement;
		ptDefTowerQm->pCommandMovement->pSpCharacter->setTag(THSP_FLAG_ENABLE);
	}
	else
	{
		ptDefTowerQm->pCommandMovement = ptChacCommandMovementDisable;
		ptDefTowerQm->pCommandMovement->pSpCharacter->setTag(THSP_FLAG_DISABLE);
	}

	/* 出售按钮. */
	strcpy_s(tSellDefTower.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
	strcpy_s(tSellDefTower.szarrSpriteTex, 64, cszpSpTex);
	strcpy_s(tSellDefTower.szarrSpriteName, 64, "Quickmenu sell");
	tSellDefTower.nDefaultTexPlistPos = 32;
	tSellDefTower.fPosY = -fBgLayerdY * 11;
	tSellDefTower.fScale = 0.75f;

	bRet = initCharacterWithPlist(&tSellDefTower, &ptChacSellDefTower);
	TH_PROCESS_ERROR(bRet);
	ptDefTowerQm->pSellTower = ptChacSellDefTower;

	this->addChild(ptDefTowerQm->pBg->pSpCharacter, 0, "QM_Bg");
	this->addChild(ptDefTowerQm->pCommandMovement->pSpCharacter, 0, "QM_CommandMove");
	this->addChild(ptDefTowerQm->pSellTower->pSpCharacter, 0, "QM_Sell");

	bRet = THTRUE;
Exit0:
	THFREE(ptDefTowerQm->pBg);
	return bRet;
}

void CThDefTowerQuickMenu::destoryBasicQm(DEFTOWER_QUICKMENU_PTR ptDefTowerQm)
{
	
	
}

thBool CThDefTowerQuickMenu::createQmWarriorLevel4(const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget)
{
	thBool bRet = THFALSE;
	m_pTaget = pTaget;
	m_emTagTowerType = m_pTaget->getDefTowerType();

	bRet = init();
	TH_PROCESS_ERROR(bRet);
	bRet = createBasicQm(cfX, cfY, cfTagScale, ptDefTowerQm);
	TH_PROCESS_ERROR(bRet);

	/* 设置一下范围圈的坐标. 防御塔锚点是 0.5, 0, 也同步修改一下. */
	m_ptMoveRangeHalo->pSpCharacter->setPosition(pTaget->getPosition());
	m_ptMoveRangeHalo->pSpCharacter->setScaleY(0.75f);
	m_ptMoveRangeHalo->pSpCharacter->setScaleX(1.05f);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::destoryQmWarriorLevel4(DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget)
{
	thBool bRet = THFALSE;
	
	uninit();
	this->removeAllChildrenWithCleanup(THTRUE);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::onMouseUp(EventMouse* pMouse)
{
	thBool bRet = THFALSE;
	Vec2 vecMousePos = pMouse->getLocationInView();
	int nFnRet = 0;
	int nSpMoveTag = 0;

	TH_EXIT_SUCCESS(NULL == m_ptQm);

	nSpMoveTag = m_ptQm->pCommandMovement->pSpCharacter->getTag();

	/* 检查出售按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pSellTower->pSpCharacter, vecMousePos);
	if (bRet)
	{
		m_pTaget->setUninitFlag();
	}
	/* 检查移动按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pCommandMovement->pSpCharacter, vecMousePos);
	if (bRet && THSP_FLAG_ENABLE == nSpMoveTag)
	{
		m_ptMoveRangeHalo->pSpCharacter->setVisible(THTRUE);
		m_ptMoveSelectingMouse->pSpCharacter->setVisible(THTRUE);
		CTHCcBaseHandler::getInstance()->setShowWinMouseCursor(THFALSE);
	}
	else
	{
		m_ptMoveRangeHalo->pSpCharacter->setVisible(THFALSE);
		m_ptMoveSelectingMouse->pSpCharacter->setVisible(THFALSE);
	}

	pMouse->stopPropagation();
	
Exit0:
	return;
}

void CThDefTowerQuickMenu::onMouseDown(EventMouse* pMouse)
{
	thBool bRet = THFALSE;
	Vec2 vecMousePos = pMouse->getLocationInView();

	TH_EXIT_SUCCESS(NULL == m_ptQm);

	/* 如果是按钮, 阻止事件传播. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pSellTower->pSpCharacter, vecMousePos) || CThBaseCharacter::getIsHoverSprite(m_ptQm->pCommandMovement->pSpCharacter, vecMousePos);
	if (bRet)
	{
		pMouse->stopPropagation();
	}

Exit0:
	return;
}

void CThDefTowerQuickMenu::onMouseMove(EventMouse* pMouse)
{
	thBool bRet = THFALSE;
	Vec2 vecMousePos = pMouse->getLocationInView();
	Vec2 vecMoveMouseLocationInNode;
	
	TH_EXIT_SUCCESS(NULL == m_ptQm);

	vecMoveMouseLocationInNode = m_ptMoveSelectingMouse->pSpCharacter->getParent()->convertToNodeSpaceAR(vecMousePos);

	/* 检查出售按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pSellTower->pSpCharacter, vecMousePos);
	m_ptSellHoverBorder->pSpCharacter->setVisible(bRet);
	if (bRet)
	{
		m_ptSellHoverBorder->pSpCharacter->setPosition(m_ptQm->pSellTower->pSpCharacter->getPosition());
		m_ptSellHoverBorder->pSpCharacter->setScale(m_ptQm->pSellTower->pSpCharacter->getScale());
	}
	else
	{
	}

	/* 检查移动按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pCommandMovement->pSpCharacter, vecMousePos);
	m_ptMoveSelectingMouse->pSpCharacter->setPosition(vecMoveMouseLocationInNode);
	m_ptMoveHoverBorder->pSpCharacter->setVisible(bRet);
	if (bRet)
	{
		m_ptMoveHoverBorder->pSpCharacter->setPosition(m_ptQm->pCommandMovement->pSpCharacter->getPosition());
		m_ptMoveHoverBorder->pSpCharacter->setScale(m_ptQm->pCommandMovement->pSpCharacter->getScale());
	}
	else
	{
	}

Exit0:
	return;
}

void CThDefTowerQuickMenu::update(float dt)
{
	return;
}
