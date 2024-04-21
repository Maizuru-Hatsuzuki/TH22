/********************************************************
* Filename		: thDefTower.cpp
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower.
********************************************************/

#include "thDefTower.h"
#include "thBaseAnimation.h"
#include "thBaseMacro.h"


CThHunterCabin::CThHunterCabin()
{
}

CThHunterCabin::~CThHunterCabin()
{
}

thBool CThHunterCabin::init(PLAYER_DESC_PTR ptPlayer)
{
	thBool bRet = THFALSE;
	

	m_pSpHunterCabin = Sprite::create("image\\sprite\\kr\\hunterCabin2.png");
	TH_PROCESS_ERROR(m_pSpHunterCabin);

	m_pSpHunterCabin->setPosition(ptPlayer->fPosX, ptPlayer->fPosY);
	m_pSpHunterCabin->setScale(ptPlayer->fScale);


	


	
	this->addChild(m_pSpHunterCabin);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThHunterCabin::getPlayer(Sprite** pRet)
{
	thBool bRet = THFALSE;
	*pRet = m_pSpHunterCabin;
	bRet = THTRUE;
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
		CCLOG("%s", szarrAniPath);
		pAniCloseTheDoor->addSpriteFrameWithFile(szarrAniPath);
	}
	pAniCloseTheDoor->setLoops(1);
	pAniCloseTheDoor->setRestoreOriginalFrame(false);
	pAniCloseTheDoor->setDelayPerUnit(0.1f);
	m_pAniCloseTheDoor = Animate::create(pAniCloseTheDoor);
}

void CThHunterCabin::initMouseEvent(Node* pScene)
{
	bool bRet = THFALSE;
	EventListenerMouse* pMouse = EventListenerMouse::create();
	
	pMouse->onMouseUp = [&](EventMouse* pEvent)
		{
			initAnimation();
			bRet = pEvent->getCurrentTarget()->getBoundingBox().containsPoint(m_pSpHunterCabin->getPosition());
			CCLOG("%f, %f", pEvent->getCurrentTarget()->getBoundingBox().getMaxX(), pEvent->getCurrentTarget()->getBoundingBox().size.height);
			if (true == bRet)
			{
				CCLOG("sb");
				//
			}
			m_pSpHunterCabin->runAction(m_pAniOpenTheDoor);
			m_pSpHunterCabin->runAction(m_pAniCloseTheDoor);
		};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pMouse, pScene);

Exit0:
	return;
}
