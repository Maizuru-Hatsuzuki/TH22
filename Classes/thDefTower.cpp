/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower.
********************************************************/

#include "thDefTower.h"
#include "thBaseAnimation.h"
#include "thBaseMacro.h"


CThDefTower::CThDefTower()
{
}

CThDefTower::~CThDefTower()
{
}

thBool CThDefTower::init(
	const char* cszpBasicCharacterDescPath,
	const char* cszpBulletDescPath,
	const char** cszarrpAniDesc,
	const short csAniDescSize,
	const DEFTOWER_WARRIORS_PTR ptWarriors
)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { m_sVacantPos, };
	EventListenerMouse* pMouse = EventListenerMouse::create();
	CHARACTER_DESC_PTR ptCharacterDesc = NULL;

	m_sVacantPos = 0;
	m_dLastSummonWarriors = 0.f;
	m_dLastAttack = 0.f;
	m_ptBulletDesc = NULL;
	m_ptTowerStatus = NULL;
	m_tAniTag.nOffset = THMAX_ANI_COUNT;
	/* AniTagTowerSummon nTag = 33 */
	m_tAniTag.arrTag[0].nTag = m_tAniTag.nOffset + 1;
	m_tAniTag.arrTag[0].cszpDesc = "AniTagTowerSummon";
	/* AniTagWarriorsDie nTag = 34 */
	m_tAniTag.arrTag[1].nTag = m_tAniTag.nOffset + 2;
	m_tAniTag.arrTag[1].cszpDesc = "AniTagWarriorsDie";

	bFnRet = CThCharacterLoadHandler::getInstance()->getCharaterDescFromIni(cszpBasicCharacterDescPath, &ptCharacterDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CThCharacterLoadHandler::getInstance()->getCharaterDescFromIni(cszpBulletDescPath, &m_ptBulletDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CThCharacterLoadHandler::getInstance()->getDefTowerDescFromIni(cszpBasicCharacterDescPath, &m_ptTowerStatus);
	TH_PROCESS_ERROR(bFnRet);

	m_arrpSpGroup = THMALLOC(CHARACTER_FRAMEINFO_PTR, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	TH_PROCESS_ERROR(m_arrpSpGroup);
	m_arrpAniGroup = THMALLOC(CHARACTER_ANI_FRAMEINFO_PTR, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	TH_PROCESS_ERROR(m_arrpAniGroup);
	m_arrpWarriors = THMALLOC(CThDefTowerWarrior_ptr, sizeof(CThDefTowerWarrior_ptr) * THMAX_DEFTOWER_TARLEVEL_WARRIORS);
	TH_PROCESS_ERROR(m_arrpWarriors);

	memset(m_arrpSpGroup, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	memset(m_arrpAniGroup, 0, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	memset(m_arrpWarriors, 0, sizeof(CThDefTowerWarrior_ptr) * THMAX_DEFTOWER_TARLEVEL_WARRIORS);

	bFnRet = initCharacter(ptCharacterDesc, &m_pTower, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	m_pTower->emCurLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1;
	bFnRet = initBaiscAnimate(cszarrpAniDesc, csAniDescSize);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initDefTowerWarriorsDesc(ptWarriors);
	TH_PROCESS_ERROR(bFnRet);

	/* 播放开门动画并初始化战士精灵. */
	bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initWarriors(m_ptTowerStatus->sMaxWarriors, m_sVacantPos);
	TH_PROCESS_ERROR(bFnRet);

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTower::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTower::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTower::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_pTower->pSpCharacter);

	m_pBatchNodeBullet = SpriteBatchNode::create(m_ptBulletDesc->szarrSpriteTexPath);
	TH_PROCESS_ERROR(m_pBatchNodeBullet);

	scheduleUpdate();

	this->addChild(m_pTower->pSpCharacter);
	this->addChild(m_pBatchNodeBullet);

	bRet = THTRUE;
Exit0:
	CThCharacterLoadHandler::getInstance()->uninitCharacterDesc(ptCharacterDesc);
	return bRet;
}

thBool CThDefTower::initCharacterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos)
{
	thBool bRet = THFALSE;

	bRet = initCharacterAnimation(pAniDesc, &m_arrpAniGroup[cnGroupPos]->pAnimate);
	TH_PROCESS_ERROR(bRet);
	
	bRet = THTRUE;
Exit0:
	
	return bRet;
}

thBool CThDefTower::initDefTowerWarriorsDesc(const DEFTOWER_WARRIORS_PTR ptWarriors)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_DESC_PTR pSpDesc = NULL;
	CHARACTER_ANI_MAP_PTR pAniMap = NULL;

	for (short i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		if (i < ptWarriors->csSize && NULL != ptWarriors->arrpTowerWarriorsDesc[i])
		{
			pSpDesc = THMALLOC(CHARACTER_DESC, sizeof(CHARACTER_DESC));
			TH_PROCESS_ERROR(pSpDesc);
			pAniMap = THMALLOC(CHARACTER_ANI_MAP, sizeof(CHARACTER_ANI_MAP));
			TH_PROCESS_ERROR(pAniMap);

			memcpy_s(pSpDesc, sizeof(CHARACTER_DESC), ptWarriors->arrpTowerWarriorsDesc[i], sizeof(CHARACTER_DESC));
			memcpy_s(pAniMap, sizeof(CHARACTER_ANI_MAP), ptWarriors->arrpTowerWarriorsDesc[i]->ptAniMap, sizeof(CHARACTER_ANI_MAP));

			pSpDesc->ptAniMap = pAniMap;
			m_arrpWarriorsDesc[ptWarriors->emLevel][i] = pSpDesc;
		}
		else
		{
			m_arrpWarriorsDesc[ptWarriors->emLevel][i] = NULL;
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initBaiscAnimate(const char** cszarrpAniDesc, const short csSize)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_ANI_DESC_PTR ptmpAniDesc = NULL;
	CHARACTER_ANI_FRAMEINFO_PTR pResAni = NULL;

	TH_PROCESS_SUCCESS(THMAX_ANI_COUNT < csSize);
	for (int i = 0; i < csSize; i++)
	{
		if (NULL != cszarrpAniDesc[i])
		{
			bFnRet = CThCharacterLoadHandler::getInstance()->getCharacterAniDescFromIni(cszarrpAniDesc[i], &ptmpAniDesc);
			TH_PROCESS_ERROR(bFnRet);
			pResAni = THMALLOC(CHARACTER_ANI_FRAMEINFO, sizeof(CHARACTER_ANI_FRAMEINFO));
			TH_PROCESS_ERROR(pResAni);
			pResAni->pAnimate = NULL;
			strcpy_s(pResAni->szarrDesc, strlen(ptmpAniDesc->szarrAniDesc) + 1, ptmpAniDesc->szarrAniDesc);
			m_arrpAniGroup[i] = pResAni;

			bFnRet = initCharacterAnimate(ptmpAniDesc, i);
			TH_PROCESS_ERROR(bFnRet);

			CThCharacterLoadHandler::getInstance()->uninitCharacterAniDesc(ptmpAniDesc);
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initWarriors(const short csCnt, short sSpArrVacantPos)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	char szarrTmpSpName[32] = { 0 };
	short sWarriorType = 0;
	CHARACTER_DESC_PTR ptSpDesc = NULL;
	CHARACTER_ANI_FRAMEINFO_PTR ptAniMoveTo = NULL;
	CThDefTowerWarrior* ptmpWarrior = NULL;

	/* debug use */
	static float fWarriorsBirthAngle = 270.f;

	TH_PROCESS_SUCCESS(THMAX_DEFTOWER_TARLEVEL_WARRIORS < csCnt);
	for (int i = 0; i < csCnt; i++)
	{
		/* 重建精灵. */
		while (NULL == ptSpDesc)
		{
			sWarriorType = (rand() % THMAX_DEFTOWER_TARLEVEL_WARRIORS);
			ptSpDesc = m_arrpWarriorsDesc[m_pTower->emCurLevel][sWarriorType];
		}
		bFnRet = _getWarArrayVacantPos(&sSpArrVacantPos);
		/* 检查是否满容量. */
		TH_PROCESS_SUCCESS(bFnRet);
		getAniFrameInfoByTag(ptSpDesc->ptAniMap->szarrAniMoveTransverse, &ptAniMoveTo);
		TH_PROCESS_ERROR(ptAniMoveTo);
		getWarriorExistsByAngle(fWarriorsBirthAngle, &bFnRet);
		if (THTRUE == bFnRet)
		{
			fWarriorsBirthAngle += 15.f;
			fWarriorsBirthAngle = fmodf(fWarriorsBirthAngle, 360.f);
		}

		ptmpWarrior = THNEW_CLASS(CThDefTowerWarrior);
		TH_PROCESS_ERROR(ptmpWarrior);
		bFnRet = ptmpWarrior->init(
			ptSpDesc,
			sSpArrVacantPos,
			fWarriorsBirthAngle,
			m_pTower->pSpCharacter->getPositionX(),
			m_pTower->pSpCharacter->getPositionY(),
			m_pTower->nAttackRadius,
			ptAniMoveTo
		);
		TH_PROCESS_ERROR(bFnRet);
		this->addChild(ptmpWarrior);

		m_arrpWarriors[sSpArrVacantPos] = ptmpWarrior;
		m_dLastSummonWarriors = time(NULL);

		m_ptTowerStatus->sCurWarriors++;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initBullet(float fShootAngle)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR ptFrBullet = NULL;
	Sequence* pSeBulletMove = NULL;
	FiniteTimeAction* arrpActionCallback[2] = { 0 };
	float fMoveDstX = 0.f;
	float fMoveDstY = 0.f;
	float fMoveTime = 0.f;
	short sVancantPos = 0;

	bFnRet = _getSpArrayVacantPos(&sVancantPos);
	/* 检查是否满容量. */
	TH_PROCESS_SUCCESS(bFnRet);

	bFnRet = initCharacter(m_ptBulletDesc, &ptFrBullet, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	m_arrpSpGroup[sVancantPos] = ptFrBullet;

	ptFrBullet->pSpCharacter->setPosition(m_pTower->pSpCharacter->getPositionX(), m_pTower->pSpCharacter->getPositionY());
	m_pBatchNodeBullet->addChild(ptFrBullet->pSpCharacter);

	fMoveDstX = m_pTower->pSpCharacter->getPositionX() + m_pTower->nAttackRadius * cos(fShootAngle * (M_PI / 180));
	fMoveDstY = m_pTower->pSpCharacter->getPositionY() + m_pTower->nAttackRadius * sin(fShootAngle * (M_PI / 180));

	arrpActionCallback[0] = CallFuncN::create(CC_CALLBACK_1(CThDefTower::uninitBullet, this, sVancantPos));
	getCharacterMoveSpeed(m_pTower->pSpCharacter->getPositionX(), m_pTower->pSpCharacter->getPositionY(), fMoveDstX, fMoveDstY, ptFrBullet->emMoveSpeed, &fMoveTime);
	bFnRet = CThBaseCharacterAction::getInstance()->createActionMoveTo(fMoveTime, fMoveDstX, fMoveDstY, arrpActionCallback, 1, &pSeBulletMove);
	TH_PROCESS_ERROR(bFnRet);
	ptFrBullet->pSpCharacter->runAction(pSeBulletMove);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTower::uninit()
{
	for (int i = 0; i < THMAX_SP_COUNT; i++)
	{
		THFREE(m_arrpSpGroup[i]);
	}
	for (int i = 0; i < THMAX_ANI_COUNT; i++)
	{
		m_arrpAniGroup[i]->pAnimate->release();
		THFREE(m_arrpAniGroup[i]);
	}
	for (int i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		if (NULL != m_arrpWarriors[i])
		{
			m_arrpWarriors[i]->uninit();
			THDELETE(m_arrpWarriors[i]);
		}
	}

	THFREE(m_arrpSpGroup);
	THFREE(m_arrpAniGroup);
	THFREE(m_arrpWarriors);

	return;
}

void CThDefTower::uninitBullet(Node* pNode, const short csBullet)
{
	unscheduleUpdate();
	m_pBatchNodeBullet->removeChild(m_arrpSpGroup[csBullet]->pSpCharacter, THTRUE);
	THFREE(m_arrpSpGroup[csBullet]);
	return;
}

void CThDefTower::getAniTagByDesc(const char* cszpDesc, int* pnRet)
{
	for (short i = 0; i < THMAX_ANI_TAG; i++)
	{
		if (0 == strcmp(m_tAniTag.arrTag[i].cszpDesc, cszpDesc))
		{
			*pnRet = m_tAniTag.arrTag[i].nTag;
			break;
		}
	}
	return;
}

void CThDefTower::getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{
	*ppRet = m_pTower;
	return;
}

void CThDefTower::getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
{
	for (int i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (0 == strcmp(cszpTag, m_arrpSpGroup[i]->szarrDesc))
		{
			*ppRet = m_arrpSpGroup[i];
			break;
		}
	}
	return;
}

void CThDefTower::getAniFrameInfoByTag(const char* cszpTag, CHARACTER_ANI_FRAMEINFO_PTR* ppRet)
{
	for (short i = 0; i < THMAX_ANI_COUNT; i++)
	{
		if (0 == strcmp(m_arrpAniGroup[i]->szarrDesc, cszpTag))
		{
			*ppRet = m_arrpAniGroup[i];
			break;
		}
	}
	return;
}

void CThDefTower::getWarriorExistsByAngle(const float cfAngle, thBool* pbRet)
{
	*pbRet = THFALSE;
	for (short i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		if (NULL != m_arrpWarriors[i] && cfAngle == m_arrpWarriors[i]->getWarriorBirthMoveAngle())
		{
			*pbRet = THTRUE;
			break;
		}
	}
	return;
}

thBool CThDefTower::_getSpArrayVacantPos(short* psRet)
{
	thBool bFull = THTRUE;

	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (NULL == m_arrpSpGroup[i])
		{
			bFull = THFALSE;
			*psRet = i;
			break;
		}
	}
	return bFull;
}

thBool CThDefTower::_getWarArrayVacantPos(short* psRet)
{
	thBool bFull = THTRUE;

	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (NULL == m_arrpWarriors[i])
		{
			bFull = THFALSE;
			*psRet = i;
			break;
		}
	}
	return bFull;
}

void CThDefTower::setWarriorExistsByAngle(const float cfAngle, const short csTag)
{
	m_arrpWarriors[csTag]->setWarriorBirthMoveAngle(cfAngle);
	return;
}

thBool CThDefTower::setPlayAniTowerSummon(const short* arrnCondAniTag, const short cnSize, const thBool bIsSummoning)
{
	/* arrnCondAniTag: int 数组，代表Action tag, 当此tag指向的action全部播放结束后，才执行关门动画. */
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	static thBool s_bDoorStatus = THFALSE;
	thBool bIsPlayEnd = THTRUE;
	int nAniSummonTag = 0;

	for (int i = 0; i < cnSize; i++)
	{
		if (NULL != getActionByTag(arrnCondAniTag[i]))
		{
			bIsPlayEnd = THFALSE;
			break;
		}
	}

	/* 检查开门动画是否播放完. */
	getAniTagByDesc("AniTagTowerSummon", &nAniSummonTag);
	TH_PROCESS_ERROR(nAniSummonTag);
	if (NULL != m_pTower->pSpCharacter->getActionByTag(nAniSummonTag))
	{
		bIsPlayEnd = THFALSE;
	}
	else
	{
		if (THFALSE == s_bDoorStatus && THTRUE == bIsPlayEnd && THTRUE == bIsSummoning)
		{
			// init.
			bFnRet = _setPlayAniOpenTheDoor();
			TH_PROCESS_ERROR(bFnRet);
			s_bDoorStatus = THTRUE;
		}
		else if (THTRUE == s_bDoorStatus && THTRUE == bIsPlayEnd && THFALSE == bIsSummoning)
		{
			// release.
			bFnRet = _setPlayAniCloseTheDoor();
			TH_PROCESS_ERROR(bFnRet);
			s_bDoorStatus = THFALSE;
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_setPlayAniOpenTheDoor()
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniOpDor = NULL;
	int nAniSummonTag = 0;
	
	getAniTagByDesc("AniTagTowerSummon", &nAniSummonTag);
	TH_PROCESS_ERROR(nAniSummonTag);
	getAniFrameInfoByTag("TowerInitWarriorsOpenDoor", &pAniOpDor);
	TH_PROCESS_ERROR(pAniOpDor);
	m_pTower->pSpCharacter->runAction(pAniOpDor->pAnimate);
	pAniOpDor->pAnimate->setTag(nAniSummonTag);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_setPlayAniCloseTheDoor()
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniClDor = NULL;
	int nAniSummonTag = 0;

	getAniTagByDesc("AniTagTowerSummon", &nAniSummonTag);
	TH_PROCESS_ERROR(nAniSummonTag);
	getAniFrameInfoByTag("TowerInitWarriorsCloseDoor", &pAniClDor);
	TH_PROCESS_ERROR(pAniClDor);
	m_pTower->pSpCharacter->runAction(pAniClDor->pAnimate);
	pAniClDor->pAnimate->setTag(nAniSummonTag);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_setPlayAniWarriorsDie(const short sPos, CThDefTowerWarrior_ptr pSp)
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniDie = NULL;
	int nAniWarriorDie = 0;
	short sVancatPos = 0;

	getAniTagByDesc("AniTagWarriorsDie", &nAniWarriorDie);
	TH_PROCESS_ERROR(nAniWarriorDie);
	getAniFrameInfoByTag("TowerWarriorsDie", &pAniDie);
	TH_PROCESS_ERROR(pAniDie);
	bRet = pSp->usSetWarriorDie(sPos, pAniDie);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::execTowerShoot(const short csBulletCnt)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	static float s_fAngle = 300.f;
	static short s_sShot = 0;

	if (m_pTower->nAttackCD < time(NULL) - m_dLastAttack)
	{
		switch (m_ptTowerStatus->emBulletType)
		{
		case THEM_BULLET_TYPE::SHOOTCHAT_TRACKING:
			break;

		case THEM_BULLET_TYPE::SHOOTCHAT_WINDMILL:
			m_ptBulletDesc->emMoveSpeed = THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_HIGHEX;
			break;

		case THEM_BULLET_TYPE::SHOOTCHAT_RING:
			break;

		default:
			break;
		}

		s_fAngle += 360.f / csBulletCnt;
		s_fAngle = fmodf(s_fAngle, 360.f);
		bFnRet = initBullet(s_fAngle);
		TH_PROCESS_ERROR(bFnRet);
		s_sShot++;
		
		if (csBulletCnt == s_sShot)
		{
			m_dLastAttack = time(NULL);
			s_sShot = 0;
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTower::onMouseUp(EventMouse* pEvent)
{
}

void CThDefTower::onMouseDown(EventMouse* pEvent)
{
	return;
}

void CThDefTower::onMouseMove(EventMouse* pEvent)
{

}

void CThDefTower::update(float dt)
{
	thBool bFnRet = THFALSE;

	bFnRet = globalMonitoring();
	ASSERT(bFnRet);

	bFnRet = execTowerShoot(m_ptTowerStatus->sMaxBullets);
	ASSERT(bFnRet);
	
	return;
}

thBool CThDefTower::globalMonitoring()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	
	bFnRet = globalMonitoringWarriors();
	TH_PROCESS_ERROR(bFnRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::globalMonitoringWarriors()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CThDefTowerWarrior_ptr ptmpSpWarr = NULL;
	const thBool bIsNeedSummon = m_ptTowerStatus->sMaxBullets > m_ptTowerStatus->sCurWarriors;
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { m_sVacantPos, };

	bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THFALSE);
	TH_PROCESS_ERROR(bFnRet);

	// 获取防御塔战士精灵信息并做一些健康检查.
	for (short i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		ptmpSpWarr = m_arrpWarriors[i];
		if (NULL != ptmpSpWarr)
		{
			bFnRet = _monitoringWarriorsHealthy(i, ptmpSpWarr);
			TH_PROCESS_ERROR(bFnRet);
		}
	}

	// 检查是否需要创建精灵：检查HP，检查重生CD，检查精灵数组内容是否为空.
	if (bIsNeedSummon && m_ptTowerStatus->sSummonWarriorsCD <= time(NULL) - m_dLastSummonWarriors)
	{
		bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THTRUE);
		TH_PROCESS_ERROR(bFnRet);
		bFnRet = initWarriors(1, m_sVacantPos);
		TH_PROCESS_ERROR(bFnRet);
		m_ptTowerStatus->sCurWarriors++;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_monitoringWarriorsHealthy(const short sPos, CThDefTowerWarrior_ptr pSp)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR pSpFrame = NULL;
	double dWarriorInPositionTime = pSp->getWarriorInPositionTime();

	pSp->getCharacterFrameInfo(&pSpFrame);
	TH_PROCESS_ERROR(pSpFrame);

	if (0 >= pSpFrame->nHP)
	{
		/* 精灵死亡, 释放. */
		bFnRet = _setPlayAniWarriorsDie(sPos, pSp);
		TH_PROCESS_ERROR(bFnRet);

		pSp->uninit();
		this->removeChild(pSp);
		THDELETE(pSp);

		m_ptTowerStatus->sCurWarriors--;
		m_dLastDieWarriors = time(NULL);
	}
	else if (pSpFrame->nSupportDuration < time(NULL) - dWarriorInPositionTime)
	{
		/* 精灵超时, 释放. */
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}


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
	const short csActionRadius,
	const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo
)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	m_ptWarriorFrameInfo = NULL;
	m_fWarriorBirthMoveAngle = cfWarriorsBirthAngle;
	m_fWarriorBirthX = cfWarriorsBirthX;
	m_fWarriorBirthY = cfWarriorsBirthY;
	m_sActionRadius = csActionRadius;
	m_fsmWarriorObject = THNEW_CLASS(CThFSMCharacter);
	m_emCurFsmStatus = THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND;

	bFnRet = initFsmEvent();
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initWarriors(cptSpDesc, csSpArrVacantPos, cptAniMoveTo);
	TH_PROCESS_ERROR(bFnRet);

	bRet = THTRUE;
Exit0:
	return bRet;
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

	fDstX = m_fWarriorBirthX + m_sActionRadius * cos(m_fWarriorBirthMoveAngle * (M_PI / 180));
	fDstY = m_fWarriorBirthY + m_sActionRadius * sin(m_fWarriorBirthMoveAngle * (M_PI / 180));
	
	bFnRet = initCharacter(cptSpDesc, &m_ptWarriorFrameInfo, THTRUE);
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

void CThDefTowerWarrior::uninit()
{
	unscheduleUpdate();

	m_fsmWarriorObject->uninit();
	THDELETE(m_fsmWarriorObject);

	this->removeAllChildrenWithCleanup(THTRUE);
	THFREE(m_ptWarriorFrameInfo);

	return;
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

void CThDefTowerWarrior::getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
{
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
	}
	else
	{
		// 右.
		m_ptWarriorFrameInfo->pSpCharacter->setFlippedX(true);
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

	pEnv->setCurFsmStatus(THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventUpdateStand(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::fsmEventReleaseStand(void* vpEnv, void** parrArgs)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;

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

	bRet = THTRUE;
Exit0:
	return bRet;
}
