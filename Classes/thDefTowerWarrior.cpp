/********************************************************
* Filename		: thDefTowerWarrior.cpp
* Creator		: ac
* Date time		: 2024.05.08
* Description	: def tower warrior event.
********************************************************/


#include "thCcDefTower.h"
#include "thCcAnimation.h"
#include "thCcDbg.h"
#include "thBaseMacro.h"


CThDefTowerWarrior::CThDefTowerWarrior()
{
}

CThDefTowerWarrior::~CThDefTowerWarrior()
{
}

thBool CThDefTowerWarrior::init(
	const CHARACTER_DESC_PTR cptSpDesc,
	const short csSpArrVacantPos,
	const float cfWarriorsBirthAngle,
	const float cfWarriorsBirthX,
	const float cfWarriorsBirthY,
	float* arrfDstXY,
	const CHARACTER_ANI_MAP_PTR cptAniMap,
	const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo
)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	EventListenerMouse* pMouse = EventListenerMouse::create();
	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTowerWarrior::onMouseUp, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTowerWarrior::onMouseDown, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTowerWarrior::onMouseMove, this);

	m_ptAniMap = cptAniMap;
	m_bIsFlip = THFALSE;
	m_ptWarriorFrameInfo = NULL;
	m_fWarriorBirthMoveAngle = cfWarriorsBirthAngle;
	m_fWarriorBirthX = cfWarriorsBirthX;
	m_fWarriorBirthY = cfWarriorsBirthY;
	m_arrfWarriorMovePos = arrfDstXY;
	m_fsmWarriorObject = THNEW_CLASS(CThFSMCharacter);
	m_emCurFsmStatus = THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND;

	bFnRet = initFsmEvent();
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initWarriors(cptSpDesc, csSpArrVacantPos, cptAniMoveTo);
	TH_PROCESS_ERROR(bFnRet);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_ptWarriorFrameInfo->pSpCharacter);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerWarrior::uninit()
{
	unscheduleUpdate();

	m_fsmWarriorObject->uninit();
	THDELETE(m_fsmWarriorObject);

	this->removeAllChildrenWithCleanup(THTRUE);
	this->removeFromParentAndCleanup(THTRUE);
	THFREE(m_ptWarriorFrameInfo);

	return;
}

thBool CThDefTowerWarrior::initFsmEvent()
{
	thBool bRet = THFALSE;
	const short csMaxFsmEvent = 3;
	THFSM_CHARACTER_DESC tFsmEvnStand =
	{
		THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND,
		"defTower warrior stand.",
		fsmEventInitStand,
		fsmEventUpdateStand,
		fsmEventReleaseStand
	};
	THFSM_CHARACTER_DESC tFsmEvnMove =
	{
		THEM_CHARACTER_FSM_EVENT::FSM_EVENT_MOVE,
		"defTower warrior move.",
		fsmEventInitMove,
		fsmEventUpdateMove,
		fsmEventReleaseMove
	};
	THFSM_CHARACTER_DESC tFsmEvnDie =
	{
		THEM_CHARACTER_FSM_EVENT::FSM_EVENT_DIE,
		"defTower warrior die.",
		fsmEventInitDie,
		fsmEventUpdateDie,
		fsmEventReleaseDie
	};
	THFSM_CHARACTER_DESC_PTR arrptCharacterFsmEvent[csMaxFsmEvent] = { &tFsmEvnStand, &tFsmEvnMove, &tFsmEvnDie };

	bRet = m_fsmWarriorObject->init(arrptCharacterFsmEvent, csMaxFsmEvent, this);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::initWarriors(const CHARACTER_DESC_PTR cptSpDesc, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	Sequence* pSeMoveTo = NULL;
	float fDstX = 0.f;
	float fDstY = 0.f;
	float fInPositionTimeDeviation = 0.f;

	fDstX = m_arrfWarriorMovePos[0];
	fDstY = m_arrfWarriorMovePos[1];

	bFnRet = initCharacterWithPlist(cptSpDesc, &m_ptWarriorFrameInfo);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = usSetWarriorMove(fDstX, fDstY, csSpArrVacantPos, cptAniMoveTo);
	TH_PROCESS_ERROR(bFnRet);

	/* 计算下从防御塔到目的地点的时间, 加上这个偏差求完整初始化完成时间. */
	getCharacterMoveSpeed(
		m_ptWarriorFrameInfo->pSpCharacter->getPositionX(),
		m_ptWarriorFrameInfo->pSpCharacter->getPositionY(),
		fDstX,
		fDstY,
		m_ptWarriorFrameInfo->emMoveSpeed,
		&fInPositionTimeDeviation
	);
	m_dInPositionTime = time(NULL) + fInPositionTimeDeviation;

	scheduleUpdate();

	this->addChild(m_ptWarriorFrameInfo->pSpCharacter);
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerWarrior::getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{
	*ppRet = m_ptWarriorFrameInfo;
	return;
}

const float CThDefTowerWarrior::getWarriorBirthMoveAngle() const
{
	return m_fWarriorBirthMoveAngle;
}

const float CThDefTowerWarrior::getWarriorInPositionTime() const
{
	return m_dInPositionTime;
}

void CThDefTowerWarrior::getWarriorAniMap(CHARACTER_ANI_MAP_PTR* ppRet) const
{
	*ppRet = m_ptAniMap;
	return;
}

void CThDefTowerWarrior::getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
{
}

void CThDefTowerWarrior::getWarriorFsmStatus(enum THEM_CHARACTER_FSM_EVENT* pemRet) const
{
	*pemRet = m_emCurFsmStatus;
	return;
}

void CThDefTowerWarrior::setWarriorBirthMoveAngle(const float fAngle)
{
	m_fWarriorBirthMoveAngle = fAngle;
	return;
}

thBool CThDefTowerWarrior::usSetWarriorMove(const float cfDstX, const float cfDstY, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	float fMoveTime = 0.f;
	void* varrpFsmRet[2] = { 0 };
	Sequence* pSeMoveTo = NULL;
	m_csSpArrVacantPos = csSpArrVacantPos;

	getCharacterMoveSpeed(
		m_ptWarriorFrameInfo->pSpCharacter->getPositionX(),
		m_ptWarriorFrameInfo->pSpCharacter->getPositionY(),
		cfDstX,
		cfDstY,
		m_ptWarriorFrameInfo->emMoveSpeed,
		&fMoveTime
	);
	bFnRet = CThBaseCharacterAction::getInstance()->createActionMoveTo(fMoveTime, cfDstX, cfDstY, NULL, 0, &pSeMoveTo);
	TH_PROCESS_ERROR(bFnRet);

	// 判断左移还是右移
	if (m_fWarriorBirthX > cfDstX)
	{
		// 左.
		m_ptWarriorFrameInfo->pSpCharacter->setFlippedX(true);
	}
	else
	{
		// 右.
	}

	if (NULL != varrpFsmRet)
	{
		varrpFsmRet[0] = pSeMoveTo;
		varrpFsmRet[1] = cptAniMoveTo;
	}

	bFnRet = m_fsmWarriorObject->switchEvent(m_emCurFsmStatus, THEM_CHARACTER_FSM_EVENT::FSM_EVENT_MOVE, varrpFsmRet);
	TH_PROCESS_ERROR(bFnRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::usSetWarriorDie(const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	m_csSpArrVacantPos = csSpArrVacantPos;
	void* varrpFsmRet[2] = { NULL, cptAniMoveTo };

	bFnRet = m_fsmWarriorObject->switchEvent(m_emCurFsmStatus, THEM_CHARACTER_FSM_EVENT::FSM_EVENT_DIE, varrpFsmRet);
	TH_PROCESS_ERROR(bFnRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTowerWarrior::setCurFsmStatus(enum THEM_CHARACTER_FSM_EVENT emStatus)
{
	m_emCurFsmStatus = emStatus;
	return;
}

void CThDefTowerWarrior::onMouseUp(EventMouse* pEvent)
{
	thBool bRet = THFALSE;

	bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(pEvent->getLocationInView());
	if (bRet)
	{

#ifdef _DEBUG
		bRet = TDBG_CHARACTER::getInstance()->setDbgFrameInfo(m_ptWarriorFrameInfo);
		ASSERT(bRet);
#endif // _DEBUG
	}

	return;
}

void CThDefTowerWarrior::onMouseDown(EventMouse* pEvent)
{

}

void CThDefTowerWarrior::onMouseMove(EventMouse* pEvent)
{

}

thBool CThDefTowerWarrior::globalMonitoring()
{
	return THTRUE;
}

void CThDefTowerWarrior::update(float dt)
{
	m_fsmWarriorObject->main(m_emCurFsmStatus);
	return;
}

thBool CThDefTowerWarrior::fsmEventInitStand(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);
	pEnv->m_tLastStandFlipTime = time(NULL);

	pEnv->setCurFsmStatus(THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND);
	setPlayerStopAllAction(pEnv->m_ptWarriorFrameInfo->pSpCharacter);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventUpdateStand(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);
	time_t llCurTime = time(NULL);
	float fTimeDiff = (float)difftime(llCurTime, pEnv->m_tLastStandFlipTime);
	static float s_fTimeCond = rand() % 3 + 2;

	if (s_fTimeCond <= fTimeDiff)
	{
		/* 哨兵放哨动作. */
		pEnv->m_ptWarriorFrameInfo->pSpCharacter->setFlippedX(!pEnv->m_bIsFlip);
		pEnv->m_bIsFlip = !pEnv->m_bIsFlip;
		pEnv->m_tLastStandFlipTime = time(NULL);
		s_fTimeCond = rand() % 3 + 5;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventReleaseStand(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);

	CThBaseCharacter::setPlayerStopAllAction(pEnv->m_ptWarriorFrameInfo->pSpCharacter);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventInitMove(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);
	Sequence* pSeMoveTo = static_cast<Sequence*>(parrArgs[0]);
	const CHARACTER_ANI_FRAMEINFO_PTR cptAni = static_cast<CHARACTER_ANI_FRAMEINFO_PTR>(parrArgs[1]);

	pEnv->setCurFsmStatus(THEM_CHARACTER_FSM_EVENT::FSM_EVENT_MOVE);
	pEnv->m_ptWarriorFrameInfo->pSpCharacter->runAction(cptAni->pAnimate->clone());
	pEnv->m_ptWarriorFrameInfo->pSpCharacter->runAction(pSeMoveTo);
	pSeMoveTo->setTag(pEnv->m_csSpArrVacantPos);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventUpdateMove(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);

	if (NULL == pEnv->m_ptWarriorFrameInfo->pSpCharacter->getActionByTag(pEnv->m_csSpArrVacantPos))
	{
		CHACFSM_SWITCH_STAND(pEnv, m_fsmWarriorObject);
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventReleaseMove(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventInitDie(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);
	const CHARACTER_ANI_FRAMEINFO_PTR cptAni = static_cast<CHARACTER_ANI_FRAMEINFO_PTR>(parrArgs[1]);
	Animate* pAniClone = cptAni->pAnimate->clone();

	pEnv->setCurFsmStatus(THEM_CHARACTER_FSM_EVENT::FSM_EVENT_DIE);
	pEnv->m_ptWarriorFrameInfo->pSpCharacter->runAction(pAniClone);
	pAniClone->setTag(pEnv->m_csSpArrVacantPos);

	bRet = THTRUE;
Exit0:
	return bRet;

}

thBool CThDefTowerWarrior::fsmEventUpdateDie(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);

	if (NULL == pEnv->m_ptWarriorFrameInfo->pSpCharacter->getActionByTag(pEnv->m_csSpArrVacantPos))
	{
		CHACFSM_SWITCH_STAND(pEnv, m_fsmWarriorObject);
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventReleaseDie(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior* pEnv = static_cast<CThDefTowerWarrior*>(vpEnv);

	pEnv->m_ptWarriorFrameInfo->nHP = -1;

	bRet = THTRUE;
Exit0:
	return bRet;
}
