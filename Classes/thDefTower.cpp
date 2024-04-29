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
	const CHARACTER_DESC_PTR pTowerDesc,
	CHARACTER_ANI_DESC_PTR* arrpAniDesc,
	DEFTOWER_DESC_PTR ptTowerDesc,
	const CHARACTER_DESC_PTR* arrpTowerWarriorsDesc,
	enum THEM_CHARACTER_LEVEL emLevel,
	const short csTowerWarriosSize
)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	EventListenerMouse* pMouse = EventListenerMouse::create();
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { m_sVacantPos, };

	m_sVacantPos = 0;
	m_dLastSummonWarriors = 0.f;
	m_tAniTag.nOffset = THMAX_ANI_COUNT;
	/* nTag = 33 */
	m_tAniTag.arrTag[0].nTag = m_tAniTag.nOffset + 1;
	m_tAniTag.arrTag[0].cszpDesc = "AniTagTowerSummon";
	/* nTag = 34 */
	m_tAniTag.arrTag[1].nTag = m_tAniTag.nOffset + 2;
	m_tAniTag.arrTag[1].cszpDesc = "AniTagWarriorsDie";

	m_ptTowerStatus = THMALLOC(DEFTOWER_DESC, sizeof(DEFTOWER_DESC));
	TH_PROCESS_ERROR(m_ptTowerStatus);
	m_arrpSpGroup = THMALLOC(CHARACTER_FRAMEINFO_PTR, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	TH_PROCESS_ERROR(m_arrpSpGroup);
	m_arrpAniGroup = THMALLOC(CHARACTER_ANI_FRAMEINFO_PTR, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	TH_PROCESS_ERROR(m_arrpAniGroup);
	m_arrpWarriors = THMALLOC(CThDefTowerWarrior_ptr, sizeof(CThDefTowerWarrior_ptr) * THMAX_DEFTOWER_TARLEVEL_WARRIORS);
	TH_PROCESS_ERROR(m_arrpWarriors);

	memcpy_s(m_ptTowerStatus, sizeof(DEFTOWER_DESC), ptTowerDesc, sizeof(DEFTOWER_DESC));
	memset(m_arrpSpGroup, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	memset(m_arrpAniGroup, 0, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	memset(m_arrpWarriors, 0, sizeof(CThDefTowerWarrior_ptr) * THMAX_DEFTOWER_TARLEVEL_WARRIORS);

	bFnRet = initCharater(pTowerDesc, &m_pTower, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initBaiscAnimate(arrpAniDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initDefTowerWarriorsDesc(arrpTowerWarriorsDesc, emLevel, csTowerWarriosSize);
	TH_PROCESS_ERROR(bFnRet);
	/* 播放开门动画并初始化战士精灵. */
	bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initWarriors(ptTowerDesc->csMaxWarriors, m_sVacantPos);
	TH_PROCESS_ERROR(bFnRet);
	m_ptTowerStatus->sCurWarriors = ptTowerDesc->csMaxWarriors;

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTower::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTower::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTower::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_pTower->pSpCharacter);

	scheduleUpdate();

	this->addChild(m_pTower->pSpCharacter);
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initCharaterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos)
{
	thBool bRet = THFALSE;

	bRet = initCharaterAnimation(pAniDesc, &m_arrpAniGroup[cnGroupPos]->pAnimate);
	TH_PROCESS_ERROR(bRet);
	
	bRet = THTRUE;
Exit0:
	
	return bRet;
}

thBool CThDefTower::initDefTowerWarriorsDesc(const CHARACTER_DESC_PTR* arrpTowerWarriorsDesc, enum THEM_CHARACTER_LEVEL emLevel, const short csSize)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_DESC_PTR pSpDesc = NULL;
	CHARACTER_ANI_MAP_PTR pAniMap = NULL;

	for (short i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		if (i < csSize)
		{
			pSpDesc = THMALLOC(CHARACTER_DESC, sizeof(CHARACTER_DESC));
			TH_PROCESS_ERROR(pSpDesc);
			pAniMap = THMALLOC(CHARACTER_ANI_MAP, sizeof(CHARACTER_ANI_MAP));
			TH_PROCESS_ERROR(pAniMap);

			memcpy_s(pSpDesc, sizeof(CHARACTER_DESC), arrpTowerWarriorsDesc[i], sizeof(CHARACTER_DESC));
			memcpy_s(pAniMap, sizeof(CHARACTER_ANI_MAP), arrpTowerWarriorsDesc[i]->ptAniMap, sizeof(CHARACTER_ANI_MAP));

			m_arrpWarriorsDesc[emLevel][i] = pSpDesc;
			pSpDesc->ptAniMap = pAniMap;
		}
		else
		{
			m_arrpWarriorsDesc[emLevel][i] = NULL;
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initBaiscAnimate(CHARACTER_ANI_DESC_PTR* arrpAniDesc)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_ANI_DESC_PTR ptmpAniDesc = NULL;
	CHARACTER_ANI_FRAMEINFO_PTR pResAni = NULL;

	for (int i = 0; i < THMAX_ANI_COUNT; i++)
	{
		ptmpAniDesc = arrpAniDesc[i];
		if (NULL != ptmpAniDesc)
		{
			pResAni = THMALLOC(CHARACTER_ANI_FRAMEINFO, sizeof(CHARACTER_ANI_FRAMEINFO));
			pResAni->pAnimate = NULL;
			strcpy_s(pResAni->szarrDesc, strlen(ptmpAniDesc->szarrAniDesc) + 1, ptmpAniDesc->szarrAniDesc);
			m_arrpAniGroup[i] = pResAni;

			bFnRet = initCharaterAnimate(ptmpAniDesc, i);
			TH_PROCESS_ERROR(bFnRet);
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initWarriors(const short csCnt, short sSpArrVacantPos)
{
	thBool bRet									= THFALSE;
	thBool bFnRet								= THFALSE;
	short sWarriorType							= 0;
	CHARACTER_DESC_PTR ptSpDesc					= NULL;
	CHARACTER_ANI_FRAMEINFO_PTR ptAniMoveTo		= NULL;
	CThDefTowerWarrior* ptmpWarrior				= NULL;

	/* debug use */
	static float fWarriorsBirthAngle = 0.f;

	TH_PROCESS_SUCCESS(THMAX_DEFTOWER_TARLEVEL_WARRIORS < csCnt);
	for (int i = 0; i < csCnt; i++)
	{
		/* 重建精灵. */
		while (NULL == ptSpDesc)
		{
			sWarriorType = (rand() % THMAX_DEFTOWER_TARLEVEL_WARRIORS);
			ptSpDesc = m_arrpWarriorsDesc[m_ptTowerStatus->emCurLevel][sWarriorType];
		}
		_getWarArrayVacantPos(&sSpArrVacantPos);
		getAniFrameInfoByTag(ptSpDesc->ptAniMap->cszpAniMoveTransverse, &ptAniMoveTo);
		TH_PROCESS_ERROR(ptAniMoveTo);
		getWarriorExistsByAngle(fWarriorsBirthAngle, &bFnRet);
		if (THTRUE == bFnRet)
		{
			fWarriorsBirthAngle += 15.f;
			fWarriorsBirthAngle = fmodf(fWarriorsBirthAngle, 360.f);
		}

		ptmpWarrior = new CThDefTowerWarrior;
		TH_PROCESS_ERROR(ptmpWarrior);
		bFnRet = ptmpWarrior->init(
			ptSpDesc, 
			sSpArrVacantPos,
			fWarriorsBirthAngle,
			m_pTower->pSpCharacter->getPositionX(),
			m_pTower->pSpCharacter->getPositionY(), 
			m_ptTowerStatus->sActionRadius, ptAniMoveTo
		);
		TH_PROCESS_ERROR(bFnRet);
		this->addChild(ptmpWarrior);

		m_arrpWarriors[sSpArrVacantPos] = ptmpWarrior;
		m_dLastSummonWarriors = time(NULL);
	}

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

void CThDefTower::getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{
	*ppRet = m_pTower;
	return;
}

void CThDefTower::getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
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

void CThDefTower::_getSpArrayVacantPos(short* psRet)
{
	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (NULL == m_arrpSpGroup[i])
		{
			*psRet = i;
			break;
		}
	}
	return;
}

void CThDefTower::_getWarArrayVacantPos(short* psRet)
{
	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		if (NULL == m_arrpWarriors[i])
		{
			*psRet = i;
			break;
		}
	}
	return;
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

thBool CThDefTower::_setPlayAniWarriorsDie()
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniDie = NULL;
	int nAniWarriorDie = 0;

	getAniTagByDesc("AniTagWarriorsDie", &nAniWarriorDie);
	TH_PROCESS_ERROR(nAniWarriorDie);
	getAniFrameInfoByTag("TowerWarriorsDie", &pAniDie);
	TH_PROCESS_ERROR(pAniDie);
	m_pTower->pSpCharacter->runAction(pAniDie->pAnimate);
	pAniDie->pAnimate->setTag(nAniWarriorDie);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTower::onMouseUp(EventMouse* pEvent)
{
	return;
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
	const thBool bIsNeedSummon = m_ptTowerStatus->csMaxWarriors > m_ptTowerStatus->sCurWarriors;
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { m_sVacantPos, };

	bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THFALSE);
	TH_PROCESS_ERROR(bFnRet);

	// 获取防御塔战士精灵信息并做一些健康检查.
	for (short i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		ptmpSpWarr = m_arrpWarriors[i];
		if (NULL != ptmpSpWarr)
		{
			bFnRet = _monitoringWarriorsHealthy(ptmpSpWarr);
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

thBool CThDefTower::_monitoringWarriorsHealthy(CThDefTowerWarrior_ptr pSp)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR pSpFrame = NULL;

	pSp->getCharaterFrameInfo(&pSpFrame);
	TH_PROCESS_ERROR(pSpFrame);

	if (0 >= pSpFrame->nHP)
	{
		// 精灵死亡，释放.
		bFnRet = _setPlayAniWarriorsDie();
		TH_PROCESS_ERROR(bFnRet);

		pSp->uninit();
		this->removeChild(pSp);
		THDELETE(pSp);

		m_ptTowerStatus->sCurWarriors--;
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
	
	bFnRet = initWarriors(cptSpDesc, csSpArrVacantPos, cptAniMoveTo);
	TH_PROCESS_ERROR(bFnRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTowerWarrior::initWarriors(const CHARACTER_DESC_PTR cptSpDesc, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	float fReinitPosX = 0.f;
	float fReinitPosY = 0.f;
	Sequence* pSeMoveTo = NULL;

	bFnRet = initCharater(cptSpDesc, &m_ptWarriorFrameInfo, THTRUE);
	TH_PROCESS_ERROR(bFnRet);

	fReinitPosX = m_fWarriorBirthX + m_sActionRadius * cos(m_fWarriorBirthMoveAngle * (M_PI / 180));
	fReinitPosY = m_fWarriorBirthY + m_sActionRadius * sin(m_fWarriorBirthMoveAngle * (M_PI / 180));
	bFnRet = CThBaseCharacterAction::getInstance()->createActionMoveTo(1.f, fReinitPosX, fReinitPosY, NULL, &pSeMoveTo);
	TH_PROCESS_ERROR(bFnRet);

	// 判断左移还是右移
	if (m_fWarriorBirthX > fReinitPosX)
	{
		// 左.
	}
	else
	{
		// 右.
		m_ptWarriorFrameInfo->pSpCharacter->setFlippedX(true);
	}

	m_ptWarriorFrameInfo->pSpCharacter->runAction(cptAniMoveTo->pAnimate->clone());
	m_ptWarriorFrameInfo->pSpCharacter->runAction(pSeMoveTo);
	pSeMoveTo->setTag(csSpArrVacantPos);

	this->addChild(m_ptWarriorFrameInfo->pSpCharacter);

	bRet = THTRUE;
Exit0:
	return bRet;
}


void CThDefTowerWarrior::uninit()
{
	this->removeAllChildrenWithCleanup(THTRUE);
	THFREE(m_ptWarriorFrameInfo);
	return;
}

void CThDefTowerWarrior::getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{
	*ppRet = m_ptWarriorFrameInfo;
	return;
}

const float CThDefTowerWarrior::getWarriorBirthMoveAngle() const
{
	return m_fWarriorBirthMoveAngle;
}

void CThDefTowerWarrior::getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)
{}

void CThDefTowerWarrior::setWarriorBirthMoveAngle(const float fAngle)
{
	m_fWarriorBirthMoveAngle = fAngle;
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
