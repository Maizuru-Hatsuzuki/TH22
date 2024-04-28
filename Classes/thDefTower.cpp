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

thBool CThDefTower::init(CHARACTER_DESC_PTR pTowerDesc, CHARACTER_ANI_DESC_PTR* arrpAniDesc, DEFTOWER_DESC_PTR ptTowerDesc)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	EventListenerMouse* pMouse = EventListenerMouse::create();
	m_nAniTagTowerSummon = 33;
	m_ptTowerStatus = THMALLOC(DEFTOWER_DESC, sizeof(DEFTOWER_DESC));
	memcpy_s(m_ptTowerStatus, sizeof(DEFTOWER_DESC), ptTowerDesc, sizeof(DEFTOWER_DESC));

	m_arrpSpGroup = THMALLOC(CHARACTER_FRAMEINFO_PTR, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	TH_PROCESS_ERROR(m_arrpSpGroup);
	m_arrpAniGroup = THMALLOC(CHARACTER_ANI_FRAMEINFO_PTR, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	TH_PROCESS_ERROR(m_arrpAniGroup);

	memset(m_arrpSpGroup, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	memset(m_arrpAniGroup, 0, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);

	bFnRet = initCharater(pTowerDesc, &m_pTower, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	m_pTower->emCharacterType = CHARACTER_DEFTOWER;

	bFnRet = _initBaiscAnimate(arrpAniDesc);
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(m_pTower->pSpCharacter);

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTower::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTower::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTower::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_pTower->pSpCharacter);

	scheduleUpdate();

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

thBool CThDefTower::initDefTowerWarriorsDesc(CHARACTER_DESC_PTR* arrpCharacterDesc, enum THEM_CHARACTER_LEVEL emLevel, const short csSize)
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

			memcpy_s(pSpDesc, sizeof(CHARACTER_DESC), arrpCharacterDesc[i], sizeof(CHARACTER_DESC));
			memcpy_s(pAniMap, sizeof(CHARACTER_ANI_MAP), arrpCharacterDesc[i]->ptAniMap, sizeof(CHARACTER_ANI_MAP));

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

thBool CThDefTower::_initBaiscAnimate(CHARACTER_ANI_DESC_PTR* arrpAniDesc)
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

	THFREE(m_arrpSpGroup);
	THFREE(m_arrpAniGroup);

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

thBool CThDefTower::setPlayAniTowerSummon(const short* arrnCondAniTag, const short cnSize, const thBool bIsSummoning)
{
	/* arrnCondAniTag: int 数组，代表Action tag, 当此tag指向的action全部播放结束后，才执行关门动画. */
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	static thBool s_bDoorStatus = THFALSE;
	thBool bIsPlayEnd = THTRUE;

	for (int i = 0; i < cnSize; i++)
	{
		if (NULL != getActionByTag(arrnCondAniTag[i]))
		{
			bIsPlayEnd = THFALSE;
			break;
		}
	}
	/* 检查开门动画是否播放完. */
	if (NULL != m_pTower->pSpCharacter->getActionByTag(m_nAniTagTowerSummon)) bIsPlayEnd = THFALSE;

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

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_setPlayAniOpenTheDoor()
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniOpDor = NULL;
	
	getAniFrameInfoByTag("TowerInitWarriorsOpenDoor", &pAniOpDor);
	TH_PROCESS_ERROR(pAniOpDor);
	m_pTower->pSpCharacter->runAction(pAniOpDor->pAnimate);
	pAniOpDor->pAnimate->setTag(m_nAniTagTowerSummon);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_setPlayAniCloseTheDoor()
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniClDor = NULL;

	getAniFrameInfoByTag("TowerInitWarriorsCloseDoor", &pAniClDor);
	TH_PROCESS_ERROR(pAniClDor);
	m_pTower->pSpCharacter->runAction(pAniClDor->pAnimate);
	pAniClDor->pAnimate->setTag(m_nAniTagTowerSummon);

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
	
	bFnRet = monitoringFighter();
	TH_PROCESS_ERROR(bFnRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::monitoringFighter()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_DESC_PTR ptSpDesc = NULL;
	CHARACTER_FRAMEINFO_PTR pSp = NULL;
	CHARACTER_ANI_FRAMEINFO_PTR ptAniMoveTo = NULL;
	Sequence* pSeMoveTo = NULL;
	float fReinitPosX = 0.f;
	float fReinitPosY = 0.f;
	short sWarriorsCnt = 0;
	short sWarriorType = 0;
	short sSpArrVacantPos = 0;
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { sSpArrVacantPos, };
	const thBool bIsNeedSummon = m_ptTowerStatus->cnMaxWarriors > m_ptTowerStatus->sCurWarriors;

	/* debug use */
	static float fWarriorsBirthAngle = 35.f;


	// 获取防御塔战士精灵信息并做一些检查.
	for (short i = 0; i < THMAX_SP_COUNT; i++)
	{
		pSp = m_arrpSpGroup[i];
		if (NULL != pSp)
		{
			if (0 >= pSp->nHP)
			{
				// 精灵死亡或超时，释放.
				m_ptTowerStatus->sCurWarriors--;
			}
		}
	}

	pSp = NULL;
	// 检查是否需要创建精灵：检查HP，检查重生CD，检查精灵数组内容是否为空.
	setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, bIsNeedSummon);
	if (bIsNeedSummon)
	{
		while (NULL == ptSpDesc)
		{
			sWarriorType = (rand() % THMAX_DEFTOWER_TARLEVEL_WARRIORS);
			ptSpDesc = m_arrpWarriorsDesc[m_ptTowerStatus->emCurLevel][sWarriorType];
		}
		_getSpArrayVacantPos(&sSpArrVacantPos);

		// 获取需要重建的精灵数量，重建精灵.
		sWarriorsCnt = m_ptTowerStatus->cnMaxWarriors - m_ptTowerStatus->sCurWarriors;
		bFnRet = initCharater(ptSpDesc, &pSp, THTRUE);
		TH_PROCESS_ERROR(bFnRet);
		this->addChild(pSp->pSpCharacter);

		fReinitPosX = m_pTower->pSpCharacter->getPositionX() + m_ptTowerStatus->sActionRadius * cos(fWarriorsBirthAngle * (M_PI / 180));
		fReinitPosY = m_pTower->pSpCharacter->getPositionX() + m_ptTowerStatus->sActionRadius * sin(fWarriorsBirthAngle * (M_PI / 180));
		bFnRet = CThBaseCharacterAction::getInstance()->createActionMoveTo(1.f, fReinitPosX, fReinitPosY, NULL, &pSeMoveTo);
		TH_PROCESS_ERROR(bFnRet);

		// 判断左移还是右移
		if (m_pTower->pSpCharacter->getPositionX() > fReinitPosX)
		{
			// 左.
		}
		else
		{
			// 右.
			pSp->pSpCharacter->setFlippedX(true);
		}
		getAniFrameInfoByTag(ptSpDesc->ptAniMap->cszpAniMoveTransverse, &ptAniMoveTo);
		TH_PROCESS_ERROR(ptAniMoveTo);

		pSp->pSpCharacter->runAction(ptAniMoveTo->pAnimate->clone());
		pSp->pSpCharacter->runAction(pSeMoveTo);
		pSeMoveTo->setTag(sSpArrVacantPos);

		fWarriorsBirthAngle += 25.f;
		m_arrpSpGroup[sSpArrVacantPos] = pSp;

		m_ptTowerStatus->sCurWarriors++;
	}


	bRet = THTRUE;
Exit0:
	return bRet;
}
