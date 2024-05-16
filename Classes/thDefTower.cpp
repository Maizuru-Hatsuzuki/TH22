/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower.
********************************************************/

#include "thCcDefTower.h"
#include "thCcAnimation.h"
#include "thBaseMacro.h"


CThDefTower::CThDefTower()
{
}

CThDefTower::~CThDefTower()
{
}

thBool CThDefTower::init(
	const char* cszpDefTowerCharacterDescPath,
	const char* cszpBulletDescPath,
	const char* cszpBulletPlistPng,
	char** szarrpAniDesc,
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
	m_tAniTag.sOffset = THMAX_ANI_COUNT;
	/* AniTagTowerSummon nTag = 33 */
	m_tAniTag.arrTag[0].sTag = m_tAniTag.sOffset + 1;
	m_tAniTag.arrTag[0].cszpDesc = "AniTagTowerSummon";
	/* AniTagWarriorsDie nTag = 34 */
	m_tAniTag.arrTag[1].sTag = m_tAniTag.sOffset + 2;
	m_tAniTag.arrTag[1].cszpDesc = "AniTagWarriorsDie";

	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(cszpDefTowerCharacterDescPath, &ptCharacterDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(cszpBulletDescPath, &m_ptBulletDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getDefTowerDescFromIni(cszpDefTowerCharacterDescPath, &m_ptTowerStatus);
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

	bFnRet = initCharacterWithPlist(ptCharacterDesc, &m_ptTower);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initBaiscAnimate(szarrpAniDesc, csAniDescSize);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initDefTowerWarriorsDesc(ptWarriors);
	TH_PROCESS_ERROR(bFnRet);

	/* ���ſ��Ŷ�������ʼ��սʿ����. */
	bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THTRUE);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = initWarriors(m_ptTowerStatus->sMaxWarriors, m_sVacantPos);
	TH_PROCESS_ERROR(bFnRet);

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTower::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTower::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTower::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_ptTower->pSpCharacter);

	/* �ӵ���ͼ */
	m_pBatchNodeBullet = SpriteBatchNode::create(cszpBulletPlistPng);
	TH_PROCESS_ERROR(m_pBatchNodeBullet);

	scheduleUpdate();

	this->addChild(m_ptTower->pSpCharacter);
	this->addChild(m_pBatchNodeBullet);

	bRet = THTRUE;
Exit0:
	CthCcCharacterLoadHandler::getInstance()->uninitCharacterDesc(ptCharacterDesc);
	return bRet;
}

void CThDefTower::uninit()
{
	unscheduleUpdate();
	uninitDefTowerWarriorsDesc();

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

			/* pAniMap �Աȵ��� Anixxx.ini �е� szAniDesc. */
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

void CThDefTower::uninitDefTowerWarriorsDesc()
{
	for (short i = 0; i < THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL; i++)
	{
		for (short j = 0; j < THMAX_DEFTOWER_TARLEVEL_WARRIORS; j++)
		{
			if (NULL != m_arrpWarriorsDesc[i][j])
			{
				THFREE(m_arrpWarriorsDesc[i][j]->ptAniMap);
				THFREE(m_arrpWarriorsDesc[i][j]);
			}
		}
	}
}

thBool CThDefTower::initBaiscAnimate(char** szarrpAniDesc, const short csSize)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_ANI_DESC_PTR ptmpAniDesc = NULL;
	CHARACTER_ANI_FRAMEINFO_PTR pResAni = NULL;

	TH_PROCESS_SUCCESS(THMAX_ANI_COUNT < csSize);
	for (int i = 0; i < csSize; i++)
	{
		if (NULL != szarrpAniDesc[i])
		{
			bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharacterAniDescFromIni(szarrpAniDesc[i], &ptmpAniDesc);
			TH_PROCESS_ERROR(bFnRet);
			pResAni = THMALLOC(CHARACTER_ANI_FRAMEINFO, sizeof(CHARACTER_ANI_FRAMEINFO));
			TH_PROCESS_ERROR(pResAni);
			pResAni->pAnimate = NULL;
			strcpy_s(pResAni->szarrDesc, strlen(ptmpAniDesc->szarrAniDesc) + 1, ptmpAniDesc->szarrAniDesc);
			m_arrpAniGroup[i] = pResAni;

			bFnRet = initCharacterAnimate(ptmpAniDesc, i);
			TH_PROCESS_ERROR(bFnRet);

			CthCcCharacterLoadHandler::getInstance()->uninitCharacterAniDesc(ptmpAniDesc);
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
		/* �ؽ�����. */
		while (NULL == ptSpDesc)
		{
			sWarriorType = (rand() % THMAX_DEFTOWER_TARLEVEL_WARRIORS);
			ptSpDesc = m_arrpWarriorsDesc[m_ptTower->emCurLevel][sWarriorType];
		}
		bFnRet = _getWarArrayVacantPos(&sSpArrVacantPos);
		/* ����Ƿ�������. */
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
			m_ptTower->pSpCharacter->getPositionX(),
			m_ptTower->pSpCharacter->getPositionY(),
			m_ptTower->nAttackRadius,
			ptSpDesc->ptAniMap,
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
	/* ����Ƿ�������. */
	TH_PROCESS_SUCCESS(bFnRet);

	bFnRet = initCharacterWithPlist(m_ptBulletDesc, &ptFrBullet);
	TH_PROCESS_ERROR(bFnRet);
	m_arrpSpGroup[sVancantPos] = ptFrBullet;

	ptFrBullet->pSpCharacter->setPosition(m_ptTower->pSpCharacter->getPositionX(), m_ptTower->pSpCharacter->getPositionY());
	m_pBatchNodeBullet->addChild(ptFrBullet->pSpCharacter);

	fMoveDstX = m_ptTower->pSpCharacter->getPositionX() + m_ptTower->nAttackRadius * cos(fShootAngle * (M_PI / 180));
	fMoveDstY = m_ptTower->pSpCharacter->getPositionY() + m_ptTower->nAttackRadius * sin(fShootAngle * (M_PI / 180));

	arrpActionCallback[0] = CallFuncN::create(CC_CALLBACK_1(CThDefTower::uninitBullet, this, sVancantPos));
	getCharacterMoveSpeed(m_ptTower->pSpCharacter->getPositionX(), m_ptTower->pSpCharacter->getPositionY(), fMoveDstX, fMoveDstY, ptFrBullet->emMoveSpeed, &fMoveTime);
	bFnRet = CThBaseCharacterAction::getInstance()->createActionMoveTo(fMoveTime, fMoveDstX, fMoveDstY, arrpActionCallback, 1, &pSeBulletMove);
	TH_PROCESS_ERROR(bFnRet);
	ptFrBullet->pSpCharacter->runAction(pSeBulletMove);

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTower::uninitBullet(Node* pNode, const short csBullet)
{
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
			*pnRet = m_tAniTag.arrTag[i].sTag;
			break;
		}
	}
	return;
}

void CThDefTower::getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)
{
	*ppRet = m_ptTower;
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
		if (NULL != m_arrpAniGroup[i] && 0 == strcmp(m_arrpAniGroup[i]->szarrDesc, cszpTag))
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

void CThDefTower::getTowerInfoArcher(
	enum THEM_CHARACTER_LEVEL emLevel, char* szpArcherRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
)
{
	TH_RUN_SUCCESS(NULL != szpArcherRet, strcpy_s(szpArcherRet, MAX_PATH, "data\\CharacterConfig\\SaigyoSakura\\ChacSaigyoSakura.ini"));
	TH_RUN_SUCCESS(NULL != szpDefTowerConstruction, strcpy_s(szpDefTowerConstruction, MAX_PATH, "data\\CharacterConfig\\DefTowerSubsoil\\TexDefTowerConstructionArcher.ini"));

	switch (emLevel)
	{
	case CHARACTER_LEVEL_1:
		if (NULL != arrpAniRet && NULL != psAniSizeRet)
		{
			arrpAniRet[0] = "data\\CharacterConfig\\SaigyoSakura\\AniWarriorsMove.ini";
			arrpAniRet[1] = "data\\CharacterConfig\\SaigyoSakura\\AniOpenTheDoor.ini";
			arrpAniRet[2] = "data\\CharacterConfig\\SaigyoSakura\\AniCloseTheDoor.ini";
			arrpAniRet[3] = "data\\CharacterConfig\\SaigyoSakura\\AniTagWarriorsDie.ini";
			*psAniSizeRet = 4;
		}
		if (NULL != arrpWarriorsRet && NULL != psWarriorsCnt)
		{
			arrpWarriorsRet[0] = "data\\CharacterConfig\\SaigyoSakura\\ChacWarrior.ini";
			*psWarriorsCnt = 1;
		}
		break;

	case CHARACTER_LEVEL_2:
		break;
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
	/* arrnCondAniTag: int ���飬����Action tag, ����tagָ���actionȫ�����Ž����󣬲�ִ�й��Ŷ���. */
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

	/* ��鿪�Ŷ����Ƿ񲥷���. */
	getAniTagByDesc("AniTagTowerSummon", &nAniSummonTag);
	TH_PROCESS_ERROR(nAniSummonTag);
	if (NULL != m_ptTower->pSpCharacter->getActionByTag(nAniSummonTag))
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
	m_ptTower->pSpCharacter->runAction(pAniOpDor->pAnimate);
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
	m_ptTower->pSpCharacter->runAction(pAniClDor->pAnimate);
	pAniClDor->pAnimate->setTag(nAniSummonTag);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::_setPlayAniWarriorsDie(CThDefTowerWarrior_ptr pSp)
{
	thBool bRet = THFALSE;
	CHARACTER_ANI_FRAMEINFO_PTR pAniDie = NULL;
	CHARACTER_ANI_MAP_PTR ptAniMap = NULL;
	int nAniWarriorDie = 0;
	short sVancatPos = 0;

	pSp->getWarriorAniMap(&ptAniMap);
	TH_PROCESS_ERROR(ptAniMap);
	getAniTagByDesc("AniTagWarriorsDie", &nAniWarriorDie);
	TH_PROCESS_ERROR(nAniWarriorDie);
	getAniFrameInfoByTag(ptAniMap->szarrAniDie, &pAniDie);
	TH_PROCESS_ERROR(pAniDie);
	bRet = pSp->usSetWarriorDie(nAniWarriorDie, pAniDie);
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

	if (m_ptTower->nAttackCD < time(NULL) - m_dLastAttack)
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
	const thBool bIsNeedSummon = m_ptTowerStatus->sMaxWarriors > m_ptTowerStatus->sCurWarriors;
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { m_sVacantPos, };

	bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THFALSE);
	TH_PROCESS_ERROR(bFnRet);

	// ��ȡ������սʿ������Ϣ����һЩ�������.
	for (short i = 0; i < THMAX_DEFTOWER_TARLEVEL_WARRIORS; i++)
	{
		ptmpSpWarr = m_arrpWarriors[i];
		if (NULL != ptmpSpWarr)
		{
			bFnRet = _monitoringWarriorsHealthy(ptmpSpWarr, &bRet);
			TH_PROCESS_ERROR(bFnRet);
			if (THTRUE == bRet)
			{
				/* �� bRet ��������, ���껹ԭ, ͵���ڴ�. */
				m_arrpWarriors[i] = NULL;
				bRet = THFALSE;
			}
		}
	}

	// ����Ƿ���Ҫ�������飺���HP���������CD����龫�����������Ƿ�Ϊ��.
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

thBool CThDefTower::_monitoringWarriorsHealthy(CThDefTowerWarrior_ptr pSp, pthBool pbIsRelease)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	CHARACTER_FRAMEINFO_PTR pSpFrame = NULL;
	double dWarriorInPositionTime = pSp->getWarriorInPositionTime();

	pSp->getCharacterFrameInfo(&pSpFrame);
	TH_PROCESS_ERROR(pSpFrame);

	/* ��������, �ͷ�. */
	if (0 >= pSpFrame->nHP)
	{
		switch (pSpFrame->nHP)
		{
		case THSP_FLAG_DIE:
			bFnRet = _setPlayAniWarriorsDie(pSp);
			TH_PROCESS_ERROR(bFnRet);
			break;

		case THSP_FLAG_CLEAN:
			/* HP = 0 ʱ����DIE״̬, ������Ϊ�߳��л�, ���Ŷ�����Ҫʱ��, ����������������HP=-1, ������Ի���. */
			/* ���ﲻ��Ҫ�� delete, uninit ���� removeFromParentAndCleanup ������ͷ�. */
			pSp->uninit();

			m_ptTowerStatus->sCurWarriors--;
			m_dLastDieWarriors = time(NULL);
			/* ���ϲ�������ֱ���ÿ�, ���ս���CC����. */
			*pbIsRelease = THTRUE;

		default:
			break;
		}
	}
	else if (pSpFrame->nSupportDuration < time(NULL) - dWarriorInPositionTime)
	{
		/* ���鳬ʱ, �ͷ�. */
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

