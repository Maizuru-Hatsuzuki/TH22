/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.05.23
* Description	: def tower quick menu.
* *******************************************************/


#include "thCcDefTower.h"
#include "thCcAnimation.h"
#include "thCcDbg.h"
#include "thBaseMacro.h"


CThDefTowerQuickMenu* CThDefTowerQuickMenu::m_pSelf;


CThDefTowerQuickMenu::CThDefTowerQuickMenu()
{
	m_pMouse								= NULL;
	m_ptQm									= NULL;
	m_pTaget								= NULL;
	m_ptSellHoverBorder						= NULL;
	m_ptMoveHoverBorder						= NULL;
	m_ptSkillHoverBorder					= NULL;
	m_ptMoveRangeHalo						= NULL;
	m_ptMoveSelectingMouse					= NULL;
	m_ptMoveSelectedMouse					= NULL;
	m_ptMoveSelectedErrorMouse				= NULL;
	m_ptAniMovePosSelectingMouse			= NULL;
	m_ptAniMovePosSelectedMouse				= NULL;
	m_ptAniMovePosSelectedErrorMouse		= NULL;
	m_ptCurrentMouse						= NULL;
	m_emTagTowerType						= THEM_DEFTOWER_TYPE::DEFTOWERTYPE_UNKNOW;
	m_emStepUninit							= THEM_DELAY_UNINIT_FLAG::FLAG_NOTNEED_UNINIT;
}

CThDefTowerQuickMenu::~CThDefTowerQuickMenu()
{
}

thBool CThDefTowerQuickMenu::init()
{
	thBool bRet = THFALSE;
	const char* cszpSpTex = "Quickmenu Material_";
	CHARACTER_DESC tTmpChacDesc = { 0 };

	/* 如果 m_pMouse 有值, 就代表已经初始化过了. */
	TH_PROCESS_SUCCESS(m_pMouse);
	m_pMouse = EventListenerMouse::create();

	if (NULL == m_ptSellHoverBorder)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu sell border", cszpSpTex, 34, &m_ptSellHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptSellHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptSellHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptMoveHoverBorder)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu move border", cszpSpTex, 63, &m_ptMoveHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptSkillHoverBorder)
	{

	}
	if (NULL == m_ptMoveRangeHalo)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Warrior move range", "HUD Material_", 412, &m_ptMoveRangeHalo);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveRangeHalo->pSpCharacter->setVisible(THFALSE);
		m_ptMoveRangeHalo->pSpCharacter->setScaleY(0.75f);
		m_ptMoveRangeHalo->pSpCharacter->setScaleX(1.05f);
		
		m_pMoveRangeHaloJudge = DrawNode::create();
		m_pMoveRangeHaloJudge->drawSolidCircle(
			m_ptMoveRangeHalo->pSpCharacter->getPosition(), 
			m_ptMoveRangeHalo->pSpCharacter->getContentSize().width / 2,
			CC_DEGREES_TO_RADIANS(360), 
			100, 
			1.05f,
			0.75f,
			Color4F::RED
		);
		this->addChild(m_pMoveRangeHaloJudge);
#if !THDBG_FLAG_MOVERANGEHALO
		m_pMoveRangeHaloJudge->setVisible(THFALSE);
#endif

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

Exit1:
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::initBasicAni()
{
	thBool bRet = THFALSE;
	char szarrPathAni[MAX_PATH] = { 0 };
	CHARACTER_DESC tTmpChacDesc = { 0 };

	/* 创建动画所需的精灵. */
	if (NULL == m_ptMoveSelectingMouse)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("sp_selectingMouse", "posMouse", 1, &m_ptMoveSelectingMouse);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveSelectingMouse->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveSelectingMouse->pSpCharacter);
	}
	if (NULL == m_ptMoveSelectedMouse)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("sp_selectedMouse", "wMove", 1, &m_ptMoveSelectedMouse);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveSelectedMouse->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveSelectedMouse->pSpCharacter);
	}
	if (NULL == m_ptMoveSelectedErrorMouse)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("sp_selectedErrorMouse", "moveErr", 1, &m_ptMoveSelectedErrorMouse);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveSelectedErrorMouse->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveSelectedErrorMouse->pSpCharacter);
	}
	
	/* 创建动画. */
	if (NULL == m_ptAniMovePosSelectingMouse)
	{
		TH_GETWINRESPATH(szarrPathAni, "data\\AnimateConfig\\Basic\\AniMoveSelectingMouse.ini");
		bRet = CthCcFrameByFrameAnimation::getInstance()->createAnimationWithPListIni(szarrPathAni, &m_ptAniMovePosSelectingMouse);
		TH_PROCESS_ERROR(bRet && m_ptMoveSelectingMouse);
		m_ptMoveSelectingMouse->pSpCharacter->runAction(m_ptAniMovePosSelectingMouse->pAnimate);
	}
	if (NULL == m_ptAniMovePosSelectedMouse)
	{
		TH_GETWINRESPATH(szarrPathAni, "data\\AnimateConfig\\Basic\\AniMoveSelectedMouse.ini");
		bRet = CthCcFrameByFrameAnimation::getInstance()->createAnimationWithPListIni(szarrPathAni, &m_ptAniMovePosSelectedMouse);
		TH_PROCESS_ERROR(bRet && m_ptAniMovePosSelectedMouse);
		m_ptAniMovePosSelectedMouse->pAnimate->retain();
		m_ptAniMovePosSelectedMouse->pAnimate->setTag(TH_ANITAG_MOVING);
	}
	if (NULL == m_ptAniMovePosSelectedErrorMouse)
	{
		TH_GETWINRESPATH(szarrPathAni, "data\\AnimateConfig\\Basic\\AniMoveSelectedErrorMouse.ini");
		bRet = CthCcFrameByFrameAnimation::getInstance()->createAnimationWithPListIni(szarrPathAni, &m_ptAniMovePosSelectedErrorMouse);
		TH_PROCESS_ERROR(bRet && m_ptAniMovePosSelectedErrorMouse);
		m_ptAniMovePosSelectedErrorMouse->pAnimate->retain();
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
	THFREE(m_ptMoveSelectedMouse);
	THFREE(m_ptMoveSelectedErrorMouse);
	THFREE(m_ptAniMovePosSelectingMouse);
	THFREE(m_ptAniMovePosSelectedMouse);
	THFREE(m_ptAniMovePosSelectedErrorMouse);
	THFREE(m_ptQm->pBg);
	THFREE(m_ptQm->pCommandMovement);
	THFREE(m_ptQm->pSellTower);
	
	/* m_ptCurrentMouse 不是堆上的内存, 直接置空. */
	m_ptCurrentMouse = NULL;

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
		TH_PROCESS_ERROR(m_pSelf);
	}

	bRet = THTRUE;
Exit0:
	ASSERT(bRet);
	return m_pSelf;
}

thBool CThDefTowerQuickMenu::getMouseCursorIsPlayAni(const int cnAniTag)
{
	thBool bRet = THFALSE;

	switch (cnAniTag)
	{
	case TH_ANITAG_MOVEERROR:
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectedErrorMouse, bRet = m_ptMoveSelectedErrorMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVEERROR));
		break;

	case TH_ANITAG_MOVING:
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectingMouse, bRet = m_ptMoveSelectedMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVING));
		break;

	default:
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectedErrorMouse, bRet = m_ptMoveSelectedErrorMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVEERROR));
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectedMouse, bRet = m_ptMoveSelectedMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVING));
		break;
	}
	
	return bRet;
}

thBool CThDefTowerQuickMenu::getIsClickInMoveRangeHalo(Vec2 vecPosInView)
{
	thBool bRet = THFALSE;
	Vec2 vecMousePosInHaloJudgeNode = m_pMoveRangeHaloJudge->convertToNodeSpace(vecPosInView);
	Vec2 vecMoveHaloPos = m_ptMoveRangeHalo->pSpCharacter->getPosition();
	float fDistanceClickHalo = vecMousePosInHaloJudgeNode.distance(vecMoveHaloPos);
	float fHaloJudgeRadius = m_ptMoveRangeHalo->pSpCharacter->getBoundingBox().size.width / 2;

	if (fDistanceClickHalo <= fHaloJudgeRadius)
	{
		bRet = THTRUE;
	}

	return bRet;
}

/* 获取技能图标位置, 最大 5 个. */
thBool CThDefTowerQuickMenu::getChacSkillPos(const int nSkillCnt, int arrarrnRet[][2])
{
	thBool bRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR ptTower = NULL;
	float fBgWidth = m_ptQm->pBg->pSpCharacter->getBoundingBox().size.width * m_pTaget->getQmScale();
	float fBgHight = m_ptQm->pBg->pSpCharacter->getBoundingBox().size.height * m_pTaget->getQmScale();
	float fBgRadius = 0;

	TH_PROCESS_ERROR(5 > nSkillCnt);
	switch (nSkillCnt)
	{
	case 1:
	{
		fBgRadius = m_ptQm->pBg->pSpCharacter->getBoundingBox().size.height / 2;
		arrarrnRet[0][0] = m_ptQm->pBg->pSpCharacter->getPositionX() + cos(90 * (M_PI / 180));
		arrarrnRet[0][1] = m_ptQm->pBg->pSpCharacter->getPositionY() + fBgRadius + sin(90 * (M_PI / 180));
		break;
	}
	case 2:
	{
		m_pTaget->getCharacterFrameInfo(&ptTower);
		TH_PROCESS_ERROR(ptTower);

		fBgRadius = sqrtf(
			fBgWidth * fBgWidth +
			fBgHight * fBgHight
		) * 0.34f;
		arrarrnRet[0][0] = m_ptQm->pBg->pSpCharacter->getPositionX() - fBgRadius - cos(60 * (M_PI / 180));
		arrarrnRet[0][1] = m_ptQm->pBg->pSpCharacter->getPositionY() + fBgRadius + sin(60 * (M_PI / 180));
		arrarrnRet[1][0] = m_ptQm->pBg->pSpCharacter->getPositionX() + fBgRadius + cos(60 * (M_PI / 180));
		arrarrnRet[1][1] = arrarrnRet[0][1];
		break;
	}
	case 3:
	{

	}
	case 4:
	{

	}
	case 5:
	{

	}
	default:
		break;
	}
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::getChacSkillLevelPointPos(const short csCnt, float* arrfRet, TH_SKILL_PTR pSk)
{
	thBool bRet = THFALSE;
	float fRadius = pSk->pChacFrSkill->pSpCharacter->getBoundingBox().size.height / 2;
	float fTagX = 0.f;
	float fTagY = 0.f;

	for (short s = 0; s < csCnt; s++)
	{
		TH_GETX(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionX() + cos(90 * (M_PI / 180));
		TH_GETY(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionY() + fRadius + sin(90 * (M_PI / 180));
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::setChacSkillLevelPoint(enum THEM_CHARACTER_LEVEL emLv, TH_SKILL_PTR pSk)
{
	thBool bRet = THFALSE;
	
	float fSkHeight = 0.f;
	float fSkWidth = 0.f;

	float arrfPosRet[2] = { 0 };
	char szarrSkillPointIni[MAX_PATH] = { 0 };
	CHARACTER_DESC_PTR pSkPointDesc = NULL;

	TH_PROCESS_ERROR(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4 >= emLv);
	TH_GETWINRESPATH(szarrSkillPointIni, "data\\CharacterConfig\\Skill\\ChacSkLevelPoint.ini");
	bRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrSkillPointIni, &pSkPointDesc);
	TH_PROCESS_ERROR(bRet);

	for (short s = 0; s < (int)emLv; s++)
	{
		bRet = initCharacterWithPlist(pSkPointDesc, &pSk->arrpSkillLevelPoint[s]);
		TH_PROCESS_ERROR(bRet);
		/* 函数外进行 addChild, Z 值靠前. */
	}
	fSkHeight = pSk->arrpSkillLevelPoint[0]->pSpCharacter->getBoundingBox().size.height;
	fSkHeight = pSk->arrpSkillLevelPoint[0]->pSpCharacter->getBoundingBox().size.width;

	switch (emLv)
	{
	case CHARACTER_LEVEL_1:
	{
		bRet = getChacSkillLevelPointPos(int(emLv), arrfPosRet, pSk);
		TH_PROCESS_ERROR(bRet);
		pSk->arrpSkillLevelPoint[0]->pSpCharacter->setPositionX(TH_GETX(arrfPosRet));
		pSk->arrpSkillLevelPoint[0]->pSpCharacter->setPositionY(TH_GETY(arrfPosRet));
		break;
	}
	case CHARACTER_LEVEL_2:
	{

		break;
	}
	case CHARACTER_LEVEL_3:
		break;
	case CHARACTER_LEVEL_4:
		break;
	case CHARACTER_LEVEL_5:
		break;
	case CHARACTER_MAXLEVEL:
		break;
	default:
		break;
	}
	bRet = THTRUE;
Exit0:
	return bRet;
}

enum THEM_DELAY_UNINIT_FLAG CThDefTowerQuickMenu::getDefTowerDelayUninitType() const
{
	return m_emStepUninit;
}

void CThDefTowerQuickMenu::setMouseCursorAni(enum THEM_QM_MOUSECURSOR emMouseType)
{
	Sequence* pAniSeqMouseErr = NULL;
	CallFunc* fnSwitchSelectingMouse = NULL;

	TH_RUN_SUCCESS(NULL != m_ptCurrentMouse, m_ptCurrentMouse->pSpCharacter->setVisible(THFALSE));

	switch (emMouseType)
	{
	case CUR_SELECTING_POSITION:
		m_ptMoveSelectingMouse->pSpCharacter->setVisible(THTRUE);
		CTHCcBaseHandler::getInstance()->setShowWinMouseCursor(THFALSE);
		m_ptCurrentMouse = m_ptMoveSelectingMouse;
		break;

	case CUR_SELECTED_POSITION:
		m_ptMoveSelectedMouse->pSpCharacter->setVisible(THTRUE);
		m_ptMoveSelectedMouse->pSpCharacter->runAction(m_ptAniMovePosSelectedMouse->pAnimate);
		m_ptCurrentMouse = m_ptMoveSelectedMouse;
		CTHCcBaseHandler::getInstance()->setShowWinMouseCursor(THTRUE);
		break;

	case CUR_SELECTERR_POSITION:
		fnSwitchSelectingMouse = CallFunc::create(CC_CALLBACK_0(CThDefTowerQuickMenu::setMouseCursorAni, this, THEM_QM_MOUSECURSOR::CUR_SELECTING_POSITION));
		pAniSeqMouseErr = Sequence::create(m_ptAniMovePosSelectedErrorMouse->pAnimate, fnSwitchSelectingMouse, NULL);
		pAniSeqMouseErr->setTag(TH_ANITAG_MOVEERROR);

		m_ptMoveSelectedErrorMouse->pSpCharacter->setVisible(THTRUE);
		m_ptMoveSelectedErrorMouse->pSpCharacter->runAction(pAniSeqMouseErr);
		m_ptCurrentMouse = m_ptMoveSelectedErrorMouse;
		break;

	default:
		break;
	}

	return ;
}

void CThDefTowerQuickMenu::setStopUninit()
{
	m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_UNINIT;
	return;
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
	/* 快速菜单背景(圈). */
	bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu bg", cszpSpTex, 96, &ptChacFrameQuickMenuBg);
	TH_PROCESS_ERROR(bRet && ptChacFrameQuickMenuBg);
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
	
	bRet = CThBaseCharacter::initCharacterWithPlist(&tCommandMovement, &ptChacCommandMovement);
	TH_PROCESS_ERROR(bRet);

	/* 移动(禁用)按钮. */
	tCommandMovement.nDefaultTexPlistPos = 48;
	bRet = CThBaseCharacter::initCharacterWithPlist(&tCommandMovement, &ptChacCommandMovementDisable);
	TH_PROCESS_ERROR(bRet);

	if (THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS == m_emTagTowerType || THEM_DEFTOWER_TYPE::DEFTOWERTYPE_ARCHER_WARRIORS == m_emTagTowerType)
	{
		/* 可移动单位. */
		ptDefTowerQm->pCommandMovement = ptChacCommandMovement;
		ptDefTowerQm->pCommandMovement->pSpCharacter->setTag(THSP_FLAG_ENABLE);
	}
	else
	{
		/* 不可移动单位. */
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

	bRet = CThBaseCharacter::initCharacterWithPlist(&tSellDefTower, &ptChacSellDefTower);
	TH_PROCESS_ERROR(bRet);
	ptDefTowerQm->pSellTower = ptChacSellDefTower;

	this->addChild(ptDefTowerQm->pBg->pSpCharacter, 0, "QM_Bg");
	this->addChild(ptDefTowerQm->pCommandMovement->pSpCharacter, 0, "QM_CommandMove");
	this->addChild(ptDefTowerQm->pSellTower->pSpCharacter, 0, "QM_Sell");

	m_ptQm = ptDefTowerQm;
	bRet = THTRUE;
Exit0:
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
	m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_NOTNEED_UNINIT;
	int arrarrnSkillPos[2][2] = { 0, 0 };

	bRet = init();
	TH_PROCESS_ERROR(bRet);
	bRet = createBasicQm(cfX, cfY, cfTagScale, ptDefTowerQm);
	TH_PROCESS_ERROR(bRet);

	bRet = CThCcCharacterSkillHanlder::getInstance()->setTargetSkillUnion(ptDefTowerQm->emTowerType, ptDefTowerQm->emCharacterLevel, THTRUE, &m_ptQm->puChacSkill, &m_ptQm->nChacSkillCnt);
	TH_PROCESS_ERROR(bRet);
	bRet = getChacSkillPos(m_ptQm->nChacSkillCnt, arrarrnSkillPos);
	TH_PROCESS_ERROR(bRet);

	GETSK_DOLLREPAIR(m_ptQm)->pSpCharacter->setPositionX(arrarrnSkillPos[0][0]);
	GETSK_DOLLREPAIR(m_ptQm)->pSpCharacter->setPositionY(arrarrnSkillPos[0][1]);
	setChacSkillLevelPoint(GETSK_DOLLREPAIR(m_ptQm)->emMaxLevel, m_ptQm->puChacSkill->ptAliceMargatroidLv4Skill->ptSkDollRepair);

	GETSK_DOLLSTRENGTHEM(m_ptQm)->pSpCharacter->setPositionX(arrarrnSkillPos[1][0]);
	GETSK_DOLLSTRENGTHEM(m_ptQm)->pSpCharacter->setPositionY(arrarrnSkillPos[1][1]);

	this->addChild(GETSK_DOLLREPAIR(m_ptQm)->pSpCharacter);
	this->addChild(GETSK_DOLLSTRENGTHEM(m_ptQm)->pSpCharacter);

	for (short s = 0; s < (int)GETSK_DOLLREPAIR(m_ptQm)->emMaxLevel; s++)
	{
		this->addChild(m_ptQm->puChacSkill->ptAliceMargatroidLv4Skill->ptSkDollRepair->arrpSkillLevelPoint[s]->pSpCharacter);
	}
	/* 设置一下范围圈的坐标. 防御塔锚点是 0.5, 0, 也同步修改一下. */
	m_ptMoveRangeHalo->pSpCharacter->setPosition(pTaget->getPosition());
	m_ptMoveSelectedMouse->pSpCharacter->setScale(cfTagScale);

	bRet = THTRUE;
Exit0:
	if (NULL != m_ptQm->puChacSkill)
	{
		CThCcCharacterSkillHanlder::getInstance()->uninitWarriorSkillLv4Union(m_ptQm->puChacSkill);
	}
	return bRet;
}

thBool CThDefTowerQuickMenu::destoryQmWarriorLevel4(DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget)
{
	thBool bRet = THFALSE;
	
	/* 因为有鼠标精灵动画播放, 所以也要做延迟释放. */
	m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_NEED_UNINIT;

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

	TH_PROCESS_SUCCESS(NULL == m_ptQm);

	nSpMoveTag = m_ptQm->pCommandMovement->pSpCharacter->getTag();
	/* 检查出售按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pSellTower->pSpCharacter, vecMousePos, THFALSE);
	if (bRet)
	{
		m_pTaget->setUninitFlag();
	}
	/* 检查移动按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pCommandMovement->pSpCharacter, vecMousePos, THFALSE);
	if (bRet && THSP_FLAG_ENABLE == nSpMoveTag)
	{
		m_ptMoveRangeHalo->pSpCharacter->setVisible(THTRUE);
		setMouseCursorAni(THEM_QM_MOUSECURSOR::CUR_SELECTING_POSITION);
	}

	pMouse->stopPropagation();
	
Exit1:
	bRet = THTRUE;
Exit0:
	ASSERT(bRet);
	return;
}

void CThDefTowerQuickMenu::onMouseDown(EventMouse* pMouse)
{
	thBool bRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR ptTower = NULL;
	Vec2 vecMousePos = pMouse->getLocationInView();
	
	TH_PROCESS_SUCCESS(NULL == m_ptQm);

	/* 如果正在播放错误动画则 PASS. */
	if (THFALSE == getMouseCursorIsPlayAni(TH_ANITAG_MOVEERROR))
	{
		/* 如果是按钮, 阻止事件传播. */
		bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pSellTower->pSpCharacter, vecMousePos, THFALSE) ||
			CThBaseCharacter::getIsHoverSprite(m_ptQm->pCommandMovement->pSpCharacter, vecMousePos, THFALSE);

		if (bRet)
		{
			pMouse->stopPropagation();
		}

		/* 指挥移动 */
		if (THTRUE == m_ptMoveSelectingMouse->pSpCharacter->isVisible())
		{
			m_pTaget->getCharacterFrameInfo(&ptTower);
			bRet = getIsClickInMoveRangeHalo(vecMousePos) &&
				!CThBaseCharacter::getIsHoverSprite(ptTower->pSpCharacter, vecMousePos, THFALSE);
			
			if (bRet)
			{
				/* 可通行. */
				setMouseCursorAni(THEM_QM_MOUSECURSOR::CUR_SELECTED_POSITION);
				bRet = m_pTaget->setWarriorsOverallMovement(vecMousePos.x, vecMousePos.y);
				TH_PROCESS_ERROR(bRet);
			}
			else
			{
				/* 立入禁止. */
				setMouseCursorAni(THEM_QM_MOUSECURSOR::CUR_SELECTERR_POSITION);
				pMouse->stopPropagation();
			}
		}
	}
	/* 移动错误时可以持续点击. */
	if (NULL != m_ptMoveSelectedErrorMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVEERROR))
	{
		setMouseCursorAni(THEM_QM_MOUSECURSOR::CUR_SELECTERR_POSITION);
		pMouse->stopPropagation();
	}

Exit1:
	bRet = THTRUE;
Exit0:
	ASSERT(bRet);
	return;
}

void CThDefTowerQuickMenu::onMouseMove(EventMouse* pMouse)
{
	thBool bRet = THFALSE;
	Vec2 vecMousePos = pMouse->getLocationInView();
	Vec2 vecMoveMouseLocationInNode;
	
	TH_PROCESS_SUCCESS(NULL == m_ptQm);

	/* 需要跟随鼠标指针的按钮精灵坐标实时计算. */
	vecMoveMouseLocationInNode = m_ptMoveSelectingMouse->pSpCharacter->getParent()->convertToNodeSpaceAR(vecMousePos);
	m_ptMoveSelectingMouse->pSpCharacter->setPosition(vecMoveMouseLocationInNode);
	m_ptMoveSelectedErrorMouse->pSpCharacter->setPosition(vecMoveMouseLocationInNode);
	if (THFALSE == m_ptMoveSelectedMouse->pSpCharacter->isVisible())
	{
		m_ptMoveSelectedMouse->pSpCharacter->setPosition(vecMoveMouseLocationInNode);
	}

	/* 检查出售按钮. */
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pSellTower->pSpCharacter, vecMousePos, THFALSE);
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
	bRet = CThBaseCharacter::getIsHoverSprite(m_ptQm->pCommandMovement->pSpCharacter, vecMousePos, THFALSE);
	m_ptMoveHoverBorder->pSpCharacter->setVisible(bRet);
	if (bRet)
	{
		m_ptMoveHoverBorder->pSpCharacter->setPosition(m_ptQm->pCommandMovement->pSpCharacter->getPosition());
		m_ptMoveHoverBorder->pSpCharacter->setScale(m_ptQm->pCommandMovement->pSpCharacter->getScale());
	}
	else
	{
	}

Exit1:
Exit0:
	return;
}

thBool CThDefTowerQuickMenu::delayUninitMonitoring()
{
	thBool bRet = THFALSE;
	Action* pAni = NULL;
	Vector<cocos2d::Node*> vecAllChild = this->getChildren();
	int nAniSummonTag = 0;

	switch (m_emStepUninit)
	{
	case FLAG_NOTNEED_UNINIT:
		break;
	case FLAG_NEED_UNINIT:
		for (ssize_t i = 0; i < vecAllChild.size(); i++)
		{
			if (m_ptMoveSelectedMouse->pSpCharacter != vecAllChild.at(i))
			{
				vecAllChild.at(i)->setVisible(THFALSE);
			}
		}

		m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_UNITING;
		break;

	case FLAG_UNITING:
		/* 检查动画是否播放完成. */
		bRet = getMouseCursorIsPlayAni(TH_ANITAG_ALLANI);
		if (THFALSE == bRet)
		{
			m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_UNINIT;
		}
		break;

	case FLAG_UNINIT:
		this->removeAllChildrenWithCleanup(THTRUE);
		this->removeFromParentAndCleanup(THTRUE);
		uninit();
		m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_UNINIT_COMPLETE;
		break;

	case FLAG_UNINIT_COMPLETE:
		break;
	default:
		break;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::update(float dt)
{
	thBool bRet = THFALSE;
	
	bRet = delayUninitMonitoring();
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	ASSERT(bRet);
	return;
}
