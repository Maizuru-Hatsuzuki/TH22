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

thBool CThDefTower::init(CHARACTER_DESC_PTR pTowerDesc, CHARACTER_ANI_DESC_PTR* arrpAniDesc)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	EventListenerMouse* pMouse = EventListenerMouse::create();
	m_arrpSpGroup = THMALLOC(CHARACTER_FRAMEINFO_PTR, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	TH_PROCESS_ERROR(m_arrpSpGroup);
	m_arrpAniGroup = THMALLOC(CHARACTER_ANI_FRAMEINFO_PTR, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);
	TH_PROCESS_ERROR(m_arrpAniGroup);

	memset(m_arrpSpGroup, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SP_COUNT);
	memset(m_arrpAniGroup, 0, sizeof(CHARACTER_ANI_FRAMEINFO_PTR) * THMAX_ANI_COUNT);

	bFnRet = initCharater(pTowerDesc, &m_pTower);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = _initBaiscAnimate(arrpAniDesc);
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(m_pTower->pSpCharater);

	pMouse->onMouseUp = CC_CALLBACK_1(CThDefTower::onMouseUp, this);
	pMouse->onMouseMove = CC_CALLBACK_1(CThDefTower::onMouseMove, this);
	pMouse->onMouseDown = CC_CALLBACK_1(CThDefTower::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_pTower->pSpCharater);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThDefTower::initCharaterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos)
{
	thBool bRet = THFALSE;
	char szarrPlistPath[MAX_PATH] = { 0 };
	char szarrPlistPngPath[MAX_PATH] = { 0 };
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();

	sprintf_s(szarrPlistPath, "%s.plist", pAniDesc->szarrBasicFrameAniPath);
	sprintf_s(szarrPlistPngPath, "%s.png", pAniDesc->szarrBasicFrameAniPath);

	pSpFrameCache->addSpriteFramesWithFile(pAniDesc->szarrBasicFrameAniPath);
	
	bRet = initCharaterAnimation(pAniDesc, &m_arrpAniGroup[cnGroupPos]->pAnimate);
	TH_PROCESS_ERROR(bRet);
	
	bRet = THTRUE;
Exit0:
	pSpFrameCache->removeSpriteFramesFromFile(szarrPlistPath);
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
	return;
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

			bFnRet = CThBaseAnimation::getInstance()->createPlayAnimationWithPList(ptmpAniDesc, &pResAni->pAnimate);
			TH_PROCESS_ERROR(bFnRet);

			m_arrpAniGroup[i] = pResAni;
		}
	}

Exit0:
	return bRet;
}










void CThHunterCabin::initAnimation()
{
	char szarrAniPath[MAX_PATH] = { 0 };
	Animation* pAniOpenTheDoor = Animation::create();
	Animation* pAniCloseTheDoor = Animation::create();
	SpriteFrame* ptmpTex = NULL;

	for (int i = 2; i <= 5; i++)
	{
		sprintf_s(szarrAniPath, "image\\sprite\\kr\\hunterCabin%d.png", i);
		ptmpTex = Sprite::create(szarrAniPath)->getSpriteFrame();
		ASSERT(ptmpTex);
		pAniOpenTheDoor->addSpriteFrame(ptmpTex);
	}
	pAniOpenTheDoor->setLoops(1);
	pAniOpenTheDoor->setRestoreOriginalFrame(false);
	pAniOpenTheDoor->setDelayPerUnit(0.1f);
	m_pAniOpenTheDoor = Animate::create(pAniOpenTheDoor);

	for (int i = 5; i >= 2; i--)
	{
		sprintf_s(szarrAniPath, "image\\sprite\\kr\\hunterCabin%d.png", i);
		pAniCloseTheDoor->addSpriteFrameWithFile(szarrAniPath);
	}
	pAniCloseTheDoor->setLoops(1);
	pAniCloseTheDoor->setRestoreOriginalFrame(false);
	pAniCloseTheDoor->setDelayPerUnit(0.1f);
	m_pAniCloseTheDoor = Animate::create(pAniCloseTheDoor);
}

thBool CThHunterCabin::createFighter()
{
	thBool bRet = THFALSE;
	Sprite* ptmpSp = NULL;
	const int cnMaxFighter = 1;
	
	// 获取防御塔战士精灵信息。
	// 检查是否需要创建精灵：检查HP，检查重生CD，检查精灵数组内容是否为空。
	// 获取需要重建的精灵数量。
	// 重建精灵，并显示移动到初始位置，精灵播放移动动作，防御塔播放创建动作。
	// 精灵到达指定位置后播放待机动作。

	for (int i = 0; i < cnMaxFighter; i++)
	{
		GetTickCount64()
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThHunterCabin::destoryFighter()
{
	thBool bRet = THFALSE;
	// 检查精灵释放条件：精灵死亡，精灵持续时间超时。
	// 播放精灵死亡动画。
	// 获取精灵标识，逐步释放内存。
	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThHunterCabin::onMouseUp(EventMouse* pEvent)
{
	bool bRet = THFALSE;

	initAnimation();
	bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(pEvent->getLocationInView());
	if (true == bRet)
	{
		m_pSpHunterCabin->runAction(m_pAniOpenTheDoor);
		m_pSpHunterCabin->runAction(m_pAniCloseTheDoor);
	}

}

void CThHunterCabin::initMouseEvent(Node* pScene)
{
	EventListenerMouse* pMouse = EventListenerMouse::create();
	pMouse->onMouseUp = CC_CALLBACK_1(CThHunterCabin::onMouseUp, this);
	pMouse->onMouseMove;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, m_pSpHunterCabin);

Exit0:
	return;
}
