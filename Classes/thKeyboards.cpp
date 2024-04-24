/********************************************************
* Filename		: thKeyboards.h
* Creator		: ac
* Date time		: 2024.04.15
* Description	: keyboard ctrl.
********************************************************/


#include "thDeviceControl.h"
#include "thBaseMacro.h"
/*
CThKeyboard* CThKeyboard::m_pSelf;


CThKeyboard::CThKeyboard()
{
}

CThKeyboard::~CThKeyboard()
{
}

thBool CThKeyboard::init(CThPlayer* pCurTag)
{
	thBool bRet = THFALSE;
	m_pListener = pCurTag;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThKeyboard::listener(Node* pLister)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	EventDispatcher* pMarisaListener = Director::getInstance()->getEventDispatcher();
	EventListenerKeyboard* pKeyboard = EventListenerKeyboard::create();

	bFnRet = _KeyDownEvent(pKeyboard);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = _KeyUpEvent(pKeyboard);
	TH_PROCESS_ERROR(bFnRet);

	pMarisaListener->addEventListenerWithSceneGraphPriority(pKeyboard, pLister);

	bRet = THTRUE;
Exit0:
	return bRet;
}

CThKeyboard* CThKeyboard::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CThKeyboard();
	}
	return m_pSelf;
}

thBool CThKeyboard::_KeyDownEvent(EventListenerKeyboard* pKeyboard)
{
	thBool bRet = THFALSE;
	pKeyboard->onKeyPressed = [=](EventKeyboard::KeyCode keycode, cocos2d::Event* event)
	{
		switch (keycode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			m_pListener->setPlayerFsmCurStatus(THEM_CHARACTERFSM_STATUS::CMS_LEFTMOVE);
			CCLOG("left");
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			m_pListener->setPlayerFsmCurStatus(THEM_CHARACTERFSM_STATUS::CMS_RIGHTMOVE);
			CCLOG("right");
			break;
		}
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		{
			break;
		}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		{
			break;
		}
		case EventKeyboard::KeyCode::KEY_Z:
		{
			break;
		}
		default:
			break;
		}
	};

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThKeyboard::_KeyUpEvent(EventListenerKeyboard* pKeyboard)
{
	thBool bRet = THFALSE;

	pKeyboard->onKeyReleased = [=](EventKeyboard::KeyCode keycode, cocos2d::Event* event)
	{
		switch (keycode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			break;
		}
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		{
			break;
		}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		{
			break;
		}
		case EventKeyboard::KeyCode::KEY_Z:
		{
			break;
		}
		default:
			break;
		}
		m_pListener->setPlayerFsmCurStatus(THEM_CHARACTERFSM_STATUS::CMS_STANDBY);
	};

	
	
	bRet = THTRUE;
Exit0:
	return bRet;
}
*/