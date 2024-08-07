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
	m_ptLvUpHoverBorder						= NULL;
	m_ptMoveRangeHalo						= NULL;
	m_ptMoveSelectingMouse					= NULL;
	m_ptMoveSelectedMouse					= NULL;
	m_ptMoveSelectedErrorMouse				= NULL;
	m_ptSkillHoverBorder					= NULL;
	m_ptTowerSkillLevelUp					= NULL;
	m_ptAniMovePosSelectingMouse			= NULL;
	m_ptAniMovePosSelectedMouse				= NULL;
	m_ptAniMovePosSelectedErrorMouse		= NULL;
	m_ptAniTowerSkillLevelUp				= NULL;
	m_ptCurrentMouse						= NULL;
	m_pSpFrameActiveSkillPoint				= NULL;
	m_emTagTowerType						= THEM_DEFTOWER_TYPE::DEFTOWERTYPE_UNKNOW;
	m_emStepUninit							= THEM_DELAY_UNINIT_FLAG::FLAG_NOTNEED_UNINIT;
	memset(m_arrpCurSk, 0, sizeof(TH_SKILL_PTR) * THMAX_TOWER_SKILL_COUNT);
	memset(m_arrpQmPrice, 0, sizeof(THQM_PRICE_PTR) * THMAX_TOWER_SKILL_COUNT);
	memset(m_arrpQmSkText, 0, sizeof(THQM_SKTEXT_PTR) * THMAX_TOWER_SKILL_QM_COUNT);
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
	m_pSpFrameActiveSkillPoint = SpriteFrameCache::getInstance()->getSpriteFrameByName("Quickmenu Material_45.png");
	TH_PROCESS_ERROR(m_pSpFrameActiveSkillPoint);

	memset(m_arrpQmPrice, 0, sizeof(THQM_PRICE_PTR) * THMAX_TOWER_SKILL_COUNT);
	if (NULL == m_ptSellHoverBorder)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu sell border", cszpSpTex, 15, &m_ptSellHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptSellHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptSellHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptMoveHoverBorder)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu move border", cszpSpTex, 34, &m_ptMoveHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptMoveHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptMoveHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptSkillHoverBorder)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu skill border", cszpSpTex, 57, &m_ptSkillHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptSkillHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptSkillHoverBorder->pSpCharacter);
	}
	if (NULL == m_ptLvUpHoverBorder)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu skill lvup border", cszpSpTex, 65, &m_ptLvUpHoverBorder);
		TH_PROCESS_ERROR(bRet);
		m_ptLvUpHoverBorder->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptLvUpHoverBorder->pSpCharacter);
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
	if (NULL == m_ptTowerSkillLevelUp)
	{
		bRet = CThBaseCharacter::initCharacterWithPlistSimple("sp_towerSkillLevelUp", "towerLvUp", 1, &m_ptTowerSkillLevelUp);
		TH_PROCESS_ERROR(bRet);
		m_ptTowerSkillLevelUp->pSpCharacter->setVisible(THFALSE);
		this->addChild(m_ptTowerSkillLevelUp->pSpCharacter);
	}
	
	/* 创建动画. */
	/* 没有立马 runAction 的要计数器 +1, 不然会被GC回收. */
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
	if (NULL == m_ptAniTowerSkillLevelUp)
	{
		TH_GETWINRESPATH(szarrPathAni, "data\\AnimateConfig\\Basic\\AniTowerSkillLevelUp.ini");
		bRet = CthCcFrameByFrameAnimation::getInstance()->createAnimationWithPListIni(szarrPathAni, &m_ptAniTowerSkillLevelUp);
		TH_PROCESS_ERROR(bRet && m_ptAniTowerSkillLevelUp);
		m_ptAniTowerSkillLevelUp->pAnimate->retain();
		m_ptAniTowerSkillLevelUp->pAnimate->setTag(TH_ANITAG_TOWERSK_LVUP);
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::uninit()
{
	m_ptAniMovePosSelectedMouse->pAnimate->release();
	m_ptAniMovePosSelectedErrorMouse->pAnimate->release();
	m_ptAniTowerSkillLevelUp->pAnimate->release();
	
	THFREE(m_ptSellHoverBorder);
	THFREE(m_ptMoveHoverBorder);
	THFREE(m_ptSkillHoverBorder);
	THFREE(m_ptMoveRangeHalo);
	THFREE(m_ptTowerSkillLevelUp);
	THFREE(m_ptMoveSelectingMouse);
	THFREE(m_ptLvUpHoverBorder);
	THFREE(m_ptMoveSelectedMouse);
	THFREE(m_ptMoveSelectedErrorMouse);
	THFREE(m_ptAniMovePosSelectingMouse);
	THFREE(m_ptAniMovePosSelectedMouse);
	THFREE(m_ptAniMovePosSelectedErrorMouse);
	THFREE(m_ptAniTowerSkillLevelUp);
	THFREE(m_ptQm->pBg);
	THFREE(m_ptQm->pCommandMovement);
	THFREE(m_ptQm->pSellTower);
	
	/* m_ptCurrentMouse 不是堆上的内存, 直接置空. */
	m_ptCurrentMouse = NULL;

	uninitAllSkillIconObject();

	unscheduleUpdate();
	CTHCcBaseHandler::getInstance()->setShowWinMouseCursor(THTRUE);
	Director::getInstance()->getEventDispatcher()->removeEventListener(m_pMouse);
	m_pMouse = NULL;
	m_ptQm = NULL;

	return;
}

void CThDefTowerQuickMenu::uninitAllSkillIconObject()
{
	for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
	{
		TH_RUN_SUCCESS(NULL != m_arrpQmPrice[s], THFREE(m_arrpQmPrice[s]->ptBg));
		THFREE(m_arrpQmPrice[s]);
	}
	for (short s = 0; s < THMAX_TOWER_SKILL_QM_COUNT; s++)
	{
		TH_RUN_SUCCESS(NULL != m_arrpQmSkText[s], THFREE(m_arrpQmSkText[s]->ptBg));
		THFREE(m_arrpQmSkText[s]);
	}

	CThCcCharacterSkillHanlder::getInstance()->uninitSkillUnion(m_ptQm->puChacSkill);
	for (int i = 0; i < THMAX_TOWER_SKILL_COUNT; i++)
	{
		m_arrpCurSk[i] = NULL;
	}

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

thBool CThDefTowerQuickMenu::getQmIsPlayAni(const int cnAniTag)
{
	thBool bRet = THFALSE;
	thBool bTmpRetA = THFALSE;
	thBool bTmpRetB = THFALSE;
	thBool bTmpRetC = THFALSE;

	switch (cnAniTag)
	{
	case TH_ANITAG_MOVEERROR:
	{
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectedErrorMouse, bRet = m_ptMoveSelectedErrorMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVEERROR));
		break;
	}
	case TH_ANITAG_MOVING:
	{
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectingMouse, bRet = m_ptMoveSelectedMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVING));
		break;
	}
	case TH_ANITAG_TOWERSK_LVUP:
	{
		TH_RUN_SUCCESS(NULL != m_ptTowerSkillLevelUp, bRet = m_ptTowerSkillLevelUp->pSpCharacter->getActionByTag(TH_ANITAG_TOWERSK_LVUP));
		break;
	}
	case TH_ANITAG_SCALEQM:
	{
		bRet = this->getActionByTag(TH_ANITAG_SCALEQM);
		break;
	}
	default:
	{
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectedErrorMouse, bTmpRetA = m_ptMoveSelectedErrorMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVEERROR));
		TH_RUN_SUCCESS(NULL != m_ptMoveSelectedMouse, bTmpRetB = m_ptMoveSelectedMouse->pSpCharacter->getActionByTag(TH_ANITAG_MOVING));
		TH_RUN_SUCCESS(NULL != m_ptTowerSkillLevelUp, bTmpRetC = m_ptTowerSkillLevelUp->pSpCharacter->getActionByTag(TH_ANITAG_TOWERSK_LVUP));
		bRet = this->getActionByTag(TH_ANITAG_SCALEQM) && bTmpRetA && bTmpRetB && bTmpRetC;
		break;
	}
	}
	
Exit1:
Exit0:
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
thBool CThDefTowerQuickMenu::getChacSkillPos(const int nSkillCnt, float arrarrfRet[][2])
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
		TH_GETX(arrarrfRet[0]) = m_ptQm->pBg->pSpCharacter->getPositionX() + cos(90 * (M_PI / 180));
		TH_GETY(arrarrfRet[0]) = m_ptQm->pBg->pSpCharacter->getPositionY() + fBgRadius + sin(90 * (M_PI / 180));
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
		TH_GETX(arrarrfRet[0]) = m_ptQm->pBg->pSpCharacter->getPositionX() - fBgRadius - cos(60 * (M_PI / 180));
		TH_GETY(arrarrfRet[0]) = m_ptQm->pBg->pSpCharacter->getPositionY() + fBgRadius + sin(60 * (M_PI / 180));
		TH_GETX(arrarrfRet[1]) = m_ptQm->pBg->pSpCharacter->getPositionX() + fBgRadius + cos(60 * (M_PI / 180));
		TH_GETY(arrarrfRet[1]) = TH_GETY(arrarrfRet[0]);
		break;
	}
	case 3:
	{
		break;
	}
	case 4:
	{
		break;
	}
	case 5:
	{
		break;
	}
	default:
		break;
	}
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::getChacSkillLevelPointPos(const short csPos, float* arrfRet, TH_SKILL_PTR pSk)
{
	thBool bRet = THFALSE;
	float fRadius = pSk->pChacFrSkill->pSpCharacter->getBoundingBox().size.height / 2 - 5;
	float fTagX = 0.f;
	float fTagY = 0.f;
	float fAngle = 0.f;

	switch (csPos)
	{
	case 1:
	{
		fAngle = 90.f;
		TH_GETX(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionX() + cos(fAngle * (M_PI / 180));
		TH_GETY(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionY() + fRadius + sin(fAngle * (M_PI / 180));
		break;
	}
	case 2:
	{
		fAngle = 45.f;
		fRadius = fRadius * 2 / 3;
		TH_GETX(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionX() + fRadius + cos(fAngle * (M_PI / 180));
		TH_GETY(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionY() + fRadius + sin(fAngle * (M_PI / 180));
		break;
	}
	case 3:
	{
		fAngle = 0.f;
		TH_GETX(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionX() + fRadius + cos(fAngle * (M_PI / 180));
		TH_GETY(arrfRet) = pSk->pChacFrSkill->pSpCharacter->getPositionY() + sin(fAngle * (M_PI / 180));
		break;
	}
	default:
		break;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::setChacSkillLevelPoint(enum THEM_CHARACTER_LEVEL emLv, TH_SKILL_PTR pSk)
{
	thBool bRet = THFALSE;
	float arrfPosRet[2] = { 0 };
	char szarrSkillPointIni[MAX_PATH] = { 0 };
	CHARACTER_DESC_PTR pSkPointDesc = NULL;
	TH_SKILL_PTR pPlayerSk = NULL;

	TH_PROCESS_ERROR(THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4 >= emLv);
	TH_GETWINRESPATH(szarrSkillPointIni, "data\\CharacterConfig\\Skill\\ChacSkLevelPoint.ini");
	bRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrSkillPointIni, &pSkPointDesc);
	TH_PROCESS_ERROR(bRet);

	m_pTaget->getCurSkillByName(pSk->szarrSkill, &pPlayerSk);
	TH_PROCESS_ERROR(pPlayerSk);

	for (short s = 0; s < (int)emLv; s++)
	{
		bRet = initCharacterWithPlist(pSkPointDesc, &pSk->arrpSkillLevelPoint[s]);
		TH_PROCESS_ERROR(bRet);

		bRet = getChacSkillLevelPointPos(s + 1, arrfPosRet, pSk);
		TH_PROCESS_ERROR(bRet);
		pSk->arrpSkillLevelPoint[s]->pSpCharacter->setPositionX(TH_GETX(arrfPosRet));
		pSk->arrpSkillLevelPoint[s]->pSpCharacter->setPositionY(TH_GETY(arrfPosRet));

		/* 函数外进行 addChild, Z 值靠前. */
	}

	for (short s = 0; s < (short)pPlayerSk->pChacFrSkill->emCurLevel; s++)
	{
		pSk->arrpSkillLevelPoint[s]->pSpCharacter->setSpriteFrame(m_pSpFrameActiveSkillPoint);
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::setChacSkillPrice(TH_SKILL_PTR pSk)
{
	thBool bRet = THFALSE;
	short sTag = 0;
	float fBgY = pSk->pChacFrSkill->pSpCharacter->getPositionY() - pSk->pChacFrSkill->pSpCharacter->getBoundingBox().size.height / 2 + 12;
	char szarrPrice[6] = { 0 };
	Color3B tFontColor;

	bRet = _getSkPriceArrVancantPos(&sTag);
	TH_PROCESS_ERROR(!bRet);

	sprintf_s(szarrPrice, "%d", pSk->nSkillPrice);
	m_arrpQmPrice[sTag] = THMALLOC(THQM_PRICE, sizeof(THQM_PRICE));
	TH_PROCESS_ERROR(m_arrpQmPrice[sTag]);
	strcpy_s(m_arrpQmPrice[sTag]->szarrSk, THMAX_CHAR_DESC, pSk->szarrSkill);
	m_arrpQmPrice[sTag]->pLbText = Label::createWithTTF(szarrPrice, "fonts\\Marker Felt.ttf", 18);
	TH_PROCESS_ERROR(m_arrpQmPrice[sTag]->pLbText);

	tFontColor.r = 254;
	tFontColor.g = 253;
	tFontColor.b = 0;

	/* 文字背景. */
	bRet = CThBaseCharacter::initCharacterWithPlistSimple("price bg", "Quickmenu Material_", 48, &m_arrpQmPrice[sTag]->ptBg);
	TH_PROCESS_ERROR(bRet);
	m_arrpQmPrice[sTag]->ptBg->pSpCharacter->setPosition(pSk->pChacFrSkill->pSpCharacter->getBoundingBox().getMidX(), fBgY);
	m_arrpQmPrice[sTag]->ptBg->pSpCharacter->setAnchorPoint(Vec2(0.5, 1));
	m_arrpQmPrice[sTag]->ptBg->pSpCharacter->setScale(0.8f);

	/* 文字, 靠上中. */
	m_arrpQmPrice[sTag]->pLbText->setAnchorPoint(Vec2(0.5, 1));
	m_arrpQmPrice[sTag]->pLbText->setPosition(pSk->pChacFrSkill->pSpCharacter->getBoundingBox().getMidX(), fBgY - 7);
	m_arrpQmPrice[sTag]->pLbText->setColor(tFontColor);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::resetChacSkillPrice(const char* cszpTag, const int cnCurPrice, int* pPriceRet)
{
	thBool bRet = THFALSE;
	/* 基础上涨比例. */
	const float cfBasePercIncr = 1.5f;
	int nTagPrice = cnCurPrice * cfBasePercIncr;
	char szPrice[6] = { 0 };

	for (int i = 0; i < THMAX_TOWER_SKILL_COUNT; i++)
	{
		if (NULL != m_arrpQmPrice[i] && 0 == strcmp(cszpTag, m_arrpQmPrice[i]->szarrSk))
		{
			sprintf_s(szPrice, "%d", nTagPrice);
			m_arrpQmPrice[i]->pLbText->setString(szPrice);
		}
	}

	*pPriceRet = nTagPrice;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::setChacSkillTextDesc(TH_SKILL_PTR pSk)
{
	thBool bRet = THFALSE;
	short sTag = 0;
	float fPoxX = m_ptQm->pBg->pSpCharacter->getPositionX();
	float fPosY = TH_GETTOPY(m_ptQm->pBg->pSpCharacter);
	float fTextLabelWidth = 0.f;
	float fTextY = 0.f;
	char szarrSkTitleDesc[THMAX_CHAR_DESC] = { 0 };
	char szarrMainTextDesc[THMAX_CHAR_DESC] = { 0 };
	char szarrSubTextDesc[THMAX_CHAR_DESC] = { 0 };
	Color3B tColor;
	THQM_SKTEXT_PTR pTmpSkText = NULL;

	bRet = _getSkPriceArrVancantPos(&sTag);
	TH_PROCESS_ERROR(!bRet);

	CThCcCharacterSkillHanlder::getInstance()->getSkillTextDescXml(pSk->szarrSkill, szarrSkTitleDesc, szarrMainTextDesc, szarrSubTextDesc);
	pTmpSkText = THMALLOC(THQM_SKTEXT, sizeof(THQM_SKTEXT));
	TH_PROCESS_ERROR(pTmpSkText);
	strcpy_s(pTmpSkText->szarrTagSk, THMAX_CHAR_DESC, pSk->szarrSkill);

	/* 文字背景. */
	bRet = CThBaseCharacter::initCharacterWithPlistSimple("skill text bg", "Quickmenu Material_", 64, &pTmpSkText->ptBg);
	TH_PROCESS_ERROR(bRet);
	fPosY = fPosY + pTmpSkText->ptBg->pSpCharacter->getBoundingBox().size.height * pTmpSkText->ptBg->pSpCharacter->getAnchorPoint().y + 50;
	pTmpSkText->ptBg->pSpCharacter->setPosition(fPoxX, fPosY);
	pTmpSkText->ptBg->pSpCharacter->setVisible(THFALSE);
	pTmpSkText->ptBg->pSpCharacter->setScaleX(0.8f);
	fTextLabelWidth = pTmpSkText->ptBg->pSpCharacter->getBoundingBox().size.width - 10;

	/* 文字标题. */
	tColor.r = 196;
	tColor.g = 233;
	tColor.b = 54;
	fTextY = TH_GETTOPY(pTmpSkText->ptBg->pSpCharacter) - 10;
	TH_CREATE_CN_LABELTEXT(
		pTmpSkText->pLbTitleText, szarrSkTitleDesc, 25, tColor, fPoxX,
		fTextY, THFALSE, TextHAlignment::LEFT, fTextLabelWidth
	);
	pTmpSkText->pLbTitleText->setAnchorPoint(Vec2(0.5, 1));

	/* 文字主介绍. */
	tColor.r = 240;
	tColor.g = 230;
	tColor.b = 185;
	fTextY = TH_GETBOTTOMY(pTmpSkText->pLbTitleText) - 5;
	TH_CREATE_CN_LABELTEXT(
		pTmpSkText->pLbMainDesc, szarrMainTextDesc, 19, tColor, fPoxX,
		fTextY, THFALSE, TextHAlignment::LEFT, fTextLabelWidth
	);
	pTmpSkText->pLbMainDesc->setAnchorPoint(Vec2(0.5, 1));

	/* 文字副介绍. */
	tColor.r = 189;
	tColor.g = 183;
	tColor.b = 107;
	fTextY = TH_GETBOTTOMY(pTmpSkText->ptBg->pSpCharacter);
	TH_CREATE_CN_LABELTEXT(
		pTmpSkText->pLbSubDesc, szarrSubTextDesc, 17, tColor, fPoxX,
		fTextY, THFALSE, TextHAlignment::LEFT, fTextLabelWidth
	);
	pTmpSkText->pLbSubDesc->setAnchorPoint(Vec2(0.5, 0));

	m_arrpQmSkText[sTag] = pTmpSkText;
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::getSkTextBySkillName(const char* cszpSk, THQM_SKTEXT_PTR* ppRet)
{
	for (short s = 0; s < THMAX_TOWER_SKILL_QM_COUNT; s++)
	{
		if (NULL != m_arrpQmSkText[s] && 0 == strcmp(m_arrpQmSkText[s]->szarrTagSk, cszpSk))
		{
			*ppRet = m_arrpQmSkText[s];
			break;
		}
	}
	return;
}

thBool CThDefTowerQuickMenu::_getSkTextArrVancantPos(short* psRet) const
{
	thBool bFull = THTRUE;
	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (NULL == m_arrpQmSkText[i])
		{
			bFull = THFALSE;
			*psRet = i;
			break;
		}
	}
	return bFull;
}

thBool CThDefTowerQuickMenu::_getSkPriceArrVancantPos(short* psRet) const
{
	thBool bFull = THTRUE;
	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (NULL == m_arrpQmPrice[i])
		{
			bFull = THFALSE;
			*psRet = i;
			break;
		}
	}
	return bFull;
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

thBool CThDefTowerQuickMenu::createBasicQm(const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, thBool bIsCreateGenSk)
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
	enum THEM_CHARACTER_LEVEL emLv = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_0;

	TH_PROCESS_SUCCESS(m_ptQm);
	m_ptQm = ptDefTowerQm;

	this->setScale(0.2f);
	this->setPositionX(cfX);
	this->setPositionY(cfY);

	TH_PROCESS_ERROR(NULL != ptDefTowerQm);
	/* 快速菜单背景(圈). */
	bRet = CThBaseCharacter::initCharacterWithPlistSimple("Quickmenu bg", cszpSpTex, 63, &ptChacFrameQuickMenuBg);
	TH_PROCESS_ERROR(bRet && ptChacFrameQuickMenuBg);
	ptDefTowerQm->pBg = ptChacFrameQuickMenuBg;

	/* 根据圈计算一些定位数据. */
	fBgLayerdX = ptChacFrameQuickMenuBg->pSpCharacter->getBoundingBox().size.width / 24;
	fBgLayerdY = ptChacFrameQuickMenuBg->pSpCharacter->getBoundingBox().size.height / 24;

	/* 移动按钮. */
	strcpy_s(tCommandMovement.szarrDefaultTexPlistPos, 64, THINI_DEFAULT_STR);
	strcpy_s(tCommandMovement.szarrSpriteTex, 64, cszpSpTex);
	strcpy_s(tCommandMovement.szarrSpriteName, 64, "Quickmenu move");
	tCommandMovement.nDefaultTexPlistPos = 19;
	tCommandMovement.fPosX = fBgLayerdX * 8.5;
	tCommandMovement.fPosY = -fBgLayerdY * 7.5;
	tCommandMovement.fScale = 0.7f;
	
	bRet = CThBaseCharacter::initCharacterWithPlist(&tCommandMovement, &ptChacCommandMovement);
	TH_PROCESS_ERROR(bRet);

	/* 移动(禁用)按钮. */
	tCommandMovement.nDefaultTexPlistPos = 20;
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
	tSellDefTower.nDefaultTexPlistPos = 14;
	tSellDefTower.fPosY = -fBgLayerdY * 11;
	tSellDefTower.fScale = 0.75f;

	bRet = CThBaseCharacter::initCharacterWithPlist(&tSellDefTower, &ptChacSellDefTower);
	TH_PROCESS_ERROR(bRet);
	ptDefTowerQm->pSellTower = ptChacSellDefTower;

	this->addChild(ptDefTowerQm->pBg->pSpCharacter, 0, "QM_Bg");
	this->addChild(ptDefTowerQm->pCommandMovement->pSpCharacter, 0, "QM_CommandMove");
	this->addChild(ptDefTowerQm->pSellTower->pSpCharacter, 0, "QM_Sell");

	/* 通用技能, 防御塔低于等级 2. */
	if (bIsCreateGenSk)
	{
		emLv = m_pTaget->getDefTowerCurLv();
		bRet = createQmGenera(emLv);
		TH_PROCESS_ERROR(bRet);
	}

	/* 设置一下范围圈的坐标. 防御塔锚点是 0.5, 0, 也同步修改一下. */
	m_ptMoveRangeHalo->pSpCharacter->setPosition(m_pTaget->getPosition());
	m_ptMoveSelectedMouse->pSpCharacter->setScale(cfTagScale);

Exit1:
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerQuickMenu::destoryQm()
{
	/* 因为有鼠标精灵动画播放, 所以也要做延迟释放. */
	m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_NEED_UNINIT;
	return;
}

thBool CThDefTowerQuickMenu::createQmGenera(enum THEM_CHARACTER_LEVEL emLv)
{
	thBool bRet = THFALSE;
	float arrarrfSkillPos[1][2] = { 0 };

	bRet = CThCcCharacterSkillHanlder::getInstance()->setTargetSkillUnion(
		m_pTaget->getDefTowerType(),
		emLv,
		THTRUE,
		&m_ptQm->puChacSkill,
		&m_ptQm->nChacSkillCnt
	);
	TH_PROCESS_ERROR(bRet);

	bRet = getChacSkillPos(m_ptQm->nChacSkillCnt, arrarrfSkillPos);
	TH_PROCESS_ERROR(bRet);

	GETSK_FRAMEINFO_GEN_LEVELUP(m_ptQm)->pSpCharacter->setPositionX(TH_GETX(arrarrfSkillPos[0]));
	GETSK_FRAMEINFO_GEN_LEVELUP(m_ptQm)->pSpCharacter->setPositionY(TH_GETY(arrarrfSkillPos[0]));

	bRet = setChacSkillPrice(GETSK_GEN_LEVELUP(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	bRet = setChacSkillTextDesc(GETSK_GEN_LEVELUP(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	m_arrpCurSk[0] = GETSK_GEN_LEVELUP(m_ptQm);

	this->addChild(GETSK_FRAMEINFO_GEN_LEVELUP(m_ptQm)->pSpCharacter);

	ADDCHILD_SKPRICE_AND_SKDESC(m_arrpQmPrice, m_arrpQmSkText);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::createQmWarrior(enum THEM_CHARACTER_LEVEL emLv, const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget)
{
	thBool bRet = THFALSE;
	if (NULL == m_pTaget)
	{
		m_pTaget = pTaget;
	}
	m_emTagTowerType = m_pTaget->getDefTowerType();
	m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_NOTNEED_UNINIT;

	bRet = init();
	TH_PROCESS_ERROR(bRet);
	bRet = createBasicQm(cfX, cfY, cfTagScale, ptDefTowerQm, emLv <= THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_2);
	TH_PROCESS_ERROR(bRet);

	/* 低于等级 2 级的防御塔技能在 createBaseQm 实现. */
	switch (emLv)
	{
	case CHARACTER_LEVEL_0:
		break;
	case CHARACTER_LEVEL_1:
		break;
	case CHARACTER_LEVEL_2:
		break;
	case CHARACTER_LEVEL_3:
	{
		bRet = createQmWarriorLevel3(cfTagScale, ptDefTowerQm, pTaget);
		TH_PROCESS_ERROR(bRet);
		break;
	}
	case CHARACTER_LEVEL_4:
	{
		bRet = createQmWarriorLevel4(cfTagScale, ptDefTowerQm, pTaget);
		TH_PROCESS_ERROR(bRet);
		break;
	}
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

thBool CThDefTowerQuickMenu::createQmWarriorLevel3(const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget)
{
	thBool bRet = THFALSE;
	float arrarrfSkillPos[THMAX_TOWER_SKILL_COUNT][2] = { 0 };

	/* 删除普通升级图标. */
	for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
	{
		TH_RUN_SUCCESS(NULL != m_arrpCurSk[s], this->removeChild(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter));
		THFREE(m_arrpQmPrice[s]);
	}
	uninitAllSkillIconObject();

	/* 添加转职技能. */
	bRet = CThCcCharacterSkillHanlder::getInstance()->setTargetSkillUnion(ptDefTowerQm->emTowerType, ptDefTowerQm->emCharacterLevel, THTRUE, &m_ptQm->puChacSkill, &m_ptQm->nChacSkillCnt);
	TH_PROCESS_ERROR(bRet);
	bRet = getChacSkillPos(m_ptQm->nChacSkillCnt, arrarrfSkillPos);
	TH_PROCESS_ERROR(bRet);

	GETSK_FRAMEINFO_DTCAREERCHANGE_WARRIOR(m_ptQm)->pSpCharacter->setPositionX(TH_GETX(arrarrfSkillPos[0]));
	GETSK_FRAMEINFO_DTCAREERCHANGE_WARRIOR(m_ptQm)->pSpCharacter->setPositionY(TH_GETY(arrarrfSkillPos[0]));
	bRet = setChacSkillPrice(GETSK_DTCAREERCHANGE_WARRIOR(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	bRet = setChacSkillTextDesc(GETSK_DTCAREERCHANGE_WARRIOR(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	m_arrpCurSk[0] = GETSK_DTCAREERCHANGE_WARRIOR(m_ptQm);

	this->addChild(GETSK_FRAMEINFO_DTCAREERCHANGE_WARRIOR(m_ptQm)->pSpCharacter);
	ADDCHILD_SKPRICE_AND_SKDESC(m_arrpQmPrice, m_arrpQmSkText);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerQuickMenu::createQmWarriorLevel4(const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget)
{
	thBool bRet = THFALSE;
	float arrarrfSkillPos[THMAX_TOWER_SKILL_COUNT][2] = { 0 };

	/* 删除普通升级图标. */
	for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
	{
		TH_RUN_SUCCESS(NULL != m_arrpCurSk[s], this->removeChild(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter));
		THFREE(m_arrpQmPrice[s]);
	}
	uninitAllSkillIconObject();

	/* 技能图标. */
	bRet = CThCcCharacterSkillHanlder::getInstance()->setTargetSkillUnion(ptDefTowerQm->emTowerType, ptDefTowerQm->emCharacterLevel, THTRUE, &m_ptQm->puChacSkill, &m_ptQm->nChacSkillCnt);
	TH_PROCESS_ERROR(bRet);
	bRet = getChacSkillPos(m_ptQm->nChacSkillCnt, arrarrfSkillPos);
	TH_PROCESS_ERROR(bRet);

	GETSK_FRAMEINFO_DOLLREPAIR(m_ptQm)->pSpCharacter->setPositionX(TH_GETX(arrarrfSkillPos[0]));
	GETSK_FRAMEINFO_DOLLREPAIR(m_ptQm)->pSpCharacter->setPositionY(TH_GETY(arrarrfSkillPos[0]));
	bRet = setChacSkillLevelPoint(GETSK_FRAMEINFO_DOLLREPAIR(m_ptQm)->emMaxLevel, GETSK_DOLLREPAIR(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	bRet = setChacSkillPrice(GETSK_DOLLREPAIR(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	bRet = setChacSkillTextDesc(GETSK_DOLLREPAIR(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	m_arrpCurSk[0] = GETSK_DOLLREPAIR(m_ptQm);

	GETSK_FRAMEINFO_DOLLSTRENGTHEM(m_ptQm)->pSpCharacter->setPositionX(TH_GETX(arrarrfSkillPos[1]));
	GETSK_FRAMEINFO_DOLLSTRENGTHEM(m_ptQm)->pSpCharacter->setPositionY(TH_GETY(arrarrfSkillPos[1]));
	bRet = setChacSkillLevelPoint(GETSK_FRAMEINFO_DOLLSTRENGTHEM(m_ptQm)->emMaxLevel, GETSK_DOLLSTRENGTHEM(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	bRet = setChacSkillPrice(GETSK_DOLLSTRENGTHEM(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	bRet = setChacSkillTextDesc(GETSK_DOLLSTRENGTHEM(m_ptQm));
	TH_PROCESS_ERROR(bRet);
	m_arrpCurSk[1] = GETSK_DOLLSTRENGTHEM(m_ptQm);

	this->addChild(GETSK_FRAMEINFO_DOLLREPAIR(m_ptQm)->pSpCharacter);
	this->addChild(GETSK_FRAMEINFO_DOLLSTRENGTHEM(m_ptQm)->pSpCharacter);

	/* 技能点. */
	for (short s = 0; s < (int)GETSK_FRAMEINFO_DOLLREPAIR(m_ptQm)->emMaxLevel; s++)
	{
		this->addChild(GETSK_DOLLREPAIR(m_ptQm)->arrpSkillLevelPoint[s]->pSpCharacter);
	}
	for (short s = 0; s < (int)GETSK_FRAMEINFO_DOLLSTRENGTHEM(m_ptQm)->emMaxLevel; s++)
	{
		this->addChild(GETSK_DOLLSTRENGTHEM(m_ptQm)->arrpSkillLevelPoint[s]->pSpCharacter);
	}

	ADDCHILD_SKPRICE_AND_SKDESC(m_arrpQmPrice, m_arrpQmSkText);

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
	int nCurLv = 0;
	int nSkZ = 0;
	TH_SKILL_PTR pPlayerSk = NULL;

	/* debug use. */
	int nPlayerMoney = 1000;

	TH_PROCESS_SUCCESS(NULL == m_ptQm);

	if (EventMouse::MouseButton::BUTTON_LEFT == pMouse->getMouseButton())
	{
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

		/* 检查技能按钮. */
		for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
		{
			if (NULL != m_arrpCurSk[s] && NULL != m_pTaget)
			{
				m_pTaget->getCurSkillByName(m_arrpCurSk[s]->szarrSkill, &pPlayerSk);
				TH_PROCESS_ERROR(pPlayerSk);

				bRet = CThBaseCharacter::getIsHoverSprite(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter, vecMousePos, THFALSE);
				if (
					bRet && m_arrpCurSk[s]->nSkillPrice <= nPlayerMoney &&
					pPlayerSk->pChacFrSkill->emMaxLevel > pPlayerSk->pChacFrSkill->emCurLevel && THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL > pPlayerSk->pChacFrSkill->emCurLevel
					)
				{
					m_pTaget->setSkLevelUpByName(m_arrpCurSk[s]->szarrSkill);

					if (0 != strcmp(m_arrpCurSk[s]->szarrSkill, THSK_DEFTOWER_LVUP))
					{
						nCurLv = int(pPlayerSk->pChacFrSkill->emCurLevel);
						TH_PROCESS_ERROR(0 <= nCurLv && THMAX_SKILL_LEVEL > nCurLv);
						m_arrpCurSk[s]->arrpSkillLevelPoint[nCurLv]->pSpCharacter->setSpriteFrame(m_pSpFrameActiveSkillPoint);

						m_ptTowerSkillLevelUp->pSpCharacter->setPosition(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->getPosition());
						m_ptTowerSkillLevelUp->pSpCharacter->setVisible(THTRUE);

						bRet = getQmIsPlayAni(TH_ANITAG_TOWERSK_LVUP);
						if (THFALSE == bRet)
						{
							nSkZ = m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->getLocalZOrder();
							m_ptTowerSkillLevelUp->pSpCharacter->setLocalZOrder(nSkZ + 1);
							m_ptTowerSkillLevelUp->pSpCharacter->runAction(m_ptAniTowerSkillLevelUp->pAnimate);
						}
					}

					/* 升级后更换价格或者图标. */
					bRet = updateSkillLvUp(s);
					TH_PROCESS_ERROR(bRet);

					break;
				}
				else
				{
				#if !THDBG_FLAG_QMMOUSEUP
					CCLOG("bRet: %d", bRet);
					CCLOG("m_arrpCurSk[s]->nSkillPrice <= nPlayerMoney: %d", m_arrpCurSk[s]->nSkillPrice <= nPlayerMoney);
					CCLOG("emCurLevel: %d", pPlayerSk->pChacFrSkill->emMaxLevel > pPlayerSk->pChacFrSkill->emCurLevel);
					CCLOG("max lv: %d", THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL > pPlayerSk->pChacFrSkill->emCurLevel);
				#endif
				}
			}
		}

		pMouse->stopPropagation();
	}
	else if (EventMouse::MouseButton::BUTTON_RIGHT == pMouse->getMouseButton())
	{
	}
	
	
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

	if (EventMouse::MouseButton::BUTTON_LEFT == pMouse->getMouseButton())
	{
		/* 如果正在播放错误动画则 PASS. */
		if (THFALSE == getQmIsPlayAni(TH_ANITAG_MOVEERROR))
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

		/* 检查技能按钮并阻止释放. */
		for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
		{
			if (NULL != m_arrpCurSk[s])
			{
				bRet = CThBaseCharacter::getIsHoverSprite(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter, vecMousePos, THFALSE);
				if (bRet)
				{
					pMouse->stopPropagation();
					break;
				}
			}
		}
	}
	else if (EventMouse::MouseButton::BUTTON_RIGHT == pMouse->getMouseButton())
	{

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
	THQM_SKTEXT_PTR pTmpSkText = NULL;
	
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
		m_ptSellHoverBorder->pSpCharacter->setScale(m_ptQm->pSellTower->pSpCharacter->getScale() + 0.05f);
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
		m_ptMoveHoverBorder->pSpCharacter->setScale(m_ptQm->pCommandMovement->pSpCharacter->getScale() + 0.05f);
	}
	else
	{
	}

	/* 检查技能按钮. */
	for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
	{
		if (NULL != m_arrpCurSk[s])
		{
			bRet = CThBaseCharacter::getIsHoverSprite(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter, vecMousePos, THFALSE);
			
			/* 检查技能说明. */
			getSkTextBySkillName(m_arrpCurSk[s]->szarrSkill, &pTmpSkText);
			TH_PROCESS_ERROR(pTmpSkText);
			pTmpSkText->ptBg->pSpCharacter->setVisible(bRet);
			pTmpSkText->pLbTitleText->setVisible(bRet);
			pTmpSkText->pLbMainDesc->setVisible(bRet);
			pTmpSkText->pLbSubDesc->setVisible(bRet);

			if (bRet)
			{
				if (0 == strcmp(m_arrpCurSk[s]->szarrSkill, THSK_DEFTOWER_LVUP))
				{
					m_ptLvUpHoverBorder->pSpCharacter->setVisible(bRet);
					m_ptLvUpHoverBorder->pSpCharacter->setPosition(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->getPosition());
					m_ptLvUpHoverBorder->pSpCharacter->setScale(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->getScale() + 0.05f);
				}
				else
				{
					m_ptSkillHoverBorder->pSpCharacter->setVisible(bRet);
					m_ptSkillHoverBorder->pSpCharacter->setPosition(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->getPosition());
					m_ptSkillHoverBorder->pSpCharacter->setScale(m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->getScale() + 0.05f);
				}
				break;
			}
			else
			{
				m_ptLvUpHoverBorder->pSpCharacter->setVisible(THFALSE);
				m_ptSkillHoverBorder->pSpCharacter->setVisible(THFALSE);
			}
		}
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
		bRet = getQmIsPlayAni(TH_ANITAG_SCALEQM);
		if (THFALSE == bRet)
		{
			for (ssize_t i = 0; i < vecAllChild.size(); i++)
			{
				if (m_ptMoveSelectedMouse->pSpCharacter != vecAllChild.at(i))
				{
					vecAllChild.at(i)->setVisible(THFALSE);
				}
			}
		}
		m_emStepUninit = THEM_DELAY_UNINIT_FLAG::FLAG_UNITING;
		break;

	case FLAG_UNITING:
		/* 检查动画是否播放完成. */
		bRet = getQmIsPlayAni(TH_ANITAG_ALLANI);
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

thBool CThDefTowerQuickMenu::updateSkillLvUp(const short sPos)
{
	thBool bRet = THFALSE;
	DEFTOWER_QUICKMENU tDefQm = { NULL, NULL, NULL, THEM_DEFTOWER_TYPE::DEFTOWERTYPE_UNKNOW, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, NULL, 0 };
	THEM_CHARACTER_LEVEL emCurLv = m_pTaget->getDefTowerCurLv();

	bRet = resetChacSkillPrice(m_arrpCurSk[sPos]->szarrSkill, m_arrpCurSk[sPos]->nSkillPrice, &m_arrpCurSk[sPos]->nSkillPrice);
	TH_PROCESS_ERROR(bRet);

	switch (m_pTaget->getDefTowerType())
	{
	case THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS:
	{
		tDefQm.emCharacterLevel = emCurLv;
		tDefQm.emTowerType = THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS;
		bRet = createQmWarrior(emCurLv, 0.f, 0.f, 0.f, &tDefQm, m_pTaget);
		TH_PROCESS_ERROR(bRet);
		break;
	}
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
	TH_SKILL_PTR pPlayerSk = NULL;

	/* debug use. */
	int nPlayerMoney = 1000;
	
	/* 更新技能图标.*/
	for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)
	{
		if (NULL != m_arrpCurSk[s] && NULL != m_pTaget)
		{
			m_pTaget->getCurSkillByName(m_arrpCurSk[s]->szarrSkill, &pPlayerSk);
			TH_PROCESS_ERROR(pPlayerSk);

			if (nPlayerMoney >= m_arrpCurSk[s]->nSkillPrice)
			{
				m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->setSpriteFrame(m_arrpCurSk[s]->pSpFrameActiveSkill);
			}
			else
			{
				m_arrpCurSk[s]->pChacFrSkill->pSpCharacter->setSpriteFrame(m_arrpCurSk[s]->pSpFrameDisableSkill);
			}
		}
	}

	if (THFALSE == getQmIsPlayAni(TH_ANITAG_TOWERSK_LVUP))
	{
		m_ptTowerSkillLevelUp->pSpCharacter->setVisible(THFALSE);
	}

	bRet = delayUninitMonitoring();
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	ASSERT(bRet);
	return;
}
