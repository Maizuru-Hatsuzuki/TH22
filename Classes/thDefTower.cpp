/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower.
********************************************************/

#include "thCcDefTower.h"
#include "thCcAnimation.h"
#include "thBaseMacro.h"


float CThDefTower::ms_fWarriorsBirthAngle;


CThDefTower::CThDefTower()
{
}

CThDefTower::~CThDefTower()
{
}

thBool CThDefTower::init(
	const char* cszpDefTowerCharacterDescPath,
	enum THEM_BULLET emBullet,
	char** szarrpAniDesc,
	const short csAniDescSize,
	const DEFTOWER_WARRIORS_PTR ptWarriors,
	float fFaceAngle
)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	char szarrBulletDescPath[MAX_PATH] = { 0 };
	char szarrBulletPlistPng[MAX_PATH] = { 0 };
	short arrnAniTag[THMAX_DEFTOWER_SYNC_ANI] = { m_sVacantPos, };
	EventListenerMouse* pMouse = EventListenerMouse::create();
	CHARACTER_DESC_PTR ptCharacterDesc = NULL;
	Node* pSpDefTowerSubsoil = this->getParent()->getChildByName("SP_DefTowerSubsoil");
	TH_PROCESS_ERROR(pSpDefTowerSubsoil);

	CThDefTower::ms_fWarriorsBirthAngle = fFaceAngle;
	m_sVacantPos = 0;
	m_dLastSummonWarriors = 0.f;
	m_dLastAttack = 0.f;
	m_ptBulletDesc = NULL;
	m_ptTowerStatus = NULL;
	m_tChacFrameQuickMenuBg = { NULL, NULL, NULL };
	m_tAniTag.sOffset = THMAX_ANI_COUNT;
	/* AniTagTowerSummon nTag = 33 */
	m_tAniTag.arrTag[0].sTag = m_tAniTag.sOffset + 1;
	m_tAniTag.arrTag[0].cszpDesc = "AniTagTowerSummon";
	/* AniTagWarriorsDie nTag = 34 */
	m_tAniTag.arrTag[1].sTag = m_tAniTag.sOffset + 2;
	m_tAniTag.arrTag[1].cszpDesc = "AniTagWarriorsDie";

	getBulletInfo(emBullet, szarrBulletPlistPng, szarrBulletDescPath);
	TH_PROCESS_ERROR(0 != strcmp("\0", szarrBulletDescPath));

	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(cszpDefTowerCharacterDescPath, &ptCharacterDesc);
	TH_PROCESS_ERROR(bFnRet);
	ptCharacterDesc->fPosX = pSpDefTowerSubsoil->getPositionX();
	ptCharacterDesc->fPosY = pSpDefTowerSubsoil->getPositionY();
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrBulletDescPath, &m_ptBulletDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = CthCcCharacterLoadHandler::getInstance()->getDefTowerDescFromIni(cszpDefTowerCharacterDescPath, &m_ptTowerStatus);
	TH_PROCESS_ERROR(bFnRet);

	m_emTowerType = (THEM_DEFTOWER_TYPE)m_ptTowerStatus->nDefTowerProfessional;

	m_arrpSpGroup = THMALLOC(CHARACTER_FRAMEINFO_PTR, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	TH_PROCESS_ERROR(m_arrpSpGroup);
	m_arrpAniGroup = THMALLOC(CHARACTER_ANI_FRAMEINFO_PTR, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	TH_PROCESS_ERROR(m_arrpAniGroup);
	m_arrpWarriors = THMALLOC(CThDefTowerWarrior_ptr, sizeof(CThDefTowerWarrior_ptr) * THMAX_DEFTOWER_TARLEVEL_WARRIORS);
	TH_PROCESS_ERROR(m_arrpWarriors);

	memset(m_arrpSpGroup, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	memset(m_arrpAniGroup, 0, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	memset(m_arrpWarriors, 0, sizeof(CThDefTowerWarrior_ptr) * THMAX_DEFTOWER_TARLEVEL_WARRIORS);

	/* ����������. */
	bFnRet = initCharacterWithPlist(ptCharacterDesc, &m_ptTower);
	TH_PROCESS_ERROR(bFnRet);
	_setSpTowerPositionTweaks();
	m_ptTower->pSpCharacter->setAnchorPoint(Vec2(0.5, 0));
	this->addChild(m_ptTower->pSpCharacter);

	bFnRet = initBaiscAnimate(szarrpAniDesc, csAniDescSize);
	TH_PROCESS_ERROR(bFnRet);
	if (THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS == m_emTowerType || THEM_DEFTOWER_TYPE::DEFTOWERTYPE_ARCHER_WARRIORS == m_emTowerType)
	{
		/* ����սʿ�̶�λ������. */
		_initWarriorsMovePos(ptCharacterDesc, fFaceAngle);

		/* ���ſ��Ŷ�������ʼ��սʿ����. */
		bFnRet = initDefTowerWarriorsDesc(ptWarriors);
		TH_PROCESS_ERROR(bFnRet);
		bFnRet = setPlayAniTowerSummon(arrnAniTag, THMAX_DEFTOWER_SYNC_ANI, THTRUE);
		TH_PROCESS_ERROR(bFnRet);
		bFnRet = initWarriors(m_ptTowerStatus->sMaxWarriors, m_sVacantPos);
		TH_PROCESS_ERROR(bFnRet);
	}

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTower::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTower::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTower::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_ptTower->pSpCharacter);

	/* �ӵ���ͼ */
	m_pBatchNodeBullet = SpriteBatchNode::create(szarrBulletPlistPng);
	TH_PROCESS_ERROR(m_pBatchNodeBullet);
	this->addChild(m_pBatchNodeBullet);

	scheduleUpdate();

	bRet = THTRUE;
Exit0:
	if (NULL != ptCharacterDesc)
	{
		CthCcCharacterLoadHandler::getInstance()->uninitCharacterDesc(ptCharacterDesc);
	}
	return bRet;
}

void CThDefTower::uninit()
{
	unscheduleUpdate();
	uninitDefTowerWarriorsDesc();

	this->removeAllChildrenWithCleanup(THTRUE);
	this->removeFromParentAndCleanup(THTRUE);

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
	THFREE(m_ptTower);
	THFREE(m_ptTowerStatus);

	return;
}

void CThDefTower::_initWarriorsMovePos(const CHARACTER_DESC_PTR cptCharacterDesc, const float cfFaceAngle)
{
	float fWarriorsSeparationAngle = 40.f;
	const float cfWarriorsSeparationDis = 25.f;
	float fWarriorCenterPointX = 0.f;
	float fWarriorCenterPointY = 0.f;

	/* ���������������.*/
	fWarriorCenterPointX = cptCharacterDesc->fPosX + m_ptTower->nAttackRadius * cos(cfFaceAngle * (M_PI / 180));
	fWarriorCenterPointY = cptCharacterDesc->fPosY + m_ptTower->nAttackRadius * sin(cfFaceAngle * (M_PI / 180));
	/* ����ɢ�ֿ��������. */
	for (short s = 0; s < THMAX_DEFTOWER_TARLEVEL_WARRIORS; s++)
	{
		if (s < m_ptTowerStatus->sMaxWarriors)
		{
			m_arrfWarriorMovePos[s][0] = fWarriorCenterPointX + cfWarriorsSeparationDis * cos(fWarriorsSeparationAngle * (M_PI / 180));
			m_arrfWarriorMovePos[s][1] = fWarriorCenterPointY + cfWarriorsSeparationDis * sin(fWarriorsSeparationAngle * (M_PI / 180));
			fWarriorsSeparationAngle += 90.f;
			fWarriorsSeparationAngle = fmod(fWarriorsSeparationAngle, 360.f);
		}
		else
		{
			m_arrfWarriorMovePos[s][0] = 0.f;
			m_arrfWarriorMovePos[s][1] = 0.f;
		}
	}
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
		if (i < ptWarriors->sSize && NULL != ptWarriors->arrpTowerWarriorsDesc[i])
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

	TH_PROCESS_SUCCESS(THMAX_DEFTOWER_TARLEVEL_WARRIORS < csCnt);
	for (int i = 0; i < csCnt; i++)
	{
		/* �ؽ�����. */
		while (NULL == ptSpDesc)
		{
			sWarriorType = (rand() % THMAX_DEFTOWER_TARLEVEL_WARRIORS);
			ptSpDesc = m_arrpWarriorsDesc[m_ptTower->emCurLevel][sWarriorType];
		}
		/* ����Ƿ�������. */
		bFnRet = _getWarArrayVacantPos(&sSpArrVacantPos);
		TH_PROCESS_SUCCESS(bFnRet);
		getAniFrameInfoByTag(ptSpDesc->ptAniMap->szarrAniMoveTransverse, &ptAniMoveTo);
		TH_PROCESS_ERROR(ptAniMoveTo);
		getWarriorExistsByAngle(CThDefTower::ms_fWarriorsBirthAngle, &bFnRet);
		if (THTRUE == bFnRet)
		{
			CThDefTower::ms_fWarriorsBirthAngle += 15.f;
			CThDefTower::ms_fWarriorsBirthAngle = fmodf(CThDefTower::ms_fWarriorsBirthAngle, 360.f);
		}

		ptmpWarrior = THNEW_CLASS(CThDefTowerWarrior);
		TH_PROCESS_ERROR(ptmpWarrior);
		bFnRet = ptmpWarrior->init(
			ptSpDesc,
			sSpArrVacantPos,
			CThDefTower::ms_fWarriorsBirthAngle,
			m_ptTower->pSpCharacter->getPositionX(),
			m_ptTower->pSpCharacter->getPositionY(),
			m_arrfWarriorMovePos[sSpArrVacantPos],
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
	ptFrBullet->pSpCharacter->setRotation(-fShootAngle);
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

thBool CThDefTower::getTowerInfo(
	enum THEM_CHARACTER_LEVEL emLevel, enum THEM_DEFTOWER_TYPE emTowerType, DEFTOWER_WARRIORS_PTR ptWarrior,
	char* szpTowerDescRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
)
{
	thBool bFnRet = THFALSE;
	char szarrLvPath[16] = { 0 };

	switch (emLevel)
	{
	case CHARACTER_LEVEL_1:
		TH_RUN_SUCCESS(NULL != ptWarrior, ptWarrior->emLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1);
		strcpy_s(szarrLvPath, strlen("LV1") + 1, "LV1");
		break;

	case CHARACTER_LEVEL_2:
		TH_RUN_SUCCESS(NULL != ptWarrior, ptWarrior->emLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_2);
		strcpy_s(szarrLvPath, strlen("LV2") + 1, "LV2");
		break;

	case CHARACTER_LEVEL_3:
		TH_RUN_SUCCESS(NULL != ptWarrior, ptWarrior->emLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_3);
		strcpy_s(szarrLvPath, strlen("LV3") + 1, "LV3");
		break;

	case CHARACTER_LEVEL_4:
		TH_RUN_SUCCESS(NULL != ptWarrior, ptWarrior->emLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4);
		strcpy_s(szarrLvPath, strlen("LV4") + 1, "LV4");
		break;

	case CHARACTER_LEVEL_5:
		TH_RUN_SUCCESS(NULL != ptWarrior, ptWarrior->emLevel = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_5);
		strcpy_s(szarrLvPath, strlen("LV5") + 1, "LV5");
		break;

	case CHARACTER_MAXLEVEL:
		break;
	default:
		break;
	}

	switch (emTowerType)
	{
	case DEFTOWERTYPE_UNKNOW:
		break;

	case DEFTOWERTYPE_ARCHER:
		break;

	case DEFTOWERTYPE_WARRIORS:
		bFnRet = CThDefTower::getTowerInfoWarriors(szpTowerDescRet, arrpAniRet, psAniSizeRet, arrpWarriorsRet, psWarriorsCnt, szpDefTowerConstruction, szarrLvPath);
		TH_PROCESS_ERROR(bFnRet);

		if (NULL != ptWarrior)
		{
			for (short j = 0; j < *psWarriorsCnt; j++)
			{
				bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
					arrpWarriorsRet[j], &(ptWarrior->arrpTowerWarriorsDesc[j])
				);
				TH_PROCESS_ERROR(bFnRet);
			}
			ptWarrior->sSize = *psWarriorsCnt;
		}
		break;

	case DEFTOWERTYPE_ARCHER_WARRIORS:
		break;

	default:
		break;
	}

	bFnRet = THTRUE;
Exit0:
	return bFnRet;
}

void CThDefTower::getTowerInfoArcher(
	enum THEM_CHARACTER_LEVEL emLevel, char* szpArcherRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
)
{
	TH_RUN_SUCCESS(NULL != szpArcherRet, strcpy_s(szpArcherRet, MAX_PATH, "data\\CharacterConfig\\SaigyoSakura\\ChacSaigyoSakura.ini"));
	TH_RUN_SUCCESS(NULL != szpDefTowerConstruction, strcpy_s(szpDefTowerConstruction, MAX_PATH, "data\\CharacterConfig\\DefTowerSubsoil\\ChacDefTowerConstructionArcher.ini"));

	switch (emLevel)
	{
	case CHARACTER_LEVEL_1:
		if (NULL != arrpAniRet && NULL != psAniSizeRet)
		{
			*psAniSizeRet = 0;
		}
		if (NULL != arrpWarriorsRet && NULL != psWarriorsCnt)
		{
			*psWarriorsCnt = 0;
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

thBool CThDefTower::getTowerInfoWarriors(
	char* szpTowerDescRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction, char* szpLv
)
{
	thBool bRet = THFALSE;
	char sztmpPath[MAX_PATH] = { 0 };

	TH_RUN_SUCCESS(NULL != szpTowerDescRet, sprintf_s(szpTowerDescRet, MAX_PATH, "data\\CharacterConfig\\Barracks\\%s\\ChacBarracks.ini", szpLv));
	TH_RUN_SUCCESS(NULL != szpDefTowerConstruction, strcpy_s(szpDefTowerConstruction, MAX_PATH, "data\\CharacterConfig\\DefTowerSubsoil\\ChacDefTowerConstructionBarracks.ini"));

	if (NULL != arrpAniRet && NULL != psAniSizeRet)
	{
		sprintf_s(sztmpPath, "data\\CharacterConfig\\Barracks\\%s\\AniWarriorsMove.ini", szpLv);
		arrpAniRet[0] = THMALLOC(char, MAX_PATH);
		TH_PROCESS_ERROR(arrpAniRet[0]);
		strcpy_s(arrpAniRet[0], MAX_PATH, sztmpPath);

		sprintf_s(sztmpPath, "data\\CharacterConfig\\Barracks\\%s\\AniOpenTheDoor.ini", szpLv);
		arrpAniRet[1] = THMALLOC(char, MAX_PATH);
		TH_PROCESS_ERROR(arrpAniRet[1]);
		strcpy_s(arrpAniRet[1], MAX_PATH, sztmpPath);

		sprintf_s(sztmpPath, "data\\CharacterConfig\\Barracks\\%s\\AniCloseTheDoor.ini", szpLv);
		arrpAniRet[2] = THMALLOC(char, MAX_PATH);
		TH_PROCESS_ERROR(arrpAniRet[2]);
		strcpy_s(arrpAniRet[2], MAX_PATH, sztmpPath);

		sprintf_s(sztmpPath, "data\\CharacterConfig\\Barracks\\%s\\AniTagWarriorsDie.ini", szpLv);
		arrpAniRet[3] = THMALLOC(char, MAX_PATH);
		TH_PROCESS_ERROR(arrpAniRet[3]);
		strcpy_s(arrpAniRet[3], MAX_PATH, sztmpPath);

		*psAniSizeRet = 4;
	}
	if (NULL != arrpWarriorsRet && NULL != psWarriorsCnt)
	{
		sprintf_s(sztmpPath, "data\\CharacterConfig\\Barracks\\%s\\ChacWarrior.ini", szpLv);
		arrpWarriorsRet[0] = THMALLOC(char, MAX_PATH);
		TH_PROCESS_ERROR(arrpWarriorsRet[0]);
		strcpy_s(arrpWarriorsRet[0], MAX_PATH, sztmpPath);

		*psWarriorsCnt = 1;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
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

void CThDefTower::_setSpTowerPositionTweaks()
{
	/* ΢������������λ��, ��ʱ��������Դ��С��覴�, �Բ��ϵػ�, ������΢��λ��. */
	switch (m_emTowerType)
	{
	case THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS:
		_setSpTowerPositionTweaksWarrior();

	default:
		break;
	}
	return;
}

void CThDefTower::_setSpTowerPositionTweaksWarrior()
{
	switch (m_ptTower->emCurLevel)
	{
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4:
		m_ptTower->pSpCharacter->setPositionX(m_ptTower->pSpCharacter->getPositionX() - 3);
		m_ptTower->pSpCharacter->setPositionY(m_ptTower->pSpCharacter->getPositionY() + 12);
		break;
	default:
		break;
	}
	return;
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

thBool CThDefTower::_setCreateQmWarrior(const thBool cbIsCreate)
{
	thBool bRet = THFALSE;
	ScaleTo* pAcQuickMenuBgScale = NULL;

	if (THTRUE == cbIsCreate)
	{
		pAcQuickMenuBgScale = ScaleTo::create(0.1f, 0.7f, 0.7f);

		switch (m_ptTower->emCurLevel)
		{
		case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4:
			bRet = CThDefTowerQuickMenu::getInstance()->createQmWarriorLevel4(
				m_ptTower->pSpCharacter->getPositionX(),
				m_ptTower->pSpCharacter->getPositionY() + m_ptTower->pSpCharacter->getBoundingBox().size.height / 2,
				m_ptTower->pSpCharacter->getScale(),
				&m_tChacFrameQuickMenuBg
			);
			TH_PROCESS_ERROR(bRet);
			this->addChild(CThDefTowerQuickMenu::getInstance());
			CThDefTowerQuickMenu::getInstance()->runAction(pAcQuickMenuBgScale);
			break;

		default:
			break;
		}
	}
	else
	{
		switch (m_ptTower->emCurLevel)
		{
		case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4:
			bRet = CThDefTowerQuickMenu::getInstance()->destoryQmWarriorLevel4(&m_tChacFrameQuickMenuBg);
			TH_PROCESS_ERROR(bRet);
			this->removeChild(CThDefTowerQuickMenu::getInstance());
			break;

		default:
			break;
		}
	}

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
	thBool bRet = THFALSE;

	bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(pEvent->getLocationInView());

Exit0:
	return;
}

void CThDefTower::onMouseDown(EventMouse* pEvent)
{
	thBool bRet = THFALSE;
	ssize_t lQmSpCnt = CThDefTowerQuickMenu::getInstance()->getChildrenCount();
	bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(pEvent->getLocationInView());

	if (bRet && 0L == lQmSpCnt)
	{
		bRet = thOnClickCreateQucikMenu();
		ASSERT(bRet);
	}
	else
	{
		bRet = thOnClickDestoryQucikMenu();
		ASSERT(bRet);
	}

	return;
}

void CThDefTower::onMouseMove(EventMouse* pEvent)
{

}

thBool CThDefTower::thOnClickCreateQucikMenu()
{
	thBool bRet = THFALSE;
	CHARACTER_DESC tQuickMenuBg = { 0 };
	CHARACTER_DESC tCommandMovement = { 0 };
	ScaleTo* pAcQuickMenuBgScale = NULL;

	switch (m_emTowerType)
	{
	case DEFTOWERTYPE_UNKNOW:
		break;
	case DEFTOWERTYPE_ARCHER:
		break;
	case DEFTOWERTYPE_WARRIORS:
		bRet = _setCreateQmWarrior(THTRUE);
		TH_PROCESS_ERROR(bRet);
		break;
	case DEFTOWERTYPE_ARCHER_WARRIORS:
		break;
	default:
		break;
	}
	
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::thOnClickDestoryQucikMenu()
{
	thBool bRet = THFALSE;

	switch (m_emTowerType)
	{
	case DEFTOWERTYPE_UNKNOW:
		break;
	case DEFTOWERTYPE_ARCHER:
		break;
	case DEFTOWERTYPE_WARRIORS:
		bRet = _setCreateQmWarrior(THFALSE);
		TH_PROCESS_ERROR(bRet);
		break;
	case DEFTOWERTYPE_ARCHER_WARRIORS:
		break;
	default:
		break;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThDefTower::update(float dt)
{
	thBool bFnRet = THFALSE;

	bFnRet = globalMonitoring();
	ASSERT(bFnRet);

	//bFnRet = execTowerShoot(m_ptTowerStatus->sMaxBullets);
	//ASSERT(bFnRet);
	
	return;
}

thBool CThDefTower::globalMonitoring()
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	
	if (THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS == m_emTowerType || THEM_DEFTOWER_TYPE::DEFTOWERTYPE_ARCHER_WARRIORS == m_emTowerType)
	{
		bFnRet = globalMonitoringWarriors();
		TH_PROCESS_ERROR(bFnRet);
	}

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


